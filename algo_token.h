#ifndef ALGO_TOKEN_H
#define ALGO_TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_ERROR,
    
    TOKEN_IDENT,
    TOKEN_NUMBER,
    
    TOKEN_LET,
    TOKEN_FN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_TRUE,
    TOKEN_FALSE,
    
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_GT,
    TOKEN_GTE,
    
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COMMA,
    TOKEN_NEWLINE,
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
    int column;
} Token;

typedef struct {
    const char* source;
    const char* current;
    int line;
    int column;
} Lexer;

void lexer_init(Lexer* lexer, const char* source);
Token lexer_next(Lexer* lexer);

#endif
