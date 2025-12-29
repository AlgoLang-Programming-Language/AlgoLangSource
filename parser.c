#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/algo_token.h"
#include "../include/algo_ast.h"

#define ARENA_BLOCK_SIZE 8192

void arena_init(Arena* arena) {
    arena->current = malloc(sizeof(ArenaBlock));
    arena->current->memory = malloc(ARENA_BLOCK_SIZE);
    arena->current->used = 0;
    arena->current->size = ARENA_BLOCK_SIZE;
    arena->current->next = NULL;
}

void arena_free(Arena* arena) {
    ArenaBlock* block = arena->current;
    while (block) {
        ArenaBlock* next = block->next;
        free(block->memory);
        free(block);
        block = next;
    }
}

void* arena_alloc(Arena* arena, size_t size) {
    if (arena->current->used + size > arena->current->size) {
        ArenaBlock* new_block = malloc(sizeof(ArenaBlock));
        new_block->memory = malloc(ARENA_BLOCK_SIZE);
        new_block->used = 0;
        new_block->size = ARENA_BLOCK_SIZE;
        new_block->next = arena->current;
        arena->current = new_block;
    }
    
    void* ptr = arena->current->memory + arena->current->used;
    arena->current->used += size;
    return ptr;
}

typedef struct {
    Lexer* lexer;
    Token current;
    Token previous;
    Arena* arena;
    int had_error;
} Parser;

static void advance(Parser* p) {
    p->previous = p->current;
    
    while (1) {
        p->current = lexer_next(p->lexer);
        if (p->current.type == TOKEN_NEWLINE) continue;
        if (p->current.type != TOKEN_ERROR) break;
        
        fprintf(stderr, "Lexer error: %.*s\n", p->current.length, p->current.start);
        p->had_error = 1;
    }
}

static int check(Parser* p, TokenType type) {
    return p->current.type == type;
}

static int match(Parser* p, TokenType type) {
    if (!check(p, type)) return 0;
    advance(p);
    return 1;
}

static void consume(Parser* p, TokenType type, const char* message) {
    if (p->current.type == type) {
        advance(p);
        return;
    }
    
    fprintf(stderr, "Parse error: %s\n", message);
    p->had_error = 1;
}

static char* copy_string(Parser* p, const char* start, int length) {
    char* str = arena_alloc(p->arena, length + 1);
    memcpy(str, start, length);
    str[length] = '\0';
    return str;
}

static Expr* expr(Parser* p);
static Stmt* stmt(Parser* p);

static Expr* primary(Parser* p) {
    if (match(p, TOKEN_NUMBER)) {
        Expr* e = arena_alloc(p->arena, sizeof(Expr));
        e->type = EXPR_NUMBER;
        e->number = strtod(p->previous.start, NULL);
        return e;
    }
    
    if (match(p, TOKEN_TRUE)) {
        Expr* e = arena_alloc(p->arena, sizeof(Expr));
        e->type = EXPR_BOOL;
        e->boolean = 1;
        return e;
    }
    
    if (match(p, TOKEN_FALSE)) {
        Expr* e = arena_alloc(p->arena, sizeof(Expr));
        e->type = EXPR_BOOL;
        e->boolean = 0;
        return e;
    }
    
    if (match(p, TOKEN_IDENT)) {
        Expr* e = arena_alloc(p->arena, sizeof(Expr));
        e->type = EXPR_IDENT;
        e->ident = copy_string(p, p->previous.start, p->previous.length);
        return e;
    }
    
    if (match(p, TOKEN_LPAREN)) {
        Expr* e = expr(p);
        consume(p, TOKEN_RPAREN, "Expected ')' after expression");
        return e;
    }
    
    fprintf(stderr, "Parse error: Expected expression\n");
    p->had_error = 1;
    return NULL;
}

