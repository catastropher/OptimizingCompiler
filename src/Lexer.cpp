#include <cctype>
#include <stdexcept>
#include <iostream>

#include "Lexer.hpp"

Lexer::Lexer(std::string inputString_)
    : inputString(inputString_),
    currentLine(1),
    currentCol(1)
{
    begin = &inputString[0];
    end = &inputString[inputString.length()];
}

std::vector<Token>& Lexer::lexTokens()
{
    while(lexNextToken()) ;
    
    return tokens;
}

void Lexer::addToken(const char* tokenEnd, TokenType type)
{
    if(type == TOK_INVALID)
        throwErrorAtCurrentLocation("Programmer error: bad token type");
    
    tokens.push_back(Token(type, std::string(begin, tokenEnd), currentLine, currentCol));
    advanceTo(tokenEnd);
}

Token& Lexer::getLastToken()
{
    if(tokens.size() == 0)
        throw "Trying to get last token when no tokens have been lexed yet";
    
    return tokens[tokens.size() - 1];
}

void Lexer::advanceTo(const char* advancePos)
{
    while(begin < advancePos)
    {
        ++currentCol;
        
        if(*begin == '\n')
        {
            ++currentLine;
            currentCol = 1;
        }
        
        ++begin;
    }
}

bool Lexer::lexNextToken()
{
    consumeWhitespace();
    
    if(begin >= end)
        return false;
    
    if(lexNumber())
        return true;
    
    if(lexId())
    {
        changeLastTokenTypeIfKeyword();
        return true;
    }
    
    if(lexOperator())
        return true;
    
    if(lexString())
        return true;
    
    throwErrorAtCurrentLocation("Unexpected token");
    return false;
}

void Lexer::consumeWhitespace()
{
    while(*begin == ' ' || *begin == '\t' || *begin == '\n' || *begin == '\r')
        advanceTo(begin + 1);    
}

bool Lexer::lexNumber()
{
    if(!isdigit(*begin))
        return false;
    
    const char* tokenEnd = begin;
    
    do
    {
        ++tokenEnd;
    } while(isdigit(*tokenEnd));
    
    addToken(tokenEnd, TOK_NUMBER);
    
    return true;
}

bool Lexer::lexId()
{
    if(!isalpha(*begin) && *begin != '_')
        return false;
    
    const char* tokenEnd = begin;
    
    do
    {
        ++tokenEnd;
    } while(isalpha(*tokenEnd) || isdigit(*tokenEnd) || *tokenEnd == '_');
    
    addToken(tokenEnd, TOK_ID);
    
    return true;
}

void Lexer::changeLastTokenTypeIfKeyword()
{
    Token& lastToken = getLastToken();
    TokenType keywordType = Token::getKeywordType(lastToken.value);
    
    if(keywordType != TOK_INVALID)
        lastToken.type = keywordType;
    
    if(keywordType == TOK_REM || keywordType == TOK_TITLE)
        lexComment();
}

void Lexer::lexComment()
{
    consumeWhitespace();
    
    const char* tokenEnd = begin;
    
    while(tokenEnd < end && *tokenEnd != '\n')
        ++tokenEnd;
    
    getLastToken().value = std::string(begin, tokenEnd);
    advanceTo(tokenEnd);
}


bool Lexer::lexOperator()
{
    return lexComparisonOperator() ||
        lexArithmeticOperator() ||
        lexAssignOperator() ||
        lexBracketOperator();
}

bool Lexer::lexComparisonOperator()
{
    if(*begin != '=' && *begin != '<' && *begin != '>' && *begin != '!')
        return false;
    
    const char* tokenEnd = begin + 1;
    
    if(*tokenEnd == '=')
        ++tokenEnd;
    
    std::string op = std::string(begin, tokenEnd);
    
    if(op == "=")
        return false;
    
    addToken(tokenEnd, Token::getOperatorType(op));
    
    return true;
}

bool Lexer::lexBracketOperator()
{
    if(*begin != '[' && *begin != ']' && *begin != '(' && *begin != ')')
        return false;
    
    std::string op = std::string(begin, begin + 1);
    addToken(begin + 1, Token::getOperatorType(op));
    
    return true;
}

bool Lexer::lexArithmeticOperator()
{
    if(*begin != '+' && *begin != '-' && *begin != '/' && *begin != '*' && *begin != '%')
        return false;
    
    addToken(begin + 1, Token::getOperatorType(std::string(begin, begin + 1)));
    
    return true;
}

bool Lexer::lexAssignOperator()
{
    if(*begin != '=')
        return false;
    
    addToken(begin + 1, TOK_ASSIGN);
    
    return true;
}

bool Lexer::lexString()
{
    if(*begin != '"')
        return false;
    
    const char* tokenEnd = begin;
    
    do
    {
        ++tokenEnd;
    }
    while(tokenEnd < end && *tokenEnd != '\n' && *tokenEnd != '"');
    
    if(*tokenEnd != '"')
        throwErrorAtCurrentLocation("Unterminated '\"'");
    
    addToken(tokenEnd + 1, TOK_STRING);
    
    return true;
}

std::string Lexer::currentLineAndColumn()
{
    int line = 1;
    int col = 1;
    
    const char* str = &inputString[0];
    while(str < begin)
    {
        if(*str == '\n')
        {
            ++line;
            col = 1;
        }
        else
        {
            ++col;
        }
        
        ++str;
    }
    
    return "line " + std::to_string(line) + ", col " + std::to_string(col);
}

void Lexer::printCurrentLine()
{
    const char* strBegin = &inputString[0];
    const char* lineStart = begin;
    
    while(lineStart > strBegin && *(lineStart - 1) != '\n')
        --lineStart;
    
    int colNumber = begin - lineStart;
    while(lineStart < end && *lineStart != '\n')
    {
        std::cerr << *lineStart;
        ++lineStart;
    }
    
    std::cerr << std::endl;
    
    for(int i = 0; i < colNumber; ++i)
        std::cerr << " ";
    
    std::cerr << "^\n";
}

void Lexer::throwErrorAtCurrentLocation(std::string errorMessage)
{
    std::cerr << "Error on " << currentLineAndColumn() << ": " << errorMessage << std::endl;
    printCurrentLine();
    
    throw "Error in lexing";
}

