#include "tree.h"

#include "errors.h"
#include "flags.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static struct node* build(const char* name, unsigned address)
{
    struct node* node = malloc(sizeof(*node));
    node->name = strdup(name);
    node->address = address;
    node->left = NULL;
    node->rite = NULL;
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
        flags.tree = true;
    }
    else if(difference < 0)
        nodes->left = insert(nodes->left, node);
    else
        nodes->rite = insert(nodes->rite, node);
    return nodes;
}

static struct node* get(struct node* nodes, const char* name)
{
    if(nodes == NULL)
        return NULL;
    int difference = strcmp(name, nodes->name);
    if(difference == 0)
        return nodes;
    else if(difference < 0)
        return get(nodes->left, name);
    else
        return get(nodes->rite, name);
}

static void burn(struct node* nodes)
{
    if(nodes == NULL)
        return;
    burn(nodes->left);
    burn(nodes->rite);
    free(nodes->name);
    free(nodes);
}

static void print(struct node* nodes)
{
    if(nodes == NULL)
        return;
    fprintf(stderr, "%04X: %s\n", nodes->address, nodes->name);
    print(nodes->left);
    print(nodes->rite);
}

const struct tree tree = {
    .build = build,
    .insert = insert,
    .get = get,
    .burn = burn,
    .print = print
};
