#include <stdlib.h>
#include "../../include/algo_ast.h"

Expr* new_literal_number(double value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;
    expr->as.literal.type = LITERAL_NUMBER;
    expr->as.literal.as.number.value = value;
    return expr;
}

Expr* new_literal_bool(bool value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;
    expr->as.literal.type = LITERAL_BOOL;
    expr->as.literal.as.boolean.value = value;
    return expr;
}

Expr* new_literal_nil() {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;
    expr->as.literal.type = LITERAL_NIL;
    return expr;
}

Expr* new_unary(TokenType op, Expr* operand) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_UNARY;
    expr->as.unary.op = op;
    expr->as.unary.operand = operand;
    return expr;
}

Expr* new_binary(TokenType op, Expr* left, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_BINARY;
    expr->as.binary.op = op;
    expr->as.binary.left = left;
    expr->as.binary.right = right;
    return expr;
}

Expr* new_variable(Token name) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable.name = name;
    return expr;
}

Expr* new_assign(Token name, Expr* value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_ASSIGN;
    expr->as.assign.name = name;
    expr->as.assign.value = value;
    return expr;
}

Expr* new_call(Expr* callee, Expr** arguments, size_t arg_count) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_CALL;
    expr->as.call.callee = callee;
    expr->as.call.arguments = arguments;
    expr->as.call.arg_count = arg_count;
    return expr;
}

Expr* new_logical(TokenType op, Expr* left, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LOGICAL;
    expr->as.logical.op = op;
    expr->as.logical.left = left;
    expr->as.logical.right = right;
    return expr;
}

Stmt* new_expr_stmt(Expr* expression) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_EXPR;
    stmt->as.expr_stmt.expression = expression;
    return stmt;
}

Stmt* new_let_stmt(Token name, Expr* initializer) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_LET;
    stmt->as.let_stmt.name = name;
    stmt->as.let_stmt.initializer = initializer;
    return stmt;
}

Stmt* new_block_stmt(Stmt** statements, size_t count) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_BLOCK;
    stmt->as.block.statements = statements;
    stmt->as.block.count = count;
    return stmt;
}

Stmt* new_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_IF;
    stmt->as.if_stmt.condition = condition;
    stmt->as.if_stmt.then_branch = then_branch;
    stmt->as.if_stmt.else_branch = else_branch;
    return stmt;
}

Stmt* new_while_stmt(Expr* condition, Stmt* body) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_WHILE;
    stmt->as.while_stmt.condition = condition;
    stmt->as.while_stmt.body = body;
    return stmt;
}

Stmt* new_function_stmt(Token name, Token* params, size_t param_count, Stmt** body, size_t body_count) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_FUNCTION;
    stmt->as.function.name = name;
    stmt->as.function.params = params;
    stmt->as.function.param_count = param_count;
    stmt->as.function.body = body;
    stmt->as.function.body_count = body_count;
    return stmt;
}

Stmt* new_return_stmt(Expr* value) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_RETURN;
    stmt->as.return_stmt.value = value;
    return stmt;
}

Stmt* new_print_stmt(Expr* expression) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_PRINT;
    stmt->as.print_stmt.expression = expression;
    return stmt;
}

void free_expr(Expr* expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case EXPR_LITERAL:
            break;
        case EXPR_UNARY:
            free_expr(expr->as.unary.operand);
            break;
        case EXPR_BINARY:
            free_expr(expr->as.binary.left);
            free_expr(expr->as.binary.right);
            break;
        case EXPR_VARIABLE:
            break;
        case EXPR_ASSIGN:
            free_expr(expr->as.assign.value);
            break;
        case EXPR_CALL:
            free_expr(expr->as.call.callee);
            for (size_t i = 0; i < expr->as.call.arg_count; i++) {
                free_expr(expr->as.call.arguments[i]);
            }
            free(expr->as.call.arguments);
            break;
        case EXPR_LOGICAL:
            free_expr(expr->as.logical.left);
            free_expr(expr->as.logical.right);
            break;
    }
    free(expr);
}

void free_stmt(Stmt* stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_EXPR:
            free_expr(stmt->as.expr_stmt.expression);
            break;
        case STMT_LET:
            free_expr(stmt->as.let_stmt.initializer);
            break;
        case STMT_BLOCK:
            for (size_t i = 0; i < stmt->as.block.count; i++) {
                free_stmt(stmt->as.block.statements[i]);
            }
            free(stmt->as.block.statements);
            break;
        case STMT_IF:
            free_expr(stmt->as.if_stmt.condition);
            free_stmt(stmt->as.if_stmt.then_branch);
            free_stmt(stmt->as.if_stmt.else_branch);
            break;
        case STMT_WHILE:
            free_expr(stmt->as.while_stmt.condition);
            free_stmt(stmt->as.while_stmt.body);
            break;
        case STMT_FUNCTION:
            free(stmt->as.function.params);
            for (size_t i = 0; i < stmt->as.function.body_count; i++) {
                free_stmt(stmt->as.function.body[i]);
            }
            free(stmt->as.function.body);
            break;
        case STMT_RETURN:
            free_expr(stmt->as.return_stmt.value);
            break;
        case STMT_PRINT:
            free_expr(stmt->as.print_stmt.expression);
            break;
    }
    free(stmt);
}

void free_program(Program* program) {
    for (size_t i = 0; i < program->count; i++) {
        free_stmt(program->statements[i]);
    }
    free(program->statements);
}
