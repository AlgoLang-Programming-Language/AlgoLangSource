#ifndef ALGO_COMPILER_H
#define ALGO_COMPILER_H

#include "algo_common.h"
#include "algo_ast.h"
#include "algo_value.h"

typedef struct {
    Token name;
    int depth;
} Local;

typedef enum {
    TYPE_FUNCTION,
    TYPE_SCRIPT
} FunctionType;

typedef struct Compiler {
    struct Compiler* enclosing;
    ObjFunction* function;
    FunctionType type;
    
    Local locals[256];
    int local_count;
    int scope_depth;
} Compiler;

ObjFunction* compile(const char* source);

#endif
