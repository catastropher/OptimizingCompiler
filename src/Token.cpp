#include <map>
#include <map>

#include "Token.hpp"

static std::map<std::string, TokenType> languageKeywords = 
{
    { "begin", TOK_BEGIN },
    { "by", TOK_BY },
    { "end", TOK_END },
    { "for", TOK_FOR },
    { "goto", TOK_GOTO },
    { "if", TOK_IF },
    { "input", TOK_INPUT },
    { "int", TOK_INT },
    { "label", TOK_LABEL },
    { "let", TOK_LET },
    { "list", TOK_LIST },
    { "prompt", TOK_PROMPT },
    { "rem", TOK_REM },
    { "then", TOK_THEN },
    { "title", TOK_TITLE },
    { "to", TOK_TO },
    { "var", TOK_VAR },
    { "while", TOK_WHILE }
};

static std::map<std::string, TokenType> operators = 
{
    { "==", TOK_EQ },
    { "=", TOK_ASSIGN },
    { "<", TOK_LT },
    { ">", TOK_GT },
    { "<=", TOK_LE },
    { ">=", TOK_GE },
    { "[", TOK_LSQUARE_BRACKET },
    { "]", TOK_RSQUARE_BRACKET },
    { "+", TOK_ADD },
    { "-", TOK_SUB },
    { "/", TOK_DIV },
    { "*", TOK_MUL },
    { "%", TOK_MOD },
    { "(", TOK_LPAREN },
    { ")", TOK_RPAREN }
};

TokenType Token::getKeywordType(std::string keyword)
{
    if(languageKeywords.count(keyword) != 0)
        return languageKeywords[keyword];
    
    return TOK_INVALID;
}

TokenType Token::getOperatorType(std::string op)
{
    if(operators.count(op) != 0)
        return operators[op];
    
    return TOK_INVALID;
}

