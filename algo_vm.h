#ifndef ALGO_VM_H
#define ALGO_VM_H

#include "algo_value.h"
#include "algo_ast.h"

typedef enum {
    OP_CONSTANT,
    OP_NULL,
    OP_TRUE,
    OP_FALSE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_NEGATE,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
    OP_CALL,
    OP_RETURN,
    OP_POP,
} OpCode;

typedef struct {
    unsigned char* code;
    int count;
    int capacity;
    Value* constants;
    int constant_count;
    int constant_capacity;
} Chunk;

typedef struct {
    Value* slots;
    int slot_count;
    unsigned char* ip;
    Function* function;
} CallFrame;

#define FRAMES_MAX 64
#define STACK_MAX 1024

typedef struct {
    Chunk chunks[256];
    int chunk_count;
    CallFrame frames[FRAMES_MAX];
    int frame_count;
    Value stack[STACK_MAX];
    int stack_top;
    Value globals[256];
    char* global_names[256];
    int global_count;
} VM;

void chunk_init(Chunk* chunk);
void chunk_write(Chunk* chunk, unsigned char byte);
int chunk_add_constant(Chunk* chunk, Value value);

void vm_init(VM* vm);
void vm_free(VM* vm);
void vm_define_global(VM* vm, const char* name, Value value);
int vm_run(VM* vm, int chunk_index);

int compile(Program* program, VM* vm, Arena* arena);

#endif  
