#pragma once

#include <stdbool.h>

struct feed
{
    char (*name)();
    int (*number)();
    void (*init)();
    bool (*isop)();
    char (*peek)();
    void (*match)(char x);
};

extern const struct feed feed;
