#pragma once

#include "tree.h"

#include <stdio.h>

struct files
{
    FILE* (*input)();
    FILE* (*output)();
    void (*construct)();
    void (*open)(char* argv[]);
    void (*reset)();
};

extern const struct files files;
