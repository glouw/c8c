#pragma once

typedef struct node Node;

struct node
{
    Node* next;
    int rp;
    char* name;
};

struct ident
{
    void (*init)();
    Node* (*create)(char* name, const int rp);
    void (*push)(Node* const node);
    Node (*pop)();
    Node* (*find)(char* name);
};

extern const struct ident ident;
