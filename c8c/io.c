#include "io.h"

#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// The current input file character.
static int now;

// The line number.
static int nline = 1;

// Input file (c8).
static FILE* fi;

static char* source;

// Output file (asm).
static FILE* fo;

static char* assembly;

// Line buffer.
static char* lbuff;

// Number of characters reads from input file.
static int reads;

static int failure;

// Writes to the output file. A new line is included.
static void print(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(fo, msg, args);
    fprintf(fo, "\n");
    va_end(args);
}

// Writes to standard error. A newline is included. Output is in red.
static void bomb(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "error: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
    failure = 1;
    exit(1);
}

// Closes eveyrthing done. Removes the assembly file
// if something went wrong during compilation.
static void shutdown()
{
    free(lbuff);
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(failure)
        remove(assembly);
}

// Buffers a new character from the input file.
static void buff()
{
    lbuff[reads++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        reads = 0;
        print(";%s", lbuff);
    }
}

// Gets a new charcter from the input file.
// Does not ignore comments.
static void step()
{
    now = fgetc(fi);
    buff();
}

// Gets a new charcter from the input file.
// Ignore comments.
static void next()
{
    step();
    if(now == '/')
    {
        step();
        if(now != '/')
            bomb("expected '/'");
        while(now != '\n')
            step();
    }
}

// Skips white face.
static void skip()
{
    while(isspace(now))
        next();
}

static void init(char* argv[])
{
    source = argv[1];
    assembly = argv[2];
    // Input
    fi = fopen(argv[1], "r");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", source);
        exit(1);
    }
    // Output
    fo = fopen(argv[2], "w");
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s cannot be made\n", assembly);
        exit(1);
    }
    atexit(shutdown);
    lbuff = (char*) malloc(512 * sizeof(char));
    next();
    skip();
}

// Returns the current char of the input file.
static char peek()
{
    return now;
}

// Same as peek(), but in string form.
static char* peeks()
{
    char n[] = { (char) now, '\0' };
    return str.dup(n);
}

// Returns true if input file is now at the end of an expression.
static int isendexpr()
{
    return now == ';'
        || now == ')'
        || now == ']'
        || now == ',';
}

// Returns true if end of operator.
static int isendop()
{
    return isalnum(now) || isspace(now) || isendexpr();
}

// Exits if the current character does not match what is expected.
// Also advances the unput buffer by one.
static void match(const int c)
{
    skip();
    if(now != c)
        bomb("expected '%c'", c);
    next();
}

// Exits if the current string does not match what is expected.
// Also advances the unput buffer by the length of the expected string.
static void matches(const char* s)
{
    skip();
    for(char* i = (char*) s; *i; i++)
    {
        if(now != *i)
            bomb("expected '%s'", s);
        next();
    }
}

// Gets a number as a name.
static char* gname()
{
    skip();
    // First char must be alpha or underscore
    if(!(now == '_' || isalpha(now)))
        bomb("names must start with underscores or alpha chars");
    char* name = (char*) malloc(128 * sizeof(char));
    int i = 0;
    // But name can contain alpha an numeric chars.
    while(isalnum(now) || now == '_')
    {
        name[i++] = now;
        next();
    }
    name[i] = '\0';
    return name;
}

// Gets a number as a string.
static char* gnum()
{
    skip();
    if(!isdigit(now))
        bomb("expected value");
    char* num = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(isdigit(now) || isxdigit(now) || tolower(now) == 'x')
    {
        num[i++] = now;
        next();
    }
    num[i] = '\0';
    return num;
}

// Gets an operation as a string.
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

// Returns true if end of file.
static int eof()
{
    return now == EOF;
}

const struct io io = {
    print, bomb, next, skip, init, peek, peeks, isendexpr,
    isendop, match, gname, gnum, gop, eof, matches
};
