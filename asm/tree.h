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

struct tree
{
    struct node* (*build)(char* name, unsigned address);
    struct node* (*insert)(struct node* nodes, struct node* node);
    struct node* (*get)(struct node* nodes, char* name);
    void (*burn)(struct node* nodes);
    void (*print)(struct node* nodes);
    bool poisoned; // Indicates if something went wrong
};

extern struct tree tree;
