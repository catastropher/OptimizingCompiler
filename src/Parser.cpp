#include <iostream>

#include "Parser.hpp"

Parser::Parser(std::vector<Token>& tokens_)
    : tokens(tokens_),
    currentTokenId(0),
    lastToken(Token(TOK_INVALID, "", -1, -1))
{
    
}

void Parser::parse()
{
    parseHeader();
}

ExpressionNode* Parser::Parser::parseExpression()
{
    ExpressionNode* result = parseTerm();
    
    while(currentToken().type == TOK_ADD || currentToken().type == TOK_SUB)
    {
        TokenType op = currentToken().type;
        nextToken();
        ExpressionNode* right = parseTerm();
        
        result = ast.newBinaryOpNode(result, op, right);
    }
    
    return result;
}

ExpressionNode* Parser::parseFactor()
{
    TokenType unaryOp = TOK_INVALID;
    
    if(currentToken().type == TOK_ADD || currentToken().type == TOK_SUB)
    {
        unaryOp = currentToken().type;
        nextToken();
    }
    
    if(currentToken().type != TOK_NUMBER)
    {
        throwErrorAtCurrentLocation("Expected number factor");
    }
    
    FactorNode* newNode = ast.newIntegerNode(atoi(currentToken().value.c_str()));
    nextToken();
    
    if(unaryOp != TOK_INVALID)
        return ast.newUnaryOpNode(newNode, unaryOp);
    
    return newNode;
}

ExpressionNode* Parser::parseTerm()
{
    ExpressionNode* result = parseFactor();
    
    while(currentToken().type == TOK_MUL || currentToken().type == TOK_DIV || currentToken().type == TOK_MOD)
    {
        TokenType op = currentToken().type;
        nextToken();
        ExpressionNode* right = parseFactor();
        
        result = ast.newBinaryOpNode(result, op, right);
    }
    
    return result;
}

void Parser::throwErrorAtCurrentLocation(std::string errorMessage)
{
    std::cerr << "Error on line " << currentToken().line << ", col " << currentToken().col << ": " << errorMessage << std::endl;
    
    throw "Error in parsing";
}

bool Parser::parseVarDecl()
{    
    if(currentToken().type == TOK_INT)
    {
        nextToken();
        expectType(TOK_ID);
        ast.addIntegerVar(currentToken().value, currentToken().line, currentToken().col);
        nextToken();
        return true;
    }
    
    if(currentToken().type == TOK_LIST)
    {
        nextToken();
        expectType(TOK_LSQUARE_BRACKET);
        nextToken();
        
        expectType(TOK_NUMBER);        
        int totalElements = atoi(currentToken().value.c_str());
        nextToken();
        
        expectType(TOK_RSQUARE_BRACKET);
        nextToken();
        
        expectType(TOK_ID);
        
        ast.add1DListVar(currentToken().value, currentToken().line, currentToken().col, totalElements);
        nextToken();
        
        return true;
    }
    
    return false;
}

void Parser::parseHeader()
{
    do
    {
        if(currentToken().type == TOK_VAR)
        {
            nextToken();
            while(parseVarDecl()) ;
        }
        else if(currentToken().type == TOK_BEGIN)
        {
            nextToken();
            return;
        }
        else if(currentToken().type == TOK_TITLE)
        {
            std::cout << "Title: " << currentToken().value << std::endl;
            nextToken();
        }
        else if(currentToken().type == TOK_REM)
        {
            nextToken();
        }
        else
        {
            throwErrorAtCurrentLocation("Unexpected token: " + currentToken().value);
        }
    } while(true);
}



