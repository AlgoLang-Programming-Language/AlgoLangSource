#include <stdio.h>
#include <math.h>
#include "../include/algo_value.h"

Value make_number(double n) {
    Value v;
    v.type = VAL_NUMBER;
    v.number = n;
    return v;
}

Value make_bool(int b) {
    Value v;
    v.type = VAL_BOOL;
    v.boolean = b;
    return v;
}

Value make_null(void) {
    Value v;
    v.type = VAL_NULL;
    return v;
}

Value make_function(Function* fn) {
    Value v;
    v.type = VAL_FUNCTION;
    v.function = fn;
    return v;
}

Value make_native(NativeFn fn) {
    Value v;
    v.type = VAL_NATIVE;
    v.native = fn;
    return v;
}

int is_truthy(Value v) {
    switch (v.type) {
        case VAL_NULL:
            return 0;
        case VAL_BOOL:
            return v.boolean;
        case VAL_NUMBER:
            return v.number != 0.0;
        default:
            return 1;
    }
}

int values_equal(Value a, Value b) {
    if (a.type != b.type) return 0;
    
    switch (a.type) {
        case VAL_NULL:
            return 1;
        case VAL_BOOL:
            return a.boolean == b.boolean;
        case VAL_NUMBER:
            return fabs(a.number - b.number) < 1e-10;
        case VAL_FUNCTION:
            return a.function == b.function;
        case VAL_NATIVE:
            return a.native == b.native;
        default:
            return 0;
    }
}

void print_value(Value v) {
    switch (v.type) {
        case VAL_NUMBER:
            printf("%g", v.number);
            break;
        case VAL_BOOL:
            printf("%s", v.boolean ? "true" : "false");
            break;
        case VAL_NULL:
            printf("null");
            break;
        case VAL_FUNCTION:
            printf("<fn %s>", v.function->name ? v.function->name : "anonymous");
            break;
        case VAL_NATIVE:
            printf("<native fn>");
            break;
    }
}
