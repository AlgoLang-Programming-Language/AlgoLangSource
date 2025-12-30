#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../../include/algo_token.h"

void lexer_init(Lexer* lexer, const char* source) {
    lexer->source = source;
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
}

static bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer* lexer) {
    lexer->column++;
    return *lexer->current++;
}

static char peek(Lexer* lexer) {
    return *lexer->current;
}

static char peek_next(Lexer* lexer) {
    if (is_at_end(lexer)) return '\0';
    return lexer->current[1];
}

static bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) return false;
    if (*lexer->current != expected) return false;
    lexer->current++;
    lexer->column++;
    return true;
}

static Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (size_t)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;
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
    while (true) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                lexer->column = 0;
                advance(lexer);
                break;
            case '#':
                while (peek(lexer) != '\n' && !is_at_end(lexer)) {
                    advance(lexer);
                }
                break;
            default:
                return;
        }
    }
}

static TokenType check_keyword(Lexer* lexer, int start, int length, const char* rest, TokenType type) {
    if (lexer->current - lexer->start == start + length &&
        memcmp(lexer->start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* lexer) {
    switch (lexer->start[0]) {
        case 'a': return check_keyword(lexer, 1, 2, "nd", TOKEN_AND);
        case 'b': return check_keyword(lexer, 1, 4, "reak", TOKEN_BREAK);
        case 'c': return check_keyword(lexer, 1, 7, "ontinue", TOKEN_CONTINUE);
        case 'e': return check_keyword(lexer, 1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a': return check_keyword(lexer, 2, 3, "lse", TOKEN_FALSE);
                    case 'n': return check_keyword(lexer, 2, 0, "", TOKEN_FN);
                    case 'o': return check_keyword(lexer, 2, 1, "r", TOKEN_FOR);
                }
            }
            break;
        case 'i': return check_keyword(lexer, 1, 1, "f", TOKEN_IF);
        case 'l': return check_keyword(lexer, 1, 2, "et", TOKEN_LET);
        case 'o': return check_keyword(lexer, 1, 1, "r", TOKEN_OR);
        case 'p': return check_keyword(lexer, 1, 4, "rint", TOKEN_PRINT);
        case 'r': return check_keyword(lexer, 1, 5, "eturn", TOKEN_RETURN);
        case 't': return check_keyword(lexer, 1, 3, "rue", TOKEN_TRUE);
        case 'w': return check_keyword(lexer, 1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer* lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    return make_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    
    if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
        advance(lexer);
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
    }
    
    return make_token(lexer, TOKEN_NUMBER);
}

static Token string(Lexer* lexer) {
    while (peek(lexer) != '"' && !is_at_end(lexer)) {
        if (peek(lexer) == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
        advance(lexer);
    }
    
    if (is_at_end(lexer)) {
        return error_token(lexer, "Unterminated string");
    }
    
    advance(lexer);
    return make_token(lexer, TOKEN_STRING);
}

Token lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    lexer->start = lexer->current;
    
    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }
    
    char c = advance(lexer);
    
    if (isalpha(c) || c == '_') {
        return identifier(lexer);
    }
    
    if (isdigit(c)) {
        return number(lexer);
    }
    
    if (c == '"') {
        return string(lexer);
    }
    
    switch (c) {
        case '(': return make_token(lexer, TOKEN_LPAREN);
        case ')': return make_token(lexer, TOKEN_RPAREN);
        case '{': return make_token(lexer, TOKEN_LBRACE);
        case '}': return make_token(lexer, TOKEN_RBRACE);
        case '[': return make_token(lexer, TOKEN_LBRACKET);
        case ']': return make_token(lexer, TOKEN_RBRACKET);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case ';': return make_token(lexer, TOKEN_SEMICOLON);
        case '.': return make_token(lexer, TOKEN_DOT);
        case '%': return make_token(lexer, TOKEN_PERCENT);
        case '+':
            if (match(lexer, '+')) return make_token(lexer, TOKEN_PLUS_PLUS);
            if (match(lexer, '=')) return make_token(lexer, TOKEN_PLUS_EQ);
            return make_token(lexer, TOKEN_PLUS);
        case '-':
            if (match(lexer, '-')) return make_token(lexer, TOKEN_MINUS_MINUS);
            if (match(lexer, '=')) return make_token(lexer, TOKEN_MINUS_EQ);
            return make_token(lexer, TOKEN_MINUS);
        case '*':
            return make_token(lexer, match(lexer, '=') ? TOKEN_STAR_EQ : TOKEN_STAR);
        case '/':
            return make_token(lexer, match(lexer, '=') ? TOKEN_SLASH_EQ : TOKEN_SLASH);
        case '!':
            return make_token(lexer, match(lexer, '=') ? TOKEN_BANG_EQ : TOKEN_BANG);
        case '=':
            return make_token(lexer, match(lexer, '=') ? TOKEN_EQ_EQ : TOKEN_EQ);
        case '<':
            return make_token(lexer, match(lexer, '=') ? TOKEN_LT_EQ : TOKEN_LT);
        case '>':
            return make_token(lexer, match(lexer, '=') ? TOKEN_GT_EQ : TOKEN_GT);
    }
    
    return error_token(lexer, "Unexpected character");
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_LET: return "LET";
        case TOKEN_FN: return "FN";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PERCENT: return "PERCENT";
        case TOKEN_PLUS_PLUS: return "PLUS_PLUS";
        case TOKEN_MINUS_MINUS: return "MINUS_MINUS";
        case TOKEN_PLUS_EQ: return "PLUS_EQ";
        case TOKEN_MINUS_EQ: return "MINUS_EQ";
        case TOKEN_STAR_EQ: return "STAR_EQ";
        case TOKEN_SLASH_EQ: return "SLASH_EQ";
        case TOKEN_EQ: return "EQ";
        case TOKEN_EQ_EQ: return "EQ_EQ";
        case TOKEN_BANG: return "BANG";
        case TOKEN_BANG_EQ: return "BANG_EQ";
        case TOKEN_LT: return "LT";
        case TOKEN_LT_EQ: return "LT_EQ";
        case TOKEN_GT: return "GT";
        case TOKEN_GT_EQ: return "GT_EQ";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        default: return "UNKNOWN";
    }
}
