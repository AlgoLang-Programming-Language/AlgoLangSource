#ifndef ALGO_COMMON_H
#define ALGO_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ALGO_VERSION "0.2.0"

typedef enum {
    ALGO_OK = 0,
    ALGO_ERROR_SYNTAX,
    ALGO_ERROR_RUNTIME,
    ALGO_ERROR_MEMORY,
    ALGO_ERROR_TYPE,
    ALGO_ERROR_IO
} AlgoError;

#define ALGO_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s at %s:%d\n", message, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

#define ALGO_UNREACHABLE() \
    do { \
        fprintf(stderr, "Unreachable code at %s:%d\n", __FILE__, __LINE__); \
        abort(); \
    } while(0)

#endif
