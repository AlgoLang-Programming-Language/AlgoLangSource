#ifndef ALGO_PARSER_H
#define ALGO_PARSER_H

#include "algo_common.h"
#include "algo_token.h"
#include "algo_ast.h"

typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
} Parser;

void parser_init(Parser* parser, const char* source);
Program* parse(Parser* parser);

#endif
