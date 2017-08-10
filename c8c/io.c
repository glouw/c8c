#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static void bomb(char* message, ...)
{
    va_list args;
    va_start(args, message);
    printf("error: ");
    vprintf(message, args);
    printf("\n");
    va_end(args);
    exit(1);
}

static void emit(char* message, ...)
{
    va_list args;
    va_start(args, message);
    printf("\t");
    vprintf(message, args);
    printf("\n");
    va_end(args);
}

const struct io io = { bomb, emit };
