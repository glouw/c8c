#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/* create new node */
static node* new(char* token, int address)
{
    node* n = malloc(sizeof(node));
    n->token = strdup(token);
    n->address = address;
    n->less = NULL;
    n->more = NULL;
    return n;
}

/* add node to labels */
static node* add(node* labels, node* n)
{
    if(labels == NULL) return n;
    int cmp = strcmp(n->token, labels->token);
    if(cmp == 0); // do nothing
    else if(cmp < 0) labels->less = add(labels->less, n);
    else labels->more = add(labels->more, n);
    return labels;
}

/* get node from labels */
static node* get(node* labels, char* token)
{
    if(labels == NULL) return NULL;
    int cmp = strcmp(token, labels->token);
    if(cmp == 0) return labels;
    else if(cmp < 0) return get(labels->less, token);
    else return get(labels->more, token);
}

/* delete all nodes in labels */
static void delete(node* labels)
{
    if(labels == NULL) return;
    delete(labels->less);
    delete(labels->more);
    free(labels->token);
    free(labels);
}

/* print all nodes in labels */
static void print(node* labels)
{
    if(labels == NULL) return;
    print(labels->less);
    printf("%04X: %s\n", labels->address, labels->token);
    print(labels->more);
}

const struct tree tree = { new, add, get, delete, print };
