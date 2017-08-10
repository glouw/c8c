#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct node
{
    char* name;
    unsigned address;
    struct node* l;
    struct node* r;
};

struct tree
{
    struct node* (*build)(char* name, unsigned address);
    struct node* (*insert)(struct node* nodes, struct node* node);
    struct node* (*get)(struct node* nodes, const char* name);
    void (*burn)(struct node* nodes);
    void (*print)(struct node* nodes);
};

extern const struct tree tree;
