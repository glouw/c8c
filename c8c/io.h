#pragma once

#include <stdio.h>

#define NRM "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

struct io
{
    void (*init)(FILE* in, FILE* out);
    int (*get)(void);
    void (*bomb)(const char* message, ...);
    void (*emit)(const char* message, ...);
    void (*print)(const char* message, ...);
    int (*size)(void);
};

extern const struct io io;
