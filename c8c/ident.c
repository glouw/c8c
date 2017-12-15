#include "ident.h"

#include "io.h"

#include <string.h>
#include <stdlib.h>

static Node* nodes = NULL;

static void kill(struct node* node)
{
    free(node->name);
    free(node);
}

static void close()
{
    for(Node* n = nodes, *next; n; n = next)
    {
        next = n->next;
        kill(n);
    }
}

static void init()
{
    atexit(close);
}

static Node* create(char* name, const int rp)
{
    Node* n = (Node*) malloc(sizeof(*n));
    n->name = name;
    n->rp = rp;
    n->next = NULL;
    return n;
}

static void push(Node* const node)
{
    node->next = nodes;
    nodes = node;
}

static Node pop()
{
    Node front = *nodes;
    Node* next = nodes->next;
    free(nodes->name);
    free(nodes);
    nodes = next;
    return front;
}

static Node* find(char* name)
{
    for(Node* n = nodes; n; n = n->next)
        if(strcmp(n->name, name) == 0)
            return n;
    return NULL;
}

const struct ident ident = { init, create, push, pop, find };
