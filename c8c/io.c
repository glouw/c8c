#include "io.h"

#include <stdlib.h>
#include <stdarg.h>

FILE* in;
FILE* out;

void init(FILE* i, FILE* o) { in = i; out = o; }

static char get() { return fgetc(in); }

static void compose(const char* prefix, const char* message, va_list args)
{
    fprintf(out, "%s", prefix);
    vfprintf(out, message, args);
    fprintf(out, "\n");
}

static void bomb(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    compose("error: ", message, args);
    va_end(args);
    exit(1);
}

static void emit(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    compose("\t", message, args);
    va_end(args);
}

static void print(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    compose("", message, args);
    va_end(args);
}

const struct io io = { init, get, bomb, emit, print };
