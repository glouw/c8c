#include "tree.h"

#include <stdlib.h>

void burn(struct node* tree)
{
    if(tree == NULL)
        return;
    burn(tree->left);
    burn(tree->rite);
    free(tree->name);
    free(tree);
}
