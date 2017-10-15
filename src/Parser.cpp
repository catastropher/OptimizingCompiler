#include "Parser.hpp"

Parser::Parser(std::vector<Token>& tokens_)
    : tokens(tokens_),
    currentTokenId(0),
    lastToken(Token(TOK_INVALID, ""))
{
    
}

void Parser::parse()
{
    printf("Value: %d\n", parseExpression()->tryEvaluate());
}

ExpressionNode* Parser::Parser::parseExpression()
{
    if(currentToken().type == TOK_SUB || currentToken().type == TOK_ADD)
    {
        printf("Unary operator!\n");
    }
    
    return parseTerm();
}

FactorNode* Parser::parseFactor()
{
    if(currentToken().type != TOK_NUMBER)
    {
        throw "Expected integer for factor";
    }
    
    FactorNode* newNode = ast.newIntegerNode(atoi(currentToken().value.c_str()));
    nextToken();
    
    return newNode;
}

ExpressionNode* Parser::parseTerm()
{
    if(currentToken().type != TOK_NUMBER)
        return nullptr;
    
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



