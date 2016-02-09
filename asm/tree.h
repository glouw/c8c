#pragma once

/* tree node data type */
typedef struct tnode tnode;
struct tnode
{
    char* label;
    int address;
    tnode* less;
    tnode* more;
};

/* namespace */
struct tree
{
    /* creates a new tnode */
    tnode* (*new)(const char* label, int address);
    /* adds a tnode to a tree */
    tnode* (*add)(tnode* labels, tnode* n);
    /* gets a tnode from a tree */
    tnode* (*get)(tnode* labels, const char* label);
    /* deletes all the tnodes in a tree */
    void (*delete)(tnode* labels);
    /* prints all the tnodes in a tree */
    void (*print)(tnode* labels);
}
const tree;
