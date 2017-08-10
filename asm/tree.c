#include "tree.h"

#include "flags.h"

#include <stdio.h>
#include <string.h>

static char* duplicate(char* s)
{
    int len = strlen(s) + 1;
    char* p = malloc(len);
    return p ? memcpy(p, s, len) : NULL;
}

static struct node* build(char* name, unsigned address)
{
    struct node* node = malloc(sizeof(*node));
    node->name = duplicate(name);
    node->address = address;
    node->l = NULL;
    node->r = NULL;
    return node;
}

static struct node* insert(struct node* nodes, struct node* node)
{
    if(nodes == NULL)
        return node;
    int difference = strcmp(node->name, nodes->name);
    if(difference == 0)
    {
        free(node->name);
        free(node);
        flags.bits.tree = true;
    }
    else if(difference < 0)
        nodes->l = insert(nodes->l, node);
    else
        nodes->r = insert(nodes->r, node);
    return nodes;
}

static struct node* get(struct node* nodes, char* name)
{
    if(nodes == NULL)
        return NULL;
    int difference = strcmp(name, nodes->name);
    if(difference == 0)
        return nodes;
    else if(difference < 0)
        return get(nodes->l, name);
    else
        return get(nodes->r, name);
}

static void burn(struct node* nodes)
{
    if(nodes == NULL)
        return;
    burn(nodes->l);
    burn(nodes->r);
    free(nodes->name);
    free(nodes);
}

static void print(struct node* nodes)
{
    if(nodes == NULL)
        return;
    print(nodes->l);
    fprintf(stderr, "%04X: %s\n", nodes->address, nodes->name);
    print(nodes->r);
}

const struct tree tree = {
    .build = build,
    .insert = insert,
    .get = get,
    .burn = burn,
    .print = print
};
