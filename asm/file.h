#pragma once

#include "tree.h"

#include <stdio.h>

const struct file
{
    FILE* (*input)(void);
    FILE* (*output)(void);
    void (*open)(char* argv[static 3]);
}
file;
