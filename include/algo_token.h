#ifndef ALGO_TOKEN_H
#define ALGO_TOKEN_H

#include "algo_common.h"

typedef enum {
    TOKEN_EOF,
    TOKEN_ERROR,
    
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_TRUE,
    TOKEN_FALSE,
    
    TOKEN_LET,
    TOKEN_FN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_PRINT,
    
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    
    TOKEN_EQ,
    TOKEN_EQ_EQ,
    TOKEN_BANG,
    TOKEN_BANG_EQ,
    TOKEN_LT,
    TOKEN_LT_EQ,
    TOKEN_GT,
    TOKEN_GT_EQ,
    
    TOKEN_AND,
    TOKEN_OR
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    size_t length;
    int line;
    int column;
} Token;

typedef struct {
    const char* source;
    const char* start;
    const char* current;
    int line;
    int column;
} Lexer;

void lexer_init(Lexer* lexer, const char* source);
Token lexer_next_token(Lexer* lexer);
const char* token_type_name(TokenType type);

#endif
