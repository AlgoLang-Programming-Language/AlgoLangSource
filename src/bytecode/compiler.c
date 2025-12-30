#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/algo_compiler.h"
#include "../../include/algo_parser.h"
#include "../../include/algo_bytecode.h"

typedef struct {
    Parser parser;
    Compiler* current;
    bool had_error;
} CompilerState;

static CompilerState state;

static Chunk* current_chunk() {
    return &state.current->function->chunk;
}

static void emit_byte(uint8_t byte) {
    write_chunk(current_chunk(), byte, 0);
}

static void emit_bytes(uint8_t byte1, uint8_t byte2) {
    emit_byte(byte1);
    emit_byte(byte2);
}

static void emit_return() {
    emit_byte(OP_NIL);
    emit_byte(OP_RETURN);
}

static uint8_t make_constant(Value value) {
    int constant = add_constant(current_chunk(), value);
    if (constant > 255) {
        fprintf(stderr, "Too many constants in one chunk\n");
        return 0;
    }
    return (uint8_t)constant;
}

static void emit_constant(Value value) {
    emit_bytes(OP_CONSTANT, make_constant(value));
}

static int emit_jump(uint8_t instruction) {
    emit_byte(instruction);
    emit_byte(0xff);
    emit_byte(0xff);
    return current_chunk()->count - 2;
}

static void patch_jump(int offset) {
    int jump = current_chunk()->count - offset - 2;
    
    if (jump > 65535) {
        fprintf(stderr, "Too much code to jump over\n");
    }
    
    current_chunk()->code[offset] = (jump >> 8) & 0xff;
    current_chunk()->code[offset + 1] = jump & 0xff;
}

static void emit_loop(int loop_start) {
    emit_byte(OP_LOOP);
    
    int offset = current_chunk()->count - loop_start + 2;
    if (offset > 65535) {
        fprintf(stderr, "Loop body too large\n");
    }
    
    emit_byte((offset >> 8) & 0xff);
    emit_byte(offset & 0xff);
}

static void init_compiler(Compiler* compiler, FunctionType type) {
    compiler->enclosing = state.current;
    compiler->function = NULL;
    compiler->type = type;
    compiler->local_count = 0;
    compiler->scope_depth = 0;
    compiler->function = new_function();
    state.current = compiler;
}

static ObjFunction* end_compiler() {
    emit_return();
    ObjFunction* function = state.current->function;
    state.current = state.current->enclosing;
    return function;
}

static void begin_scope() {
    state.current->scope_depth++;
}

static void end_scope() {
    state.current->scope_depth--;
    
    while (state.current->local_count > 0 &&
           state.current->locals[state.current->local_count - 1].depth > state.current->scope_depth) {
        emit_byte(OP_POP);
        state.current->local_count--;
    }
}

static uint8_t identifier_constant(Token* name) {
    return make_constant(OBJ_VAL(copy_string(name->start, name->length)));
}

static bool identifiers_equal(Token* a, Token* b) {
    if (a->length != b->length) return false;
    return memcmp(a->start, b->start, a->length) == 0;
}

static int resolve_local(Compiler* compiler, Token* name) {
    for (int i = compiler->local_count - 1; i >= 0; i--) {
        Local* local = &compiler->locals[i];
        if (identifiers_equal(name, &local->name)) {
            if (local->depth == -1) {
                fprintf(stderr, "Can't read local variable in its own initializer\n");
            }
            return i;
        }
    }
    return -1;
}

static void add_local(Token name) {
    if (state.current->local_count == 256) {
        fprintf(stderr, "Too many local variables in function\n");
        return;
    }
    
    Local* local = &state.current->locals[state.current->local_count++];
    local->name = name;
    local->depth = -1;
}

static void declare_variable() {
    if (state.current->scope_depth == 0) return;
    
    Token* name = &state.parser.previous;
    
    for (int i = state.current->local_count - 1; i >= 0; i--) {
        Local* local = &state.current->locals[i];
        if (local->depth != -1 && local->depth < state.current->scope_depth) {
            break;
        }
        
        if (identifiers_equal(name, &local->name)) {
            fprintf(stderr, "Already a variable with this name in this scope\n");
        }
    }
    
    add_local(*name);
}

static void mark_initialized() {
    if (state.current->scope_depth == 0) return;
    state.current->locals[state.current->local_count - 1].depth = state.current->scope_depth;
}

