#include "feed.h"

#include "io.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static int now;
static int ln = 1;

static int lines()
{
    return ln;
}

static void spin()
{
    now = io.get();
    if(now == '\n')
        ln++;
    if(isspace(now))
        spin();
}

static void init()
{
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
    return (int) strchr("+-|&^", now);
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
    return h % 256;
}

static int number()
{
    char num[] = "\0\0\0"; /* Long enough to support 8 bit unsigned integers */
    if(peek() == '0')
    {
        match('0');
        switch(peek())
        {
        case 'x':
            match('x');
            return hex(num, sizeof(num));
        default:
            io.bomb("octal numbers not supported (for good reason)");
        }
    }
    return dec(num, sizeof(num));
}

const struct feed feed = {
    name, number, init, isop, peek, match, end, matches, lines
};
