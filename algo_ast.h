#ifndef ALGO_AST_H
#define ALGO_AST_H

typedef enum {
    EXPR_NUMBER,
    EXPR_BOOL,
    EXPR_IDENT,
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_CALL,
    EXPR_ASSIGN,
} ExprType;

typedef enum {
    STMT_EXPR,
    STMT_LET,
    STMT_BLOCK,
    STMT_IF,
    STMT_WHILE,
    STMT_RETURN,
    STMT_FUNCTION,
} StmtType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_LTE,
    OP_GT,
    OP_GTE,
    OP_NEG,
} Operator;

typedef struct Expr Expr;
typedef struct Stmt Stmt;

struct Expr {
    ExprType type;
    union {
        double number;
        int boolean;
        char* ident;
        struct {
            Operator op;
            Expr* left;
            Expr* right;
        } binary;
        struct {
            Operator op;
            Expr* operand;
        } unary;
        struct {
            Expr* callee;
            Expr** args;
            int arg_count;
        } call;
        struct {
            char* name;
            Expr* value;
        } assign;
    };
};

struct Stmt {
    StmtType type;
    union {
        struct {
            Expr* expr;
        } expr_stmt;
        struct {
            char* name;
            Expr* init;
        } let;
        struct {
            Stmt** stmts;
            int count;
        } block;
        struct {
            Expr* condition;
            Stmt* then_branch;
            Stmt* else_branch;
        } if_stmt;
        struct {
            Expr* condition;
            Stmt* body;
        } while_stmt;
        struct {
            Expr* value;
        } return_stmt;
        struct {
            char* name;
            char** params;
            int param_count;
            Stmt* body;
        } function;
    };
};

typedef struct {
    Stmt** stmts;
    int count;
} Program;

typedef struct ArenaBlock ArenaBlock;

struct ArenaBlock {
    char* memory;
    size_t used;
    size_t size;
    ArenaBlock* next;
};

typedef struct {
    ArenaBlock* current;
} Arena;

void arena_init(Arena* arena);
void arena_free(Arena* arena);
void* arena_alloc(Arena* arena, size_t size);

#endif
