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
    /* create new tnode */
    tnode* (*new)(const char* label, int address);
    /* add tnode to tree */
    tnode* (*add)(tnode* labels, tnode* n);
    /* get tnode from tree */
    tnode* (*get)(tnode* labels, const char* label);
    /* delete all tnodes in tree */
    void (*delete)(tnode* labels);
    /* print tnodes in tree */
    void (*print)(tnode* labels);
}
const tree;
