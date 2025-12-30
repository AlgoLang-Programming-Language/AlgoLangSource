#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/algo_common.h"
#include "../include/algo_vm.h"
#include "../include/algo_value.h"

extern void init_stdlib();
extern void free_objects();

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\"\n", path);
        exit(74);
    }
    
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    
    char* buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\"\n", path);
        exit(74);
    }
    
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file \"%s\"\n", path);
        exit(74);
    }
    
    buffer[bytes_read] = '\0';
    
    fclose(file);
    return buffer;
}

static void repl() {
    char line[1024];
    
    printf("Algolang %s\n", ALGO_VERSION);
    printf("Type 'exit' to quit\n\n");
    
    while (true) {
        printf("> ");
        
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        
        if (strcmp(line, "exit\n") == 0) {
            break;
        }
        
        interpret(line);
    }
}

static void run_file(const char* path) {
    char* source = read_file(path);
    InterpretResult result = interpret(source);
    free(source);
    
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

void define_native(const char* name, NativeFn function) {
    push(OBJ_VAL(copy_string(name, strlen(name))));
    push(OBJ_VAL(new_native(function)));
    
    ObjString* name_str = AS_STRING(vm.stack[0]);
    Value func_val = vm.stack[1];
    
    extern VM vm;
    
    if (vm.globals.count + 1 > vm.globals.capacity * 0.75) {
        int new_capacity = vm.globals.capacity < 8 ? 8 : vm.globals.capacity * 2;
        ObjString** new_entries = calloc(new_capacity, sizeof(ObjString*));
        
        for (int i = 0; i < vm.globals.capacity; i++) {
            if (vm.globals.entries[i] != NULL) {
                ObjString* entry = vm.globals.entries[i];
                uint32_t index = entry->hash % new_capacity;
                while (new_entries[index] != NULL) {
                    index = (index + 1) % new_capacity;
                }
                new_entries[index] = entry;
            }
        }
        
        free(vm.globals.entries);
        vm.globals.entries = new_entries;
        vm.globals.capacity = new_capacity;
    }
    
    uint32_t index = name_str->hash % vm.globals.capacity;
    while (vm.globals.entries[index] != NULL && vm.globals.entries[index] != name_str) {
        index = (index + 1) % vm.globals.capacity;
    }
    
    if (vm.globals.entries[index] == NULL) vm.globals.count++;
    vm.globals.entries[index] = name_str;
    
    pop();
    pop();
}

int main(int argc, const char* argv[]) {
    init_vm();
    init_stdlib();
    
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage: algolang [path]\n");
        exit(64);
    }
    
    free_vm();
    free_objects();
    
    return 0;
}  
