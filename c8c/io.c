#include "io.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "str.h"

static int now;

static int nline = 1;

static FILE* in;

static FILE* out;

static char* lbuff;

static int reads;

static void print(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(out, msg, args);
    fprintf(out, "\n");
    va_end(args);
}

static void bomb(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "\x1B[31merror: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\x1B[0m\n");
    va_end(args);
    exit(1);
}

static void shutdown()
{
    free(lbuff);
    if(in)
        fclose(in);
    if(out)
        fclose(out);
}

static void buff()
{
    lbuff[reads++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        reads = 0;
        print("%s", lbuff);
    }
}

static void next()
{
    now = fgetc(in);
    buff();
}

static void skip()
{
    while(isspace(now))
        next();
}

static void init()
{
    in = fopen("tests/test.c8", "r");
    out = stdout;
    atexit(shutdown);
    lbuff = (char*) malloc(512 * sizeof(char));
    next();
    skip();
}

static char peek()
{
    return now;
}

static char* peeks()
{
    char n[] = { (char) now, '\0'};
    return str.dup(n);
}

static int isendexpr()
{
    return now == ';'
        || now == ')'
        || now == ',';
}

static int isendop()
{
    return isalnum(now) || isspace(now) || isendexpr();
}

static void match(const char c)
{
    if(now != c)
        bomb("expected '%c'", c);
    next();
}

static char* gname()
{
    skip();
    char* name = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(isalpha(now))
    {
        name[i++] = now;
        next();
    }
    name[i] = '\0';
    return name;
}

static char* gnum()
{
    skip();
    if(!isdigit(now))
        bomb("expected value");
    char* num = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(isdigit(now))
    {
        num[i++] = now;
        next();
    }
    num[i] = '\0';
    return num;
}

static char* gop()
{
    skip();
    char* op = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(!isendop())
    {
        op[i++] = now;
        next();
    }
    op[i] = '\0';
    return op;
}

static int end()
{
    return now == EOF;
}

const struct io io = {
    print, bomb, next, skip, init, peek, peeks, isendexpr, isendop, match, gname, gnum, gop, end
};
