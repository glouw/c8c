#pragma once

#include "tree.h"

struct scanner
{
    struct node* (*scan)(struct node* tree);
};

extern const struct scanner scanner;
