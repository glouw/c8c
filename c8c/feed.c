#include "feed.h"

#include "io.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

static char now;

static void run() { now = io.get(); if(now == ' ') run(); }

static void init() { run(); }

static char name()
{
    if(!isalpha(now)) io.bomb("expected name, not '%c'", now);
    const char c = now;
    run();
    return c;
}

static int number()
{
    if(!isdigit(now)) io.bomb("expected number, not '%c'", now);
    const int n = now - '0';
    run();
    return n;
}

static bool isop() { return strchr("+-|&^", now); }

static char peek() { return now; }

static void match(const char x) { now == x ? run() : io.bomb("expected %c", x); }

const struct feed feed = { name, number, init, isop, peek, match };
