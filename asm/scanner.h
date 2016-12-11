#pragma once

#include "tree.h"

const struct scanner
{
    struct node* (*scan)(struct node* tree);
}
scanner;
