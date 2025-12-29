#ifndef ALGO_VALUE_H
#define ALGO_VALUE_H

typedef enum {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_NULL,
    VAL_FUNCTION,
    VAL_NATIVE,
} ValueType;

typedef struct Value Value;
typedef struct Function Function;

typedef Value (*NativeFn)(int arg_count, Value* args);

struct Function {
    int arity;
    int chunk_index;
    char* name;
};

struct Value {
    ValueType type;
    union {
        double number;
        int boolean;
        Function* function;
        NativeFn native;
    };
};

Value make_number(double n);
Value make_bool(int b);
Value make_null(void);
Value make_function(Function* fn);
Value make_native(NativeFn fn);

int is_truthy(Value v);
int values_equal(Value a, Value b);
void print_value(Value v);

#endif
