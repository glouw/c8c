#pragma once

struct node
{
    char* name;
    unsigned address;
    struct node* left;
    struct node* rite;
};

extern void burn(struct node* tree);
extern struct node* get(struct node* tree, char* name);
extern struct node* insert(struct node* tree, struct node* node);
extern struct node* new(char* name, unsigned address);
extern void print(struct node* tree);
