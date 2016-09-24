#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct node* insert(struct node* tree, struct node* node)
{
    if(tree == NULL)
        return node;
    int difference = strcmp(node->name, tree->name);
    if(difference == 0)
    {
        fprintf(stderr, "error: %s already defined\n", node->name);
        free(node->name);
        free(node);
        exit(-1);
    }
    else if(difference < 0)
        tree->left = insert(tree->left, node);
    else
        tree->rite = insert(tree->rite, node);
    return tree;
}
