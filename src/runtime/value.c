#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/algo_value.h"

static Obj* objects = NULL;

void init_chunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    chunk->constants = NULL;
    chunk->constant_count = 0;
    chunk->constant_capacity = 0;
}

void write_chunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        size_t old_capacity = chunk->capacity;
        chunk->capacity = old_capacity < 8 ? 8 : old_capacity * 2;
        chunk->code = realloc(chunk->code, chunk->capacity);
        chunk->lines = realloc(chunk->lines, chunk->capacity * sizeof(int));
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int add_constant(Chunk* chunk, Value value) {
    if (chunk->constant_capacity < chunk->constant_count + 1) {
        size_t old_capacity = chunk->constant_capacity;
        chunk->constant_capacity = old_capacity < 8 ? 8 : old_capacity * 2;
        chunk->constants = realloc(chunk->constants, chunk->constant_capacity * sizeof(Value));
    }
    chunk->constants[chunk->constant_count] = value;
    return chunk->constant_count++;
}

void free_chunk(Chunk* chunk) {
    free(chunk->code);
    free(chunk->lines);
    free(chunk->constants);
    init_chunk(chunk);
}

static Obj* allocate_object(size_t size, ObjType type) {
    Obj* object = malloc(size);
    object->type = type;
    object->next = objects;
    objects = object;
    return object;
}

static uint32_t hash_string(const char* key, size_t length) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* allocate_string(char* chars, size_t length, uint32_t hash) {
    ObjString* string = (ObjString*)allocate_object(sizeof(ObjString), OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    return string;
}

ObjString* copy_string(const char* chars, size_t length) {
    uint32_t hash = hash_string(chars, length);
    char* heap_chars = malloc(length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length, hash);
}

ObjString* take_string(char* chars, size_t length) {
    uint32_t hash = hash_string(chars, length);
    return allocate_string(chars, length, hash);
}

ObjFunction* new_function() {
    ObjFunction* function = (ObjFunction*)allocate_object(sizeof(ObjFunction), OBJ_FUNCTION);
    function->arity = 0;
    function->name = NULL;
    init_chunk(&function->chunk);
    return function;
}

ObjNative* new_native(NativeFn function) {
    ObjNative* native = (ObjNative*)allocate_object(sizeof(ObjNative), OBJ_NATIVE);
    native->function = function;
    return native;
}

static void print_function(ObjFunction* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->chars);
}

void print_value(Value value) {
    switch (value.type) {
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_OBJ:
            switch (AS_OBJ(value)->type) {
                case OBJ_STRING:
                    printf("%s", AS_CSTRING(value));
                    break;
                case OBJ_FUNCTION:
                    print_function(AS_FUNCTION(value));
                    break;
                case OBJ_NATIVE:
                    printf("<native fn>");
                    break;
            }
            break;
    }
}

bool values_equal(Value a, Value b) {
    if (a.type != b.type) return false;
    
    switch (a.type) {
        case VAL_NIL:
            return true;
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ:
            return AS_OBJ(a) == AS_OBJ(b);
        default:
            return false;
    }
}

void free_objects() {
    Obj* object = objects;
    while (object != NULL) {
        Obj* next = object->next;
        switch (object->type) {
            case OBJ_STRING: {
                ObjString* string = (ObjString*)object;
                free(string->chars);
                free(object);
                break;
            }
            case OBJ_FUNCTION: {
                ObjFunction* function = (ObjFunction*)object;
                free_chunk(&function->chunk);
                free(object);
                break;
            }
            case OBJ_NATIVE:
                free(object);
                break;
        }
        object = next;
    }
}
