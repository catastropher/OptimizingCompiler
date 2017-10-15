#include <map>
#include <map>

#include "Token.hpp"

static std::map<std::string, TokenType> languageKeywords = 
{
    { "let", TOK_LET },
    { "title", TOK_TITLE },
    { "begin", TOK_BEGIN },
    { "end", TOK_END },
    { "var", TOK_VAR },
    { "int", TOK_INT },
    { "list", TOK_LIST },
    { "rem", TOK_REM },
    { "label", TOK_LABEL },
    { "if", TOK_IF },
    { "then", TOK_THEN },
    { "goto", TOK_GOTO },
    { "input", TOK_INPUT },
    { "prompt", TOK_PROMPT },
    { "for", TOK_FOR },
    { "to", TOK_TO },
    { "by", TOK_BY }
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