static void compile_expr(Expr* expr);
static void compile_stmt(Stmt* stmt);

static void compile_literal(LiteralExpr* expr) {
    switch (expr->type) {
        case LITERAL_NUMBER:
            emit_constant(NUMBER_VAL(expr->as.number.value));
            break;
        case LITERAL_BOOL:
            emit_byte(expr->as.boolean.value ? OP_TRUE : OP_FALSE);
            break;
        case LITERAL_NIL:
            emit_byte(OP_NIL);
            break;
    }
}

static void compile_unary(UnaryExpr* expr) {
    compile_expr(expr->operand);
    
    switch (expr->op) {
        case TOKEN_MINUS:
            emit_byte(OP_NEGATE);
            break;
        case TOKEN_BANG:
            emit_byte(OP_NOT);
            break;
        default:
            return;
    }
}

static void compile_binary(BinaryExpr* expr) {
    compile_expr(expr->left);
    compile_expr(expr->right);
    
    switch (expr->op) {
        case TOKEN_PLUS:      emit_byte(OP_ADD); break;
        case TOKEN_MINUS:     emit_byte(OP_SUBTRACT); break;
        case TOKEN_STAR:      emit_byte(OP_MULTIPLY); break;
        case TOKEN_SLASH:     emit_byte(OP_DIVIDE); break;
        case TOKEN_PERCENT:   emit_byte(OP_MODULO); break;
        case TOKEN_EQ_EQ:     emit_byte(OP_EQUAL); break;
        case TOKEN_BANG_EQ:   emit_bytes(OP_EQUAL, OP_NOT); break;
        case TOKEN_GT:        emit_byte(OP_GREATER); break;
        case TOKEN_GT_EQ:     emit_bytes(OP_LESS, OP_NOT); break;
        case TOKEN_LT:        emit_byte(OP_LESS); break;
        case TOKEN_LT_EQ:     emit_bytes(OP_GREATER, OP_NOT); break;
        default:
            return;
    }
}

static void compile_variable(VariableExpr* expr) {
    int arg = resolve_local(state.current, &expr->name);
    if (arg != -1) {
        emit_bytes(OP_GET_LOCAL, (uint8_t)arg);
    } else {
        uint8_t name = identifier_constant(&expr->name);
        emit_bytes(OP_GET_GLOBAL, name);
    }
}

static void compile_assign(AssignExpr* expr) {
    compile_expr(expr->value);
    
    int arg = resolve_local(state.current, &expr->name);
    if (arg != -1) {
        emit_bytes(OP_SET_LOCAL, (uint8_t)arg);
    } else {
        uint8_t name = identifier_constant(&expr->name);
        emit_bytes(OP_SET_GLOBAL, name);
    }
}

static void compile_call(CallExpr* expr) {
    compile_expr(expr->callee);
    
    for (size_t i = 0; i < expr->arg_count; i++) {
        compile_expr(expr->arguments[i]);
    }
    
    emit_bytes(OP_CALL, (uint8_t)expr->arg_count);
}

static void compile_logical(LogicalExpr* expr) {
    compile_expr(expr->left);
    
    if (expr->op == TOKEN_OR) {
        int else_jump = emit_jump(OP_JUMP_IF_FALSE);
        int end_jump = emit_jump(OP_JUMP);
        
        patch_jump(else_jump);
        emit_byte(OP_POP);
        
        compile_expr(expr->right);
        patch_jump(end_jump);
    } else {
        int end_jump = emit_jump(OP_JUMP_IF_FALSE);
        
        emit_byte(OP_POP);
        compile_expr(expr->right);
        
        patch_jump(end_jump);
    }
}

static void compile_expr(Expr* expr) {
    switch (expr->type) {
        case EXPR_LITERAL:
            compile_literal(&expr->as.literal);
            break;
        case EXPR_UNARY:
            compile_unary(&expr->as.unary);
            break;
        case EXPR_BINARY:
            compile_binary(&expr->as.binary);
            break;
        case EXPR_VARIABLE:
            compile_variable(&expr->as.variable);
            break;
        case EXPR_ASSIGN:
            compile_assign(&expr->as.assign);
            break;
        case EXPR_CALL:
            compile_call(&expr->as.call);
            break;
        case EXPR_LOGICAL:
            compile_logical(&expr->as.logical);
            break;
    }
}

