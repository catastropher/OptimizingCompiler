#include <cctype>
#include <stdexcept>
#include <iostream>

#include "Lexer.hpp"

Lexer::Lexer::Lexer(std::string inputString_)
    : inputString(inputString_)
{
    begin = &inputString[0];
    end = &inputString[inputString.length()];
}

void Lexer::lexTokens()
{
    while(lexNextToken()) ;
}

void Lexer::addToken(const char* tokenEnd, TokenType type)
{
    tokens.push_back(Token(type, std::string(begin, tokenEnd)));
    begin = tokenEnd;    
}

bool Lexer::lexNextToken()
{
    consumeWhitespace();
    
    if(begin >= end)
        return false;
    
    if(lexNumber())
        return true;
    
    if(lexId())
        return true;
    
    if(lexOperator())
        return true;
    
    throwErrorAtCurrentLocation("Unexpected token");
    return false;
}

void Lexer::consumeWhitespace()
{
    while(*begin == ' ' || *begin == '\t' || *begin == '\n' || *begin == '\r')
        ++begin;    
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
    if(!isalpha(*begin))
        return false;
    
    const char* tokenEnd = begin;
    
    do
    {
        ++tokenEnd;
    } while(isalpha(*tokenEnd) || isdigit(*tokenEnd));
    
    addToken(tokenEnd, TOK_ID);
    
    return true;
}

bool Lexer::lexOperator()
{
    return lexComparisonOperator() ||
        lexArithmeticOperator() ||
        lexAssignOperator();
}

bool Lexer::lexComparisonOperator()
{
    if(*begin != '=' && *begin != '<' && *begin != '>')
        return false;
    
    const char* tokenEnd = begin + 1;
    
    if(*tokenEnd == '=')
        ++tokenEnd;
    
    std::string op = std::string(begin, tokenEnd);
    
    if(op == "=")
        return false;
    
    addToken(tokenEnd, Token::getComparisonOperatorType(op));
    
    return true;
}

bool Lexer::lexArithmeticOperator()
{
    if(*begin != '+' && *begin != '-' && *begin != '/' && *begin != '*' && *begin != '%')
        return false;
    
    addToken(begin + 1, Token::getArithmeticOperatorType(std::string(begin, begin + 1)));
    
    return true;
}

bool Lexer::lexAssignOperator()
{
    if(*begin != '=')
        return false;
    
    addToken(begin + 1, TOK_ASSIGN);
    
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
    
    throw std::runtime_error("Error in lexing");
}



