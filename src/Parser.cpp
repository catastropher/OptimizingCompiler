#include "Parser.hpp"

Parser::Parser(std::vector<Token>& tokens_)
    : tokens(tokens_),
    currentTokenId(0),
    lastToken(Token(TOK_INVALID, "", -1, -1))
{
    
}

void Parser::parse()
{
    printf("Value: %d\n", parseExpression()->tryEvaluate());
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
        throw "Expected number factor";
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