static void compile_expr_stmt(ExprStmt* stmt) {
    compile_expr(stmt->expression);
    emit_byte(OP_POP);
}

static void compile_let_stmt(LetStmt* stmt) {
    if (stmt->initializer != NULL) {
        compile_expr(stmt->initializer);
    } else {
        emit_byte(OP_NIL);
    }
    
    if (state.current->scope_depth > 0) {
        declare_variable();
        mark_initialized();
    } else {
        uint8_t global = identifier_constant(&stmt->name);
        emit_bytes(OP_DEFINE_GLOBAL, global);
    }
}

static void compile_block_stmt(BlockStmt* stmt) {
    for (size_t i = 0; i < stmt->count; i++) {
        compile_stmt(stmt->statements[i]);
    }
}

static void compile_if_stmt(IfStmt* stmt) {
    compile_expr(stmt->condition);
    
    int then_jump = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    compile_stmt(stmt->then_branch);
    
    int else_jump = emit_jump(OP_JUMP);
    
    patch_jump(then_jump);
    emit_byte(OP_POP);
    
    if (stmt->else_branch != NULL) {
        compile_stmt(stmt->else_branch);
    }
    
    patch_jump(else_jump);
}

static void compile_while_stmt(WhileStmt* stmt) {
    int loop_start = current_chunk()->count;
    
    compile_expr(stmt->condition);
    
    int exit_jump = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    
    compile_stmt(stmt->body);
    emit_loop(loop_start);
    
    patch_jump(exit_jump);
    emit_byte(OP_POP);
}

static void compile_function_stmt(FunctionStmt* stmt) {
    Compiler compiler;
    init_compiler(&compiler, TYPE_FUNCTION);
    begin_scope();
    
    state.current->function->name = copy_string(stmt->name.start, stmt->name.length);
    state.current->function->arity = stmt->param_count;
    
    for (size_t i = 0; i < stmt->param_count; i++) {
        uint8_t constant = identifier_constant(&stmt->params[i]);
        declare_variable();
        mark_initialized();
    }
    
    for (size_t i = 0; i < stmt->body_count; i++) {
        compile_stmt(stmt->body[i]);
    }
    
    ObjFunction* function = end_compiler();
    emit_bytes(OP_CONSTANT, make_constant(OBJ_VAL(function)));
    
    if (state.current->scope_depth > 0) {
        declare_variable();
        mark_initialized();
    } else {
        uint8_t global = identifier_constant(&stmt->name);
        emit_bytes(OP_DEFINE_GLOBAL, global);
    }
}

static void compile_return_stmt(ReturnStmt* stmt) {
    if (stmt->value != NULL) {
        compile_expr(stmt->value);
    } else {
        emit_byte(OP_NIL);
    }
    emit_byte(OP_RETURN);
}

static void compile_print_stmt(PrintStmt* stmt) {
    compile_expr(stmt->expression);
    emit_byte(OP_PRINT);
}

static void compile_stmt(Stmt* stmt) {
    switch (stmt->type) {
        case STMT_EXPR:
            compile_expr_stmt(&stmt->as.expr_stmt);
            break;
        case STMT_LET:
            compile_let_stmt(&stmt->as.let_stmt);
            break;
        case STMT_BLOCK:
            begin_scope();
            compile_block_stmt(&stmt->as.block);
            end_scope();
            break;
        case STMT_IF:
            compile_if_stmt(&stmt->as.if_stmt);
            break;
        case STMT_WHILE:
            compile_while_stmt(&stmt->as.while_stmt);
            break;
        case STMT_FUNCTION:
            compile_function_stmt(&stmt->as.function);
            break;
        case STMT_RETURN:
            compile_return_stmt(&stmt->as.return_stmt);
            break;
        case STMT_PRINT:
            compile_print_stmt(&stmt->as.print_stmt);
            break;
    }
}

ObjFunction* compile(const char* source) {
    parser_init(&state.parser, source);
    
    Compiler compiler;
    init_compiler(&compiler, TYPE_SCRIPT);
    
    state.had_error = false;
    
    Program* program = parse(&state.parser);
    
    if (state.parser.had_error) {
        free_program(program);
        free(program);
        return NULL;
    }
    
    for (size_t i = 0; i < program->count; i++) {
        compile_stmt(program->statements[i]);
    }
    
    free_program(program);
    free(program);
    
    ObjFunction* function = end_compiler();
    
    return state.had_error ? NULL : function;
}
