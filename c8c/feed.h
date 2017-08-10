#pragma once

#include <stdbool.h>

const struct feed
{
    char (*name)();
    int (*number)();
    void (*init)();
    bool (*isop)();
    char (*peek)();
    void (*match)(char x);
}
feed;
