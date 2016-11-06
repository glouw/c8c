#include "errors.h"

#include <stdio.h>

static void handle(int error, unsigned linenumber)
{
    char* types[] = {
        /* 0 */ "no error",
        /* 1 */ "operand formatting",
        /* 2 */ "label not found",
        /* 3 */ "unsupported chip8 mnemonic"
    };
    fprintf(stderr, "error: line %d: %s\n", linenumber, types[error]);
}

const struct errors errors = { handle };