static Expr* call(Parser* p) {
    Expr* e = primary(p);
    
    while (match(p, TOKEN_LPAREN)) {
        Expr* call_expr = arena_alloc(p->arena, sizeof(Expr));
        call_expr->type = EXPR_CALL;
        call_expr->call.callee = e;
        
        int capacity = 8;
        Expr** args = arena_alloc(p->arena, sizeof(Expr*) * capacity);
        int count = 0;
        
        if (!check(p, TOKEN_RPAREN)) {
            do {
                if (count >= capacity) {
                    capacity *= 2;
                    Expr** new_args = arena_alloc(p->arena, sizeof(Expr*) * capacity);
                    memcpy(new_args, args, sizeof(Expr*) * count);
                    args = new_args;
                }
                args[count++] = expr(p);
            } while (match(p, TOKEN_COMMA));
        }
        
        consume(p, TOKEN_RPAREN, "Expected ')' after arguments");
        
        call_expr->call.args = args;
        call_expr->call.arg_count = count;
        e = call_expr;
    }
    
    return e;
}

static Expr* unary(Parser* p) {
    if (match(p, TOKEN_MINUS)) {
        Expr* e = arena_alloc(p->arena, sizeof(Expr));
        e->type = EXPR_UNARY;
        e->unary.op = OP_NEG;
        e->unary.operand = unary(p);
        return e;
    }
    
    return call(p);
}

static Expr* factor(Parser* p) {
    Expr* e = unary(p);
    
    while (match(p, TOKEN_STAR) || match(p, TOKEN_SLASH) || match(p, TOKEN_PERCENT)) {
        Operator op = p->previous.type == TOKEN_STAR ? OP_MUL :
                     p->previous.type == TOKEN_SLASH ? OP_DIV : OP_MOD;
        
        Expr* binary = arena_alloc(p->arena, sizeof(Expr));
        binary->type = EXPR_BINARY;
        binary->binary.op = op;
        binary->binary.left = e;
        binary->binary.right = unary(p);
        e = binary;
    }
    
    return e;
}

static Expr* term(Parser* p) {
    Expr* e = factor(p);
    
    while (match(p, TOKEN_PLUS) || match(p, TOKEN_MINUS)) {
        Operator op = p->previous.type == TOKEN_PLUS ? OP_ADD : OP_SUB;
        
        Expr* binary = arena_alloc(p->arena, sizeof(Expr));
        binary->type = EXPR_BINARY;
        binary->binary.op = op;
        binary->binary.left = e;
        binary->binary.right = factor(p);
        e = binary;
    }
    
    return e;
}

static Expr* comparison(Parser* p) {
    Expr* e = term(p);
    
    while (match(p, TOKEN_LT) || match(p, TOKEN_LTE) || 
           match(p, TOKEN_GT) || match(p, TOKEN_GTE)) {
        Operator op = p->previous.type == TOKEN_LT ? OP_LT :
                     p->previous.type == TOKEN_LTE ? OP_LTE :
                     p->previous.type == TOKEN_GT ? OP_GT : OP_GTE;
        
        Expr* binary = arena_alloc(p->arena, sizeof(Expr));
        binary->type = EXPR_BINARY;
        binary->binary.op = op;
        binary->binary.left = e;
        binary->binary.right = term(p);
        e = binary;
    }
    
    return e;
}

static Expr* equality(Parser* p) {
    Expr* e = comparison(p);
    
    while (match(p, TOKEN_EQ) || match(p, TOKEN_NEQ)) {
        Operator op = p->previous.type == TOKEN_EQ ? OP_EQ : OP_NEQ;
        
        Expr* binary = arena_alloc(p->arena, sizeof(Expr));
        binary->type = EXPR_BINARY;
        binary->binary.op = op;
        binary->binary.left = e;
        binary->binary.right = comparison(p);
        e = binary;
    }
    
    return e;
}

static Expr* assignment(Parser* p) {
    Expr* e = equality(p);
    
    if (match(p, TOKEN_ASSIGN)) {
        if (e->type != EXPR_IDENT) {
            fprintf(stderr, "Parse error: Invalid assignment target\n");
            p->had_error = 1;
            return e;
        }
        
        Expr* assign = arena_alloc(p->arena, sizeof(Expr));
        assign->type = EXPR_ASSIGN;
        assign->assign.name = e->ident;
        assign->assign.value = assignment(p);
        return assign;
    }
    
    return e;
}

static Expr* expr(Parser* p) {
    return assignment(p);
}

