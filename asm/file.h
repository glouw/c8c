#pragma once

#include "tree.h"

#include <stdio.h>

extern FILE* input;
extern FILE* output;

struct file
{
    void (*open)(int argc, char* argv[]);
};

extern const struct file file;
