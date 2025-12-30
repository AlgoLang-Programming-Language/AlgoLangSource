#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/algo_parser.h"

static void error_at(Parser* parser, Token* token, const char* message) {
    if (parser->panic_mode) return;
    parser->panic_mode = true;
    
    fprintf(stderr, "[line %d] Error", token->line);
    
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        
    } else {
        fprintf(stderr, " at '%.*s'", (int)token->length, token->start);
    }
    
    fprintf(stderr, ": %s\n", message);
    parser->had_error = true;
}

static void error(Parser* parser, const char* message) {
    error_at(parser, &parser->previous, message);
}

static void error_at_current(Parser* parser, const char* message) {
    error_at(parser, &parser->current, message);
}

static void advance(Parser* parser) {
    parser->previous = parser->current;
    
    while (true) {
        parser->current = lexer_next_token(&parser->lexer);
        if (parser->current.type != TOKEN_ERROR) break;
        
        error_at_current(parser, parser->current.start);
    }
}

static bool check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

static bool match(Parser* parser, TokenType type) {
    if (!check(parser, type)) return false;
    advance(parser);
    return true;
}

static void consume(Parser* parser, TokenType type, const char* message) {
    if (parser->current.type == type) {
        advance(parser);
        return;
    }
    
    error_at_current(parser, message);
}

static Expr* expression(Parser* parser);
static Stmt* declaration(Parser* parser);
static Stmt* statement(Parser* parser);

