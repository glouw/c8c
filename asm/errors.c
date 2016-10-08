#include "errors.h"

#include <stdio.h>

static void handle(int error, unsigned linenumber)
{
    char* types[] = {
        "no error",
        "operand formatting",
        "label not found",
        "unsupported chip8 mnemonic"
    };
    fprintf(stderr, "error: line %d: %s\n", linenumber, types[error]);
}

struct errors errors = {
    .handle = handle
};
