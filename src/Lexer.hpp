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
    
    std::string currentLineAndColumn();
    void printCurrentLine();
    void throwErrorAtCurrentLocation(std::string errorMessage);
    
    std::vector<Token> tokens;
    std::string inputString;
    
    const char* begin;
    const char* end;
};

