#pragma once

#include "tree.h"

#include <stdio.h>

struct file
{
    void (*open)(int argc, char* argv[]);
    FILE* input;
    FILE* output;
};

extern struct file file;
