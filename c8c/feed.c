#include "feed.h"

#include "io.h"

#include <string.h>
#include <ctype.h>

static char now;

static void spin()
{
    now = io.get();
    if(now == ' ')
        spin();
}

static void init()
{
    spin();
}

static char name()
{
    if(!isalpha(now)) io.bomb("expected name, not '%c'", now);
    const char c = now;
    spin();
    return c;
}

static int number()
{
    if(!isdigit(now)) io.bomb("expected number, not '%c'", now);
    const int i = now - '0';
    spin();
    return i;
}

static bool isop()
{
    return strchr("+-|&^", now);
}

static char peek()
{
    return now;
}

static void match(const char x)
{
    now == x ? spin() : io.bomb("expected %c", x);
}

const struct feed feed = { name, number, init, isop, peek, match };
