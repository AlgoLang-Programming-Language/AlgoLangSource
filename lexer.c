#include <string.h>
#include <ctype.h>
#include "../include/algo_token.h"

void lexer_init(Lexer* lexer, const char* source) {
    lexer->source = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
}

static int is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer* lexer) {
    char c = *lexer->current++;
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static char peek_next(Lexer* lexer) {
    if (is_at_end(lexer)) return '\0';
    return lexer->current[1];
}

static Token make_token(Lexer* lexer, TokenType type, const char* start, int length) {
    Token token;
    token.type = type;
    token.start = start;
    token.length = length;
    token.line = lexer->line;
    token.column = lexer->column - length;
    return token;
}

static Token error_token(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = lexer->line;
    token.column = lexer->column;
    return token;
}

static void skip_whitespace(Lexer* lexer) {
    while (!is_at_end(lexer)) {
        char c = peek(lexer);
        if (c == ' ' || c == '\r' || c == '\t') {
            advance(lexer);
        } else if (c == '#') {
            while (!is_at_end(lexer) && peek(lexer) != '\n') {
                advance(lexer);
            }
        } else {
            break;
        }
    }
}

static Token number(Lexer* lexer) {
    const char* start = lexer->current - 1;
    
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    
    if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
        advance(lexer);
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
    }
    
    return make_token(lexer, TOKEN_NUMBER, start, lexer->current - start);
}

static TokenType check_keyword(const char* start, int length, const char* rest, TokenType type) {
    if (strncmp(start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENT;
}

static TokenType identifier_type(const char* start, int length) {
    switch (start[0]) {
        case 'e':
            if (length == 4) return check_keyword(start, 4, "else", TOKEN_ELSE);
            break;
        case 'f':
            if (length == 5) return check_keyword(start, 5, "false", TOKEN_FALSE);
            if (length == 2) return check_keyword(start, 2, "fn", TOKEN_FN);
            break;
        case 'i':
            if (length == 2) return check_keyword(start, 2, "if", TOKEN_IF);
            break;
        case 'l':
            if (length == 3) return check_keyword(start, 3, "let", TOKEN_LET);
            break;
        case 'r':
            if (length == 6) return check_keyword(start, 6, "return", TOKEN_RETURN);
            break;
        case 't':
            if (length == 4) return check_keyword(start, 4, "true", TOKEN_TRUE);
            break;
        case 'w':
            if (length == 5) return check_keyword(start, 5, "while", TOKEN_WHILE);
            break;
    }
    return TOKEN_IDENT;
}

static Token identifier(Lexer* lexer) {
    const char* start = lexer->current - 1;
    
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    
    int length = lexer->current - start;
    TokenType type = identifier_type(start, length);
    
    return make_token(lexer, type, start, length);
}

Token lexer_next(Lexer* lexer) {
    skip_whitespace(lexer);
    
    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF, lexer->current, 0);
    }
    
    const char* start = lexer->current;
    char c = advance(lexer);
    
    if (isdigit(c)) {
        return number(lexer);
    }
    
    if (isalpha(c) || c == '_') {
        return identifier(lexer);
    }
    
    switch (c) {
        case '\n':
            return make_token(lexer, TOKEN_NEWLINE, start, 1);
        case '+':
            return make_token(lexer, TOKEN_PLUS, start, 1);
        case '-':
            return make_token(lexer, TOKEN_MINUS, start, 1);
        case '*':
            return make_token(lexer, TOKEN_STAR, start, 1);
        case '/':
            return make_token(lexer, TOKEN_SLASH, start, 1);
        case '%':
            return make_token(lexer, TOKEN_PERCENT, start, 1);
        case '(':
            return make_token(lexer, TOKEN_LPAREN, start, 1);
        case ')':
            return make_token(lexer, TOKEN_RPAREN, start, 1);
        case '{':
            return make_token(lexer, TOKEN_LBRACE, start, 1);
        case '}':
            return make_token(lexer, TOKEN_RBRACE, start, 1);
        case ',':
            return make_token(lexer, TOKEN_COMMA, start, 1);
        case '=':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_EQ, start, 2);
            }
            return make_token(lexer, TOKEN_ASSIGN, start, 1);
        case '!':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_NEQ, start, 2);
            }
            break;
        case '<':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_LTE, start, 2);
            }
            return make_token(lexer, TOKEN_LT, start, 1);
        case '>':
            if (peek(lexer) == '=') {
                advance(lexer);
                return make_token(lexer, TOKEN_GTE, start, 2);
            }
            return make_token(lexer, TOKEN_GT, start, 1);
    }
    
    return error_token(lexer, "Unexpected character");
}
