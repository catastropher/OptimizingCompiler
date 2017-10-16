#include <map>
#include <map>

#include "Token.hpp"

static std::map<std::string, TokenType> languageKeywords =
{
    { "begin", TOK_BEGIN },
    { "by", TOK_BY },
    { "endfor", TOK_ENDFOR },
    { "end", TOK_END },
    { "endwhile", TOK_ENDWHILE },
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

static std::map<TokenType, std::string> tokenNames =
{
    { TOK_INVALID, "INVALID" },
    { TOK_TITLE, "title" },
    { TOK_BEGIN, "begin" },
    { TOK_END, "end" },
    { TOK_VAR, "var" },
    { TOK_INT, "int" },
    { TOK_LIST, "list" },
    { TOK_LSQUARE_BRACKET, "[" },
    { TOK_RSQUARE_BRACKET, "]" },
    { TOK_REM, "comment (rem)" },
    { TOK_LABEL, "label" },
    { TOK_LET, "let" },
    { TOK_IF, "if" },
    { TOK_THEN, "then" },
    { TOK_GOTO, "goto" },
    { TOK_INPUT, "input"},
    { TOK_PRINT, "print"},
    { TOK_PROMPT, "prompt"},
    { TOK_EQ,"==" },
    { TOK_NE, "!="},
    { TOK_LT, "<" },
    { TOK_GT, ">" },
    { TOK_LE, "<="},
    { TOK_GE, ">="},
    { TOK_ADD, "+"},
    { TOK_SUB, "-"},
    { TOK_MUL, "*"},
    { TOK_DIV, "/"},
    { TOK_MOD, "%"},
    { TOK_NUMBER, "number"},
    { TOK_ID, "Identifier"},
    { TOK_ASSIGN, "=" },
    { TOK_FOR, "for" },
    { TOK_BY, "by" },
    { TOK_TO, "to" },
    { TOK_LPAREN, "(" },
    { TOK_RPAREN, ")" },
    { TOK_WHILE, "while" },
    { TOK_ENDFOR, "endfor" },
    { TOK_ENDWHILE, "endwhile"},
    { TOK_STRING, "string"}
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

std::string Token::getTokenName(TokenType inToken)
{
    if (tokenNames.count(inToken)!= 0)
        return tokenNames[inToken];
    return tokenNames[TOK_INVALID];
}
