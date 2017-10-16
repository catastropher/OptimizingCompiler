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
    
    void expectType(TokenType expectedType)
    {
        // TODO: name the tokens and print what was expected
        if(currentToken().type != expectedType)
            throwErrorAtCurrentLocation("Unexpected token: " + currentToken().value);
    }
                
    void nextToken()
    {
        ++currentTokenId; 
        
        if(currentToken().type == TOK_REM)
            nextToken();
    }
    
    void prevToken() { if(currentTokenId > 0) --currentTokenId; }
    
    ExpressionNode* parseExpression();
    ExpressionNode* parseTerm();
    ExpressionNode* parseFactor();
    
    bool parseVarDecl();
    void parseHeader();
    
    StatementNode* parseStatement();
    LValueNode* parseLValue();
    LetStatementNode* parseLetStatement();
    
    CodeBlockNode* parseCodeBlock(TokenType endToken);
    ForLoopNode* parseForLoop();
    WhileLoopNode* parseWhileLoop();
    IfNode* parseIf();
    
    GotoNode* parseGoto();
    LabelNode* parseLabel();
    
    ExpressionNode* parseCondition();
    
    void throwErrorAtCurrentLocation(std::string errorMessage);
    
    std::vector<Token>& tokens;
    int currentTokenId;
    Token lastToken;
    Ast ast;
};

