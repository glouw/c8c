#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NRM "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"

/* node.c */
struct node
{
    int address;
    char* label;
    struct node* next;
};
void         dump(struct node* list);                                 /* display list from top */
void         delt(struct node* list);                                 /* delete list top from top */
int          find(struct node* list, char* label);              /* hash find function where label is key */
struct node* push(struct node* list, int address, char* label); /* insert node at top of list */

/* opcode.c*/
int   add(char* iname, FILE* fp, char* o, int address                );
int   and(char* iname, FILE* fp, char* o, int address                );
int  call(char* iname, FILE* fp, char* o, int address, struct node* l);
void  cls(             FILE* fp                                      );
int   drw(char* iname, FILE* fp, char* o, int address                );
int  jump(char* iname, FILE* fp, char* o, int address, struct node* l);
int    ld(char* iname, FILE* fp, char* o, int address, struct node* l);
void   or(             FILE* fp, char* o                             );
void  ret(             FILE* fp                                      );
int   rnd(char* iname, FILE* fp, char* o, int address                );
int    se(char* iname, FILE* fp, char* o, int address                );
void  shl(             FILE* fp, char* o                             );
void  shr(             FILE* fp, char* o                             );
void  skp(             FILE* fp, char* o                             );
void skpn(             FILE* fp, char* o                             );
int   sne(char* iname, FILE* fp, char* o, int address                );
void  sub(             FILE* fp, char* o                             );
void subn(             FILE* fp, char* o                             );
void  xor(             FILE* fp, char* o                             );

/* error.c */
int errora(char* iname, int address, char* label);           /* undeclared label*/
int errorb(char* iname, int address);                        /* missing 0x*/
int errorc(char* iname, int address);                        /* missing semicolon */
int errord(char* iname, int address);                        /* label with tab */
int errore(char* iname, int address);                        /* missing tab */
int errorf(char* iname, int address, int paddress, char* l); /* label already exists */
int errorg(char* iname, int address, char* m);               /* not a valid mnem */
int errorh(char* iname, char* const ename);            /* missing entry point ename that must be MAIN */
int errori(char* iname, int address, int nibs);              /* too few nibbles */
int errorj(char* iname, int address, char* a, char* b);    /* too few hex digits */
