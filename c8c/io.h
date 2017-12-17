#pragma once

#include <stdio.h>

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
