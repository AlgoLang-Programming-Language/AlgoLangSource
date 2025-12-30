#include <stdio.h>
#include <math.h>
#include "../../include/algo_value.h"
#include "../../include/algo_vm.h"

static Value native_abs(int arg_count, Value* args) {
    if (arg_count != 1) {
        fprintf(stderr, "abs() takes exactly 1 argument\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0])) {
        fprintf(stderr, "abs() argument must be a number\n");
        return NIL_VAL;
    }
    
    return NUMBER_VAL(fabs(AS_NUMBER(args[0])));
}

static Value native_min(int arg_count, Value* args) {
    if (arg_count != 2) {
        fprintf(stderr, "min() takes exactly 2 arguments\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0]) || !IS_NUMBER(args[1])) {
        fprintf(stderr, "min() arguments must be numbers\n");
        return NIL_VAL;
    }
    
    double a = AS_NUMBER(args[0]);
    double b = AS_NUMBER(args[1]);
    return NUMBER_VAL(a < b ? a : b);
}

static Value native_max(int arg_count, Value* args) {
    if (arg_count != 2) {
        fprintf(stderr, "max() takes exactly 2 arguments\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0]) || !IS_NUMBER(args[1])) {
        fprintf(stderr, "max() arguments must be numbers\n");
        return NIL_VAL;
    }
    
    double a = AS_NUMBER(args[0]);
    double b = AS_NUMBER(args[1]);
    return NUMBER_VAL(a > b ? a : b);
}

static Value native_sqrt(int arg_count, Value* args) {
    if (arg_count != 1) {
        fprintf(stderr, "sqrt() takes exactly 1 argument\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0])) {
        fprintf(stderr, "sqrt() argument must be a number\n");
        return NIL_VAL;
    }
    
    double value = AS_NUMBER(args[0]);
    if (value < 0) {
        fprintf(stderr, "sqrt() argument must be non-negative\n");
        return NIL_VAL;
    }
    
    return NUMBER_VAL(sqrt(value));
}

static Value native_pow(int arg_count, Value* args) {
    if (arg_count != 2) {
        fprintf(stderr, "pow() takes exactly 2 arguments\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0]) || !IS_NUMBER(args[1])) {
        fprintf(stderr, "pow() arguments must be numbers\n");
        return NIL_VAL;
    }
    
    return NUMBER_VAL(pow(AS_NUMBER(args[0]), AS_NUMBER(args[1])));
}

static Value native_floor(int arg_count, Value* args) {
    if (arg_count != 1) {
        fprintf(stderr, "floor() takes exactly 1 argument\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0])) {
        fprintf(stderr, "floor() argument must be a number\n");
        return NIL_VAL;
    }
    
    return NUMBER_VAL(floor(AS_NUMBER(args[0])));
}

static Value native_ceil(int arg_count, Value* args) {
    if (arg_count != 1) {
        fprintf(stderr, "ceil() takes exactly 1 argument\n");
        return NIL_VAL;
    }
    
    if (!IS_NUMBER(args[0])) {
        fprintf(stderr, "ceil() argument must be a number\n");
        return NIL_VAL;
    }
    
    return NUMBER_VAL(ceil(AS_NUMBER(args[0])));
}

void define_native(const char* name, NativeFn function);

void init_stdlib() {
    define_native("abs", native_abs);
    define_native("min", native_min);
    define_native("max", native_max);
    define_native("sqrt", native_sqrt);
    define_native("pow", native_pow);
    define_native("floor", native_floor);
    define_native("ceil", native_ceil);
}
