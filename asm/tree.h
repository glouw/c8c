#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct node
{
    char* name;
    unsigned address;
    struct node* left;
    struct node* rite;
};

const struct tree
{
    struct node* (*build)(const char* name, unsigned address);
    struct node* (*insert)(struct node* nodes, struct node* node);
    struct node* (*get)(struct node* nodes, const char* name);
    void (*burn)(struct node* nodes);
    void (*print)(struct node* nodes);
}
tree;
