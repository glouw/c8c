#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* node.c */
struct node
{
    int address;
    char* label;
    struct node* next;
};
void dump(struct node* list);                                   /* display list from top */
void delt(struct node* list);                                   /* delete list top from top */
int find(struct node* list, const char* label);                 /* hash find function where label is key */
struct node* push(struct node* list, int address, char* label); /* insert node at top of list */

/* opcodes.c */
void add (FILE* fp, char* o);
void and (FILE* fp, char* o);
bool call(FILE* fp, char* o, struct node* l, int address);
void cls (FILE* fp);
void drw (FILE* fp, char* o);
bool jump(FILE* fp, char* o, struct node* l, int address);
bool ld  (FILE* fp, char* o, struct node* l, int address);
void or  (FILE* fp, char* o);
void ret (FILE* fp);
void rnd (FILE* fp, char* o);
void se  (FILE* fp, char* o);
void shl (FILE* fp, char* o);
void shr (FILE* fp, char* o);
void skp (FILE* fp, char* o);
void skpn(FILE* fp, char* o);
void sne (FILE* fp, char* o);
void sub (FILE* fp, char* o);
void subn(FILE* fp, char* o);
void xor (FILE* fp, char* o);
