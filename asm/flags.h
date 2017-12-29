#pragma once

#include <stdbool.h>

union flags
{
    struct
    {
        unsigned tree : 1, generator : 1;
    }
    bits;
    unsigned any;
};

extern union flags flags;
