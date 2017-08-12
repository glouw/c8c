#pragma once

#include <stdio.h>

struct io
{
    void (*init)(FILE* i, FILE* o);
    char (*get)();
    void (*bomb)(const char* message, ...);
    void (*emit)(const char* message, ...);
    void (*print)(const char* message, ...);
};

extern const struct io io;
