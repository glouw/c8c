#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node
{
    int address;
    char* label;
    struct node* next;
};

struct pack
{
    FILE* out;
    char* o; // operand
    struct node* list;
};

void
dump(struct node* list); /* node.c: display list from top */

void
delt(struct node* list); /* node.c: delete list top from top */

int
find(struct node* list, char* label); /* node.c: hash find function where label is key */

struct node*
push(struct node* list, int address, char* label); /* node.c: insert node at top of list */

int
assemble(int mnum, struct pack p); /* opcode.c: writes to out assembled hex base on o, mnum */
