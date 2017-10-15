#pragma once

#include <vector>

#include "Token.hpp"

class Lexer
{
public:
    Lexer(std::string inputString_);
    
    std::vector<Token>& lexTokens();
    
private:
    void addToken(const char* tokenEnd, TokenType type);
    bool lexNextToken();
    void consumeWhitespace();
    Token& getLastToken();
    void advanceTo(const char* advancePos);
    
    bool lexNumber();
    bool lexId();
    void changeLastTokenTypeIfKeyword();
    void lexTitle();
    void lexComment();
    
    bool lexOperator();
    bool lexComparisonOperator();
    bool lexArithmeticOperator();
    bool lexAssignOperator();
    bool lexBracketOperator();
    
    bool lexString();
    
    std::string currentLineAndColumn();
    void printCurrentLine();
    void throwErrorAtCurrentLocation(std::string errorMessage);
    
    std::vector<Token> tokens;
    std::string inputString;
    
    const char* begin;
    const char* end;
};