static Expr* primary(Parser* parser) {
    if (match(parser, TOKEN_TRUE)) {
        return new_literal_bool(true);
    }
    
    if (match(parser, TOKEN_FALSE)) {
        return new_literal_bool(false);
    }
    
    if (match(parser, TOKEN_NUMBER)) {
        char* end;
        double value = strtod(parser->previous.start, &end);
        return new_literal_number(value);
    }
    
    if (match(parser, TOKEN_IDENTIFIER)) {
        return new_variable(parser->previous);
    }
    
    if (match(parser, TOKEN_LPAREN)) {
        Expr* expr = expression(parser);
        consume(parser, TOKEN_RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    error(parser, "Expected expression");
    return new_literal_nil();
}

static Expr* finish_call(Parser* parser, Expr* callee) {
    Expr** arguments = NULL;
    size_t arg_count = 0;
    size_t arg_capacity = 0;
    
    if (!check(parser, TOKEN_RPAREN)) {
        do {
            if (arg_count >= arg_capacity) {
                size_t old_capacity = arg_capacity;
                arg_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
                arguments = realloc(arguments, arg_capacity * sizeof(Expr*));
            }
            arguments[arg_count++] = expression(parser);
        } while (match(parser, TOKEN_COMMA));
    }
    
    consume(parser, TOKEN_RPAREN, "Expected ')' after arguments");
    return new_call(callee, arguments, arg_count);
}

static Expr* call(Parser* parser) {
    Expr* expr = primary(parser);
    
    while (true) {
        if (match(parser, TOKEN_LPAREN)) {
            expr = finish_call(parser, expr);
        } else {
            break;
        }
    }
    
    return expr;
}

static Expr* unary(Parser* parser) {
    if (match(parser, TOKEN_BANG) || match(parser, TOKEN_MINUS)) {
        TokenType op = parser->previous.type;
        Expr* right = unary(parser);
        return new_unary(op, right);
    }
    
    return call(parser);
}

static Expr* factor(Parser* parser) {
    Expr* expr = unary(parser);
    
    while (match(parser, TOKEN_STAR) || match(parser, TOKEN_SLASH) || match(parser, TOKEN_PERCENT)) {
        TokenType op = parser->previous.type;
        Expr* right = unary(parser);
        expr = new_binary(op, expr, right);
    }
    
    return expr;
}

static Expr* term(Parser* parser) {
    Expr* expr = factor(parser);
    
    while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
        TokenType op = parser->previous.type;
        Expr* right = factor(parser);
        expr = new_binary(op, expr, right);
    }
    
    return expr;
}

static Expr* comparison(Parser* parser) {
    Expr* expr = term(parser);
    
    while (match(parser, TOKEN_GT) || match(parser, TOKEN_GT_EQ) ||
           match(parser, TOKEN_LT) || match(parser, TOKEN_LT_EQ)) {
        TokenType op = parser->previous.type;
        Expr* right = term(parser);
        expr = new_binary(op, expr, right);
    }
    
    return expr;
}

static Expr* equality(Parser* parser) {
    Expr* expr = comparison(parser);
    
    while (match(parser, TOKEN_EQ_EQ) || match(parser, TOKEN_BANG_EQ)) {
        TokenType op = parser->previous.type;
        Expr* right = comparison(parser);
        expr = new_binary(op, expr, right);
    }
    
    return expr;
}

static Expr* logical_and(Parser* parser) {
    Expr* expr = equality(parser);
    
    while (match(parser, TOKEN_AND)) {
        TokenType op = parser->previous.type;
        Expr* right = equality(parser);
        expr = new_logical(op, expr, right);
    }
    
    return expr;
}

static Expr* logical_or(Parser* parser) {
    Expr* expr = logical_and(parser);
    
    while (match(parser, TOKEN_OR)) {
        TokenType op = parser->previous.type;
        Expr* right = logical_and(parser);
        expr = new_logical(op, expr, right);
    }
    
    return expr;
}

static Expr* assignment(Parser* parser) {
    Expr* expr = logical_or(parser);
    
    if (match(parser, TOKEN_EQ)) {
        Expr* value = assignment(parser);
        
        if (expr->type == EXPR_VARIABLE) {
            Token name = expr->as.variable.name;
            free(expr);
            return new_assign(name, value);
        }
        
        error(parser, "Invalid assignment target");
    }
    
    return expr;
}

static Expr* expression(Parser* parser) {
    return assignment(parser);
}

static Stmt* block_statement(Parser* parser) {
    Stmt** statements = NULL;
    size_t count = 0;
    size_t capacity = 0;
    
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (count >= capacity) {
            size_t old_capacity = capacity;
            capacity = old_capacity < 8 ? 8 : old_capacity * 2;
            statements = realloc(statements, capacity * sizeof(Stmt*));
        }
        statements[count++] = declaration(parser);
    }
    
    consume(parser, TOKEN_RBRACE, "Expected '}' after block");
    return new_block_stmt(statements, count);
}

static Stmt* if_statement(Parser* parser) {
    Expr* condition = expression(parser);
    
    consume(parser, TOKEN_LBRACE, "Expected '{' after if condition");
    Stmt* then_branch = block_statement(parser);
    
    Stmt* else_branch = NULL;
    if (match(parser, TOKEN_ELSE)) {
        if (match(parser, TOKEN_IF)) {
            else_branch = if_statement(parser);
        } else {
            consume(parser, TOKEN_LBRACE, "Expected '{' after else");
            else_branch = block_statement(parser);
        }
    }
    
    return new_if_stmt(condition, then_branch, else_branch);
}

static Stmt* while_statement(Parser* parser) {
    Expr* condition = expression(parser);
    
    consume(parser, TOKEN_LBRACE, "Expected '{' after while condition");
    Stmt* body = block_statement(parser);
    
    return new_while_stmt(condition, body);
}

static Stmt* return_statement(Parser* parser) {
    Expr* value = NULL;
    
    if (!check(parser, TOKEN_SEMICOLON) && !check(parser, TOKEN_RBRACE)) {
        value = expression(parser);
    }
    
    return new_return_stmt(value);
}

static Stmt* print_statement(Parser* parser) {
    Expr* value = expression(parser);
    return new_print_stmt(value);
}

static Stmt* expression_statement(Parser* parser) {
    Expr* expr = expression(parser);
    return new_expr_stmt(expr);
}

