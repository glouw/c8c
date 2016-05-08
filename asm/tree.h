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
    node* (*new)(char* token, int address);  // create new node
    node* (*add)(node* labels, node* n);     // add node to tree
    node* (*get)(node* labels, char* token); // get node from tree
    void (*delete)(node* labels);            // delete all nodes in tree
    void (*print)(node* labels);             // print nodes in tree */
}
const tree;
