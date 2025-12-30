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

extern void global_set(ObjString* key, Value value);

void define_native(const char* name, NativeFn function) {
    ObjString* name_str = copy_string(name, strlen(name));
    ObjNative* native = new_native(function);
    
    push(OBJ_VAL(name_str));
    push(OBJ_VAL(native));
    
    global_set(name_str, OBJ_VAL(native));
    
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
