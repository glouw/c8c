#include "io.h"

#include <stdlib.h>
#include <stdarg.h>

FILE* i;
FILE* o;

static void close()
{
    if(i) fclose(i);
    if(o) fclose(o);
}

static void init(FILE* in, FILE* out)
{
    atexit(close);
    i = in;
    o = out;
    if(!i) exit(1);
}

static char get()
{
    return fgetc(i);
}

static void compose(const char* prefix, const char* message, va_list args)
{
    fprintf(o, "%s", prefix);
    vfprintf(o, message, args);
    fprintf(o, "\n");
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
