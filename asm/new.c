#include "tree.h"

#include <string.h>
#include <stdlib.h>

struct node* new(char* name, unsigned address)
{
    struct node* node = malloc(sizeof(*node));
    node->name = strdup(name);
    node->address = address;
    node->left = NULL;
    node->rite = NULL;
    return node;
}