static Stmt* block(Parser* p) {
    Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
    s->type = STMT_BLOCK;
    
    int capacity = 8;
    Stmt** stmts = arena_alloc(p->arena, sizeof(Stmt*) * capacity);
    int count = 0;
    
    while (!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
        if (count >= capacity) {
            capacity *= 2;
            Stmt** new_stmts = arena_alloc(p->arena, sizeof(Stmt*) * capacity);
            memcpy(new_stmts, stmts, sizeof(Stmt*) * count);
            stmts = new_stmts;
        }
        stmts[count++] = stmt(p);
    }
    
    consume(p, TOKEN_RBRACE, "Expected '}' after block");
    
    s->block.stmts = stmts;
    s->block.count = count;
    return s;
}

static Stmt* stmt(Parser* p) {
    if (match(p, TOKEN_LET)) {
        Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
        s->type = STMT_LET;
        
        consume(p, TOKEN_IDENT, "Expected variable name");
        s->let.name = copy_string(p, p->previous.start, p->previous.length);
        
        consume(p, TOKEN_ASSIGN, "Expected '=' after variable name");
        s->let.init = expr(p);
        
        return s;
    }
    
    if (match(p, TOKEN_IF)) {
        Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
        s->type = STMT_IF;
        
        s->if_stmt.condition = expr(p);
        consume(p, TOKEN_LBRACE, "Expected '{' after if condition");
        s->if_stmt.then_branch = block(p);
        
        if (match(p, TOKEN_ELSE)) {
            consume(p, TOKEN_LBRACE, "Expected '{' after else");
            s->if_stmt.else_branch = block(p);
        } else {
            s->if_stmt.else_branch = NULL;
        }
        
        return s;
    }
    
    if (match(p, TOKEN_WHILE)) {
        Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
        s->type = STMT_WHILE;
        
        s->while_stmt.condition = expr(p);
        consume(p, TOKEN_LBRACE, "Expected '{' after while condition");
        s->while_stmt.body = block(p);
        
        return s;
    }
    
    if (match(p, TOKEN_RETURN)) {
        Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
        s->type = STMT_RETURN;
        
        if (!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
            s->return_stmt.value = expr(p);
        } else {
            s->return_stmt.value = NULL;
        }
        
        return s;
    }
    
    if (match(p, TOKEN_FN)) {
        Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
        s->type = STMT_FUNCTION;
        
        consume(p, TOKEN_IDENT, "Expected function name");
        s->function.name = copy_string(p, p->previous.start, p->previous.length);
        
        consume(p, TOKEN_LPAREN, "Expected '(' after function name");
        
        int capacity = 8;
        char** params = arena_alloc(p->arena, sizeof(char*) * capacity);
        int count = 0;
        
        if (!check(p, TOKEN_RPAREN)) {
            do {
                if (count >= capacity) {
                    capacity *= 2;
                    char** new_params = arena_alloc(p->arena, sizeof(char*) * capacity);
                    memcpy(new_params, params, sizeof(char*) * count);
                    params = new_params;
                }
                consume(p, TOKEN_IDENT, "Expected parameter name");
                params[count++] = copy_string(p, p->previous.start, p->previous.length);
            } while (match(p, TOKEN_COMMA));
        }
        
        consume(p, TOKEN_RPAREN, "Expected ')' after parameters");
        consume(p, TOKEN_LBRACE, "Expected '{' before function body");
        
        s->function.params = params;
        s->function.param_count = count;
        s->function.body = block(p);
        
        return s;
    }
    
    Stmt* s = arena_alloc(p->arena, sizeof(Stmt));
    s->type = STMT_EXPR;
    s->expr_stmt.expr = expr(p);
    return s;
}

Program parse(const char* source, Arena* arena) {
    Lexer lexer;
    lexer_init(&lexer, source);
    
    Parser parser;
    parser.lexer = &lexer;
    parser.arena = arena;
    parser.had_error = 0;
    
    advance(&parser);
    
    int capacity = 16;
    Stmt** stmts = arena_alloc(arena, sizeof(Stmt*) * capacity);
    int count = 0;
    
    while (!match(&parser, TOKEN_EOF)) {
        if (count >= capacity) {
            capacity *= 2;
            Stmt** new_stmts = arena_alloc(arena, sizeof(Stmt*) * capacity);
            memcpy(new_stmts, stmts, sizeof(Stmt*) * count);
            stmts = new_stmts;
        }
        stmts[count++] = stmt(&parser);
    }
    
    Program program;
    program.stmts = stmts;
    program.count = parser.had_error ? -1 : count;
    
    return program;
}
