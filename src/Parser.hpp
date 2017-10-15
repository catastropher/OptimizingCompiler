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
    Token& currentToken()
    {
        if(currentTokenId < (int)tokens.size())
            return tokens[currentTokenId];
        
        return lastToken;
    }
                
    void nextToken() { ++currentTokenId; }
    
    ExpressionNode* parseExpression();
    ExpressionNode* parseTerm();
    ExpressionNode* parseFactor();
    
    std::vector<Token>& tokens;
    int currentTokenId;
    Token lastToken;
    Ast ast;
};

