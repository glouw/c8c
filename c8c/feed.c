#include "feed.h"

#include "io.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

FILE* in;
static char now;

static void run()
{
    now = getc(in);
}

static void init()
{
    in = stdin;
    run();
}

static char name()
{
    if(!isalpha(now))
        io.bomb("expected name, not '%c'", now);
    char c = now;
    run();
    return c;
}

static int number()
{
    if(!isdigit(now))
        io.bomb("expected number, not '%c'", now);
    int n = now - '0';
    run();
    return n;
}

static bool isop()
{
    return strchr("+-|&^", now);
}

static char peek()
{
    return now;
}

static void match(char x)
{
    now == x ? run() : io.bomb("expected %c", x);
}

const struct feed feed = { name, number, init, isop, peek, match };
