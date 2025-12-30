#ifndef ALGO_VALUE_H
#define ALGO_VALUE_H

#include "algo_common.h"

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_OBJ
} ValueType;

typedef struct Obj Obj;
typedef struct ObjString ObjString;
typedef struct ObjFunction ObjFunction;
typedef struct ObjNative ObjNative;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
    } as;
} Value;

#define IS_NIL(value)    ((value).type == VAL_NIL)
#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJ(value)    ((value).type == VAL_OBJ)

#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value)    ((value).as.obj)

#define NIL_VAL          ((Value){VAL_NIL, {.number = 0}})
#define BOOL_VAL(val)    ((Value){VAL_BOOL, {.boolean = val}})
#define NUMBER_VAL(val)  ((Value){VAL_NUMBER, {.number = val}})
#define OBJ_VAL(object)  ((Value){VAL_OBJ, {.obj = (Obj*)object}})

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
    OBJ_NATIVE
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

struct ObjString {
    Obj obj;
    size_t length;
    char* chars;
    uint32_t hash;
};

typedef struct {
    uint8_t* code;
    size_t count;
    size_t capacity;
    Value* constants;
    size_t constant_count;
    size_t constant_capacity;
    int* lines;
} Chunk;

struct ObjFunction {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
};

typedef Value (*NativeFn)(int arg_count, Value* args);

struct ObjNative {
    Obj obj;
    NativeFn function;
};

#define IS_STRING(value)   is_obj_type(value, OBJ_STRING)
#define IS_FUNCTION(value) is_obj_type(value, OBJ_FUNCTION)
#define IS_NATIVE(value)   is_obj_type(value, OBJ_NATIVE)

#define AS_STRING(value)   ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString*)AS_OBJ(value))->chars)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)   (((ObjNative*)AS_OBJ(value))->function)

static inline bool is_obj_type(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

void init_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
int add_constant(Chunk* chunk, Value value);
void free_chunk(Chunk* chunk);

ObjString* copy_string(const char* chars, size_t length);
ObjString* take_string(char* chars, size_t length);
ObjFunction* new_function();
ObjNative* new_native(NativeFn function);

void print_value(Value value);
bool values_equal(Value a, Value b);

#endif
