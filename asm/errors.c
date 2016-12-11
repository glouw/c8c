#include "errors.h"

#include <stdio.h>

static void handle(const int error, const unsigned linenumber)
{
    const char* const types[] = {
        /* 0 */ "no error",
        /* 1 */ "operand formatting",
        /* 2 */ "label not found",
        /* 3 */ "unsupported chip8 mnemonic"
    };
    fprintf(stderr, "error: line %d: %s\n", linenumber, types[error]);
}

const struct errors errors = {
    .handle = handle
};
