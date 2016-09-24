#include "tree.h"

#include <string.h>
#include <stdlib.h>

struct node* get(struct node* tree, char* name)
{
    int difference;
    if(tree == NULL)
        return NULL;
    difference = strcmp(name, tree->name);
    if(!difference)
        return tree;
    else if(difference < 0)
        return get(tree->left, name);
    else
        return get(tree->rite, name);
}
