#include "feed.h"

#include "io.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* String for mixed asm. */
static char* mixed;
/* Line pointer. */
static int lp;
/* Line buffer size. */
static int lbs = 8;
/* Current look ahead char. */
static int now;
/* Line count. */
static int lc = 1;

static void append(const char ch)
{
    mixed[lp++] = ch;
    if(lp == lbs)
        mixed = (char*) realloc(mixed, lbs *= 2);
}

static void spin()
{
    append(now = io.get());
    if(now == '\n')
    {
        append('\0');
        printf(";%s", mixed);
        lp = 0;
        lc++;
    }
    // Eat white space.
    if(isspace(now)) spin();
}

static int lines()
{
    return lc;
}

static void shutdown()
{
    free(mixed);
}

static void init()
{
    atexit(shutdown);
    mixed = (char*) malloc(sizeof(char) * lbs);
    spin();
}

int isname(char name)
{
    return isalnum(name) || name == '_';
}

static char* name()
{
    if(!isalpha(now))
        io.bomb("expected name, not '%c'", now);
    int size = 8;
    char* str = (char*) malloc(sizeof(char) * size);
    int i = 0;
    while(isname(now))
    {
        if(i == size - 1)
            str = (char*) realloc(str, size *= 2);
        str[i++] = now;
        spin();
    }
    str[i] = '\0';
    return str;
}

static int isop()
{
    return (int) strchr("+-|&^<>", now);
}

static int peek()
{
    return now;
}

static int end()
{
    return peek() == EOF;
}

static void match(const char x)
{
    now == x ? spin() : io.bomb("expected %c", x);
}

static void matches(const char* const str)
{
    for(const char* c = str; *c; c++) match(*c);
}

static int dec(char num[], const int len)
{
    for(int i = 0; i < len && isdigit(now); i++)
    {
        num[i] = now;
        spin();
    }
    const int d = strtol(num, NULL, 10);
    if(d > 0xFF)
        io.bomb("chip8 only supports 8 bit unsigned numbers");
    if(strcmp(num, "\0\0\0") == 0)
        io.bomb("expected a number");
    return d % 256;
}

static int hex(char num[], const int len)
{
    for(int i = 0; i < len && isxdigit(now); i++)
    {
        num[i] = now;
        spin();
    }
    const int h = strtol(num, NULL, 16);
    if(h > 0xFF)
        io.bomb("chip8 only supports 8 bit unsigned numbers");
    if(strcmp(num, "\0\0\0") == 0)
        io.bomb("expected a number");
    return h % 256;
}

static int number()
{
    char num[] = "\0\0\0"; /* Long enough to support 8 bit unsigned integers */
    if(peek() == '0')
    {
        match('0');
        if(peek() == 'x')
        {
            match('x');
            return hex(num, sizeof(num));
        }
        else return 0;
    }
    return dec(num, sizeof(num));
}

const struct feed feed = {
    name, number, init, isop, peek, match, end, matches, lines
};
