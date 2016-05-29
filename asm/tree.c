#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/* Create new node */
static node* new(char* token, int address)
{
    node* new = malloc(sizeof(node));
    new->token = strdup(token);
    new->address = address;
    new->less = NULL;
    new->more = NULL;
    return new;
}

/* Add node to nodes */
static node* add(node* nodes, node* new)
{
    if(nodes == NULL)
        return new;
    int cmp = strcmp(new->token, nodes->token);
    if(cmp == 0)
        ; // do nothing
    else if(cmp < 0)
        nodes->less = add(nodes->less, new);
    else
        nodes->more = add(nodes->more, new);
    return nodes;
}

/* Get node from nodes */
static node* get(node* nodes, char* token)
{
    if(nodes == NULL)
        return NULL;
    int cmp = strcmp(token, nodes->token);
    if(cmp == 0)
        return nodes;
    else if(cmp < 0)
        return get(nodes->less, token);
    else
        return get(nodes->more, token);
}

/* Delete all nodes in nodes */
static void burn(node* nodes)
{
    if(nodes == NULL)
        return;
    burn(nodes->less);
    burn(nodes->more);
    free(nodes->token);
    free(nodes);
}

/* Print all nodes in nodes */
static void print(node* nodes)
{
    if(nodes == NULL)
        return;
    print(nodes->less);
    printf("%04X: %s\n", nodes->address, nodes->token);
    print(nodes->more);
}

const struct tree tree = { new, add, get, burn, print };
