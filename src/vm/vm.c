#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "../../include/algo_vm.h"
#include "../../include/algo_compiler.h"
#include "../../include/algo_bytecode.h"

static VM vm;

static void reset_stack() {
    vm.stack_top = vm.stack;
    vm.frame_count = 0;
}

static void runtime_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    
    for (int i = vm.frame_count - 1; i >= 0; i--) {
        CallFrame* frame = &vm.frames[i];
        ObjFunction* function = frame->function;
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[line %d] in ", function->chunk.lines[instruction]);
        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        } else {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    
    reset_stack();
}

static void init_globals() {
    vm.globals.entries = NULL;
    vm.globals.count = 0;
    vm.globals.capacity = 0;
}

void init_vm() {
    reset_stack();
    vm.objects = NULL;
    init_globals();
}

void free_vm() {
    free(vm.globals.entries);
}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

static Value peek(int distance) {
    return vm.stack_top[-1 - distance];
}

static bool is_falsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static bool call(ObjFunction* function, int arg_count) {
    if (arg_count != function->arity) {
        runtime_error("Expected %d arguments but got %d", function->arity, arg_count);
        return false;
    }
    
    if (vm.frame_count == FRAMES_MAX) {
        runtime_error("Stack overflow");
        return false;
    }
    
    CallFrame* frame = &vm.frames[vm.frame_count++];
    frame->function = function;
    frame->ip = function->chunk.code;
    frame->slots = vm.stack_top - arg_count - 1;
    return true;
}

static bool call_value(Value callee, int arg_count) {
    if (IS_OBJ(callee)) {
        switch (AS_OBJ(callee)->type) {
            case OBJ_FUNCTION:
                return call(AS_FUNCTION(callee), arg_count);
            case OBJ_NATIVE: {
                NativeFn native = AS_NATIVE(callee);
                Value result = native(arg_count, vm.stack_top - arg_count);
                vm.stack_top -= arg_count + 1;
                push(result);
                return true;
            }
            default:
                break;
        }
    }
    runtime_error("Can only call functions");
    return false;
}

static uint32_t hash_string_raw(const char* key, size_t length) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static ObjString* table_find_string(ObjString** entries, int capacity, const char* chars, size_t length, uint32_t hash) {
    if (capacity == 0) return NULL;
    
    uint32_t index = hash % capacity;
    
    while (true) {
        ObjString* entry = entries[index];
        
        if (entry == NULL) {
            return NULL;
        }
        
        if (entry->length == length && entry->hash == hash &&
            memcmp(entry->chars, chars, length) == 0) {
            return entry;
        }
        
        index = (index + 1) % capacity;
    }
}

static bool table_get(ObjString** entries, int capacity, ObjString* key, Value* value) {
    if (capacity == 0) return false;
    
    uint32_t index = key->hash % capacity;
    
    while (true) {
        ObjString* entry = entries[index];
        if (entry == NULL) return false;
        
        if (entry == key) {
            return false;
        }
        
        index = (index + 1) % capacity;
    }
}

static void table_set(ObjString*** entries, int* count, int* capacity, ObjString* key, Value value) {
    if (*count + 1 > *capacity * 0.75) {
        int new_capacity = *capacity < 8 ? 8 : *capacity * 2;
        ObjString** new_entries = calloc(new_capacity, sizeof(ObjString*));
        
        for (int i = 0; i < *capacity; i++) {
            if ((*entries)[i] != NULL) {
                ObjString* entry = (*entries)[i];
                uint32_t index = entry->hash % new_capacity;
                while (new_entries[index] != NULL) {
                    index = (index + 1) % new_capacity;
                }
                new_entries[index] = entry;
            }
        }
        
        free(*entries);
        *entries = new_entries;
        *capacity = new_capacity;
    }
    
    uint32_t index = key->hash % *capacity;
    while ((*entries)[index] != NULL && (*entries)[index] != key) {
        index = (index + 1) % *capacity;
    }
    
    bool is_new = (*entries)[index] == NULL;
    if (is_new) (*count)++;
    (*entries)[index] = key;
}

static InterpretResult run() {
    CallFrame* frame = &vm.frames[vm.frame_count - 1];
    
#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() \
    (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_CONSTANT() (frame->function->chunk.constants[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(value_type, op) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtime_error("Operands must be numbers"); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(value_type(a op b)); \
    } while (false)
    
    while (true) {
        uint8_t instruction = READ_BYTE();
        
        switch (instruction) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL:
                push(NIL_VAL);
                break;
            case OP_TRUE:
                push(BOOL_VAL(true));
                break;
            case OP_FALSE:
                push(BOOL_VAL(false));
                break;
            case OP_POP:
                pop();
                break;
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(frame->slots[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                frame->slots[slot] = peek(0);
                break;
            }
            case OP_GET_GLOBAL: {
                ObjString* name = READ_STRING();
                Value value;
                if (!table_get(vm.globals.entries, vm.globals.capacity, name, &value)) {
                    runtime_error("Undefined variable '%s'", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString* name = READ_STRING();
                table_set(&vm.globals.entries, &vm.globals.count, &vm.globals.capacity, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString* name = READ_STRING();
                Value value;
                if (!table_get(vm.globals.entries, vm.globals.capacity, name, &value)) {
                    runtime_error("Undefined variable '%s'", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                table_set(&vm.globals.entries, &vm.globals.count, &vm.globals.capacity, name, peek(0));
                break;
            }
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(values_equal(a, b)));
                break;
            }
            case OP_GREATER:
                BINARY_OP(BOOL_VAL, >);
                break;
            case OP_LESS:
                BINARY_OP(BOOL_VAL, <);
                break;
            case OP_ADD:
                BINARY_OP(NUMBER_VAL, +);
                break;
            case OP_SUBTRACT:
                BINARY_OP(NUMBER_VAL, -);
                break;
            case OP_MULTIPLY:
                BINARY_OP(NUMBER_VAL, *);
                break;
            case OP_DIVIDE:
                BINARY_OP(NUMBER_VAL, /);
                break;
            case OP_MODULO: {
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
                    runtime_error("Operands must be numbers");
                    return INTERPRET_RUNTIME_ERROR;
                }
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                push(NUMBER_VAL(fmod(a, b)));
                break;
            }
            case OP_NOT:
                push(BOOL_VAL(is_falsey(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtime_error("Operand must be a number");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_PRINT: {
                print_value(pop());
                printf("\n");
                break;
            }
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                frame->ip += offset;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if (is_falsey(peek(0))) frame->ip += offset;
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                frame->ip -= offset;
                break;
            }
            case OP_CALL: {
                int arg_count = READ_BYTE();
                if (!call_value(peek(arg_count), arg_count)) {
                    return INTERPRET_RUNTIME_ERROR;
                }
                frame = &vm.frames[vm.frame_count - 1];
                break;
            }
            case OP_RETURN: {
                Value result = pop();
                vm.frame_count--;
                if (vm.frame_count == 0) {
                    pop();
                    return INTERPRET_OK;
                }
                
                vm.stack_top = frame->slots;
                push(result);
                frame = &vm.frames[vm.frame_count - 1];
                break;
            }
        }
    }
    
#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
    ObjFunction* function = compile(source);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;
    
    push(OBJ_VAL(function));
    call(function, 0);
    
    return run();
}
