#pragma once

#include <stdbool.h>

union flags
{
    struct
    {
        unsigned tree : 1;
    };
    unsigned any;
}
flags;
