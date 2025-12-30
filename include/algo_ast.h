#ifndef ALGO_AST_H
#define ALGO_AST_H

#include "algo_common.h"
#include "algo_token.h"

typedef enum {
    EXPR_LITERAL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_VARIABLE,
    EXPR_ASSIGN,
    EXPR_CALL,
    EXPR_LOGICAL
} ExprType;

typedef struct Expr Expr;

typedef struct {
    double value;
} LiteralNumber;

typedef struct {
    bool value;
} LiteralBool;

typedef enum {
    LITERAL_NUMBER,
    LITERAL_BOOL,
    LITERAL_NIL
} LiteralType;

typedef struct {
    LiteralType type;
    union {
        LiteralNumber number;
        LiteralBool boolean;
    } as;
} LiteralExpr;

typedef struct {
    TokenType op;
    Expr* operand;
} UnaryExpr;

typedef struct {
    TokenType op;
    Expr* left;
    Expr* right;
} BinaryExpr;

typedef struct {
    Token name;
} VariableExpr;

typedef struct {
    Token name;
    Expr* value;
} AssignExpr;

typedef struct {
    Expr* callee;
    Expr** arguments;
    size_t arg_count;
} CallExpr;

typedef struct {
    TokenType op;
    Expr* left;
    Expr* right;
} LogicalExpr;

struct Expr {
    ExprType type;
    union {
        LiteralExpr literal;
        UnaryExpr unary;
        BinaryExpr binary;
        VariableExpr variable;
        AssignExpr assign;
        CallExpr call;
        LogicalExpr logical;
    } as;
};

typedef enum {
    STMT_EXPR,
    STMT_LET,
    STMT_BLOCK,
    STMT_IF,
    STMT_WHILE,
    STMT_FUNCTION,
    STMT_RETURN,
    STMT_PRINT
} StmtType;

typedef struct Stmt Stmt;

typedef struct {
    Expr* expression;
} ExprStmt;

typedef struct {
    Token name;
    Expr* initializer;
} LetStmt;

typedef struct {
    Stmt** statements;
    size_t count;
} BlockStmt;

typedef struct {
    Expr* condition;
    Stmt* then_branch;
    Stmt* else_branch;
} IfStmt;

typedef struct {
    Expr* condition;
    Stmt* body;
} WhileStmt;

typedef struct {
    Token name;
    Token* params;
    size_t param_count;
    Stmt** body;
    size_t body_count;
} FunctionStmt;

typedef struct {
    Expr* value;
} ReturnStmt;

typedef struct {
    Expr* expression;
} PrintStmt;

struct Stmt {
    StmtType type;
    union {
        ExprStmt expr_stmt;
        LetStmt let_stmt;
        BlockStmt block;
        IfStmt if_stmt;
        WhileStmt while_stmt;
        FunctionStmt function;
        ReturnStmt return_stmt;
        PrintStmt print_stmt;
    } as;
};

typedef struct {
    Stmt** statements;
    size_t count;
    size_t capacity;
} Program;

Expr* new_literal_number(double value);
Expr* new_literal_bool(bool value);
Expr* new_literal_nil();
Expr* new_unary(TokenType op, Expr* operand);
Expr* new_binary(TokenType op, Expr* left, Expr* right);
Expr* new_variable(Token name);
Expr* new_assign(Token name, Expr* value);
Expr* new_call(Expr* callee, Expr** arguments, size_t arg_count);
Expr* new_logical(TokenType op, Expr* left, Expr* right);

Stmt* new_expr_stmt(Expr* expression);
Stmt* new_let_stmt(Token name, Expr* initializer);
Stmt* new_block_stmt(Stmt** statements, size_t count);
Stmt* new_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch);
Stmt* new_while_stmt(Expr* condition, Stmt* body);
Stmt* new_function_stmt(Token name, Token* params, size_t param_count, Stmt** body, size_t body_count);
Stmt* new_return_stmt(Expr* value);
Stmt* new_print_stmt(Expr* expression);

void free_expr(Expr* expr);
void free_stmt(Stmt* stmt);
void free_program(Program* program);

#endif
