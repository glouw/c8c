#pragma once

#include "tree.h"

#include <stdio.h>

const struct files
{
    FILE* (*input)(void);
    FILE* (*output)(void);
    void (*construct)(void);
    void (*open)(char* argv[]);
}
files;
