#include <vector>

#include "Token.hpp"

class Lexer
{
public:
    Lexer(std::string inputString_);
    
    void lexTokens();
    
private:
    void addToken(const char* tokenEnd, TokenType type);
    bool lexNextToken();
    void consumeWhitespace();
    
    bool lexNumber();
    bool lexId();
    void changeLastTokenTypeIfKeyword();
    
    bool lexOperator();
    bool lexComparisonOperator();
    bool lexArithmeticOperator();
    bool lexAssignOperator();
    
    std::string currentLineAndColumn();
    void printCurrentLine();
    void throwErrorAtCurrentLocation(std::string errorMessage);
    
    std::vector<Token> tokens;
    std::string inputString;
    
    const char* begin;
    const char* end;
};

