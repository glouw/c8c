#include "tree.h"

#include <stdio.h>

void print(struct node* tree)
{
    if(tree == NULL)
        return;
    fprintf(stderr, "%04X: %s\n", tree->address, tree->name);
    print(tree->left);
    print(tree->rite);
}
