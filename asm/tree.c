/*
 *  Modified Binary Tree
 *  The Practice of Programming
 *  Brian W. Kernighan
 *  Rob Pike
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/* create new tnode */
static tnode* new(const char* label, int address)
{
    tnode* n = malloc(sizeof(tnode));
    n->label = strdup(label);
    n->address = address;
    n->less = NULL;
    n->more = NULL;
    return n;
}

/* add tnode to labels */
static tnode* add(tnode* labels, tnode* n)
{
    if(labels == NULL) return n;
    int cmp = strcmp(n->label, labels->label);
    if(cmp == 0); /* do nothing */
    else if(cmp < 0) labels->less = add(labels->less, n);
    else labels->more = add(labels->more, n);
    return labels;
}

/* get tnode from labels */
static tnode* get(tnode* labels, const char* label)
{
    if(labels == NULL) return NULL;
    int cmp = strcmp(label, labels->label);
    if(cmp == 0) return labels;
    else if(cmp < 0) return get(labels->less, label);
    else return get(labels->more, label);
}

/* delete all tnodes in labels */
static void delete(tnode* labels)
{
    if(labels == NULL) return;
    delete(labels->less);
    delete(labels->more);
    free(labels->label);
    free(labels);
}

/* print all tnodes in labels */
static void print(tnode* labels)
{
    if(labels == NULL) return;
    print(labels->less);
    printf("%04X: %s\n", labels->address, labels->label);
    print(labels->more);
}

const struct tree tree = { new, add, get, delete, print };