static Stmt* statement(Parser* parser) {
    if (match(parser, TOKEN_IF)) {
        return if_statement(parser);
    }
    
    if (match(parser, TOKEN_WHILE)) {
        return while_statement(parser);
    }
    
    if (match(parser, TOKEN_RETURN)) {
        return return_statement(parser);
    }
    
    if (match(parser, TOKEN_PRINT)) {
        return print_statement(parser);
    }
    
    if (match(parser, TOKEN_LBRACE)) {
        return block_statement(parser);
    }
    
    return expression_statement(parser);
}

static Stmt* let_declaration(Parser* parser) {
    consume(parser, TOKEN_IDENTIFIER, "Expected variable name");
    Token name = parser->previous;
    
    Expr* initializer = NULL;
    if (match(parser, TOKEN_EQ)) {
        initializer = expression(parser);
    }
    
    return new_let_stmt(name, initializer);
}

static Stmt* function_declaration(Parser* parser) {
    consume(parser, TOKEN_IDENTIFIER, "Expected function name");
    Token name = parser->previous;
    
    consume(parser, TOKEN_LPAREN, "Expected '(' after function name");
    
    Token* params = NULL;
    size_t param_count = 0;
    size_t param_capacity = 0;
    
    if (!check(parser, TOKEN_RPAREN)) {
        do {
            if (param_count >= param_capacity) {
                size_t old_capacity = param_capacity;
                param_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
                params = realloc(params, param_capacity * sizeof(Token));
            }
            
            consume(parser, TOKEN_IDENTIFIER, "Expected parameter name");
            params[param_count++] = parser->previous;
        } while (match(parser, TOKEN_COMMA));
    }
    
    consume(parser, TOKEN_RPAREN, "Expected ')' after parameters");
    consume(parser, TOKEN_LBRACE, "Expected '{' before function body");
    
    Stmt** body = NULL;
    size_t body_count = 0;
    size_t body_capacity = 0;
    
    while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
        if (body_count >= body_capacity) {
            size_t old_capacity = body_capacity;
            body_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
            body = realloc(body, body_capacity * sizeof(Stmt*));
        }
        body[body_count++] = declaration(parser);
    }
    
    consume(parser, TOKEN_RBRACE, "Expected '}' after function body");
    
    return new_function_stmt(name, params, param_count, body, body_count);
}

static Stmt* declaration(Parser* parser) {
    if (match(parser, TOKEN_LET)) {
        return let_declaration(parser);
    }
    
    if (match(parser, TOKEN_FN)) {
        return function_declaration(parser);
    }
    
    return statement(parser);
}

void parser_init(Parser* parser, const char* source) {
    lexer_init(&parser->lexer, source);
    parser->had_error = false;
    parser->panic_mode = false;
    advance(parser);
}

Program* parse(Parser* parser) {
    Program* program = malloc(sizeof(Program));
    program->statements = NULL;
    program->count = 0;
    program->capacity = 0;
    
    while (!match(parser, TOKEN_EOF)) {
        if (program->count >= program->capacity) {
            size_t old_capacity = program->capacity;
            program->capacity = old_capacity < 8 ? 8 : old_capacity * 2;
            program->statements = realloc(program->statements, program->capacity * sizeof(Stmt*));
        }
        
        program->statements[program->count++] = declaration(parser);
        
        if (parser->panic_mode) {
            while (parser->current.type != TOKEN_EOF) {
                if (parser->previous.type == TOKEN_SEMICOLON) break;
                if (parser->current.type == TOKEN_LET) break;
                if (parser->current.type == TOKEN_FN) break;
                if (parser->current.type == TOKEN_IF) break;
                if (parser->current.type == TOKEN_WHILE) break;
                if (parser->current.type == TOKEN_RETURN) break;
                advance(parser);
            }
            parser->panic_mode = false;
        }
    }
    
    return program;
}
