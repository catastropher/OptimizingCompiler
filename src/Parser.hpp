#pragma once

#include <vector>

#include "Token.hpp"
#include "Ast.hpp"

class Parser
{
public:
    Parser(std::vector<Token>& tokens_);
    
    void parse();
    
private:
    Token& currentToken() { return tokens[currentTokenId]; }
    void nextToken() { ++currentTokenId; }
    
    ExpressionNode* parseExpression();
    ExpressionNode* parseTerm();
    FactorNode* parseFactor();
    
    std::vector<Token>& tokens;
    int currentTokenId;
    Token lastToken;
    Ast ast;
};

