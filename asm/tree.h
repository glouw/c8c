#pragma once

typedef struct node node;
struct node
{
    char* token;
    int address;
    node* less;
    node* more;
};

struct tree
{
    /* Creates a new node on the heap */
    node* (*new)(char* token, int address); 
    /* Adds a node to the tree */
    node* (*add)(node* nodes, node* new);
    /* Gets a node from the tree */
    node* (*get)(node* nodes, char* token);
    /* Frees the entire tree from heap */
    void (*burn)(node* nodes);
    /* Prints tree to stdout */
    void (*print)(node* nodes);
}
const tree;
