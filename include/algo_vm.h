#ifndef ALGO_VM_H
#define ALGO_VM_H

#include "algo_common.h"
#include "algo_value.h"

#define STACK_MAX 256
#define FRAMES_MAX 64

typedef struct {
    ObjFunction* function;
    uint8_t* ip;
    Value* slots;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frame_count;
    
    Value stack[STACK_MAX];
    Value* stack_top;
    
    Obj* objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void init_vm();
void free_vm();
InterpretResult interpret(const char* source);

void push(Value value);
Value pop();

#endif
