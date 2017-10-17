#pragma once

#include <vector>

#include "Token.hpp"
#include "Ast.hpp"

class Parser
{
public:
    Parser(std::vector<Token>& tokens_);
    Ast& parse();
    
private:
    Token& currentToken()
    {
        if(currentTokenId < (int)tokens->size())
            return (*tokens)[currentTokenId];

        return lastToken;
    }
    
    Token& peekNextToken()
    {
        nextToken();
        Token& next = currentToken();
        prevToken();
        
        return next;
    }

    void expectType(TokenType expectedType)
    {
        // TODO: name the tokens and print what was expected
        if(currentToken().type != expectedType)
            throwErrorAtCurrentLocation(
              "Unexpected token: " + currentToken().value
              + " Expected: " + Token::getTokenName(expectedType)
              );
    }
                
    void nextToken()
    {
        ++currentTokenId; 
        
        if(currentToken().type == TOK_REM)
            nextToken();
    }
    
    LabelNode* generateTempLabel()
    {
        return ast.addLabelNode("L_" + std::to_string(nextLabelId++), -1, -1);
    }
    
    void prevToken() { if(currentTokenId > 0) --currentTokenId; }
    
    CodeBlockNode* parseString(std::string str);
    
    ExpressionNode* parseExpression();
    ExpressionNode* parseTerm();
    ExpressionNode* parseFactor();
    FactorNode* parseVarFactor();
    
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
    
    PrintNode* parsePrint();
    PromptNode* parsePrompt();
    InputNode* parseInput();
    
    ExpressionNode* parseCondition();
    
    CodeBlockNode* transformForLoop(ForLoopNode* node);
    
    void throwErrorAtCurrentLocation(std::string errorMessage);

    std::vector<Token>* tokens;
    int currentTokenId;
    Token lastToken;
    Ast ast;
    int nextLabelId;
};
