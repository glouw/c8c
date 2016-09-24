#include "assemble.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>

static int add(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // ADD Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c4\n", a[1], b[1]);
    // ADD I, Vx
    else
    if(strlen(a) == 1 && a[0] == 'I' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c1E\n", b[1]);
    // ADD Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "7%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int and(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // AND Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c2\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int call(FILE* hexid, char* operand, struct node* tree)
{
    char* a = strtok(operand, "\t ");
    struct node* found = get(tree, a);
    // CALL address
    if(found)
        fprintf(hexid, "2%03X\n", found->address);
    else
        return 2;
    return 0;
}

static int cls(FILE* hexid, char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    fprintf(hexid, "00E0\n");
    return 0;
}

static int db(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ");
    // DB
    if(strlen(a) == 4 && strncmp(a, "0x", 2) == 0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]))
           fprintf(hexid, "%c%c\n", a[2], a[3]);
    else
        return 1;
    return 0;
}

static int drw(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    char* c = strtok(NULL, "\t ");
    // DRW Vx, Vy, nib
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]) &&
       strlen(c) == 3 && strncmp(c, "0x", 2) == 0 && isxdigit(c[2]))
           fprintf(hexid, "D%c%c%c\n", a[1], b[1], c[2]);
    else
        return 1;
    return 0;
}

static int end(FILE* hexid, char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    // End
    fprintf(hexid, "00EF\n");
    return 0;
}

static int jp(FILE* hexid, char* operand, struct node* tree)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // JP V0, address
    if(strlen(a) == 2 && a[0] == 'V' && a[1] == '0' &&
       (found = get(tree, b)))
           fprintf(hexid, "B%03X\n", found->address);
    // JP ad
    else
    if((found = get(tree, a)))
        fprintf(hexid, "1%03X\n", found->address);
    else
        return 2;
    return 0;
}

static int ld(FILE* hexid, char* operand, struct node* tree)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // LD DT, Vx
    if(strlen(a) == 2 && a[0] == 'D' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c15\n", b[1]);
    // LD ST, Vx
    else
    if(strlen(a) == 2 && a[0] == 'S' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c18\n", b[1]);
    // LD F, Vx
    else
    if(strlen(a) == 1 && a[0] == 'F' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c29\n", b[1]);
    // LD B, Vx
    else
    if(strlen(a) == 1 && a[0] == 'B' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c33\n", b[1]);
    // LD Vx, DT
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'D' && b[1] == 'T')
           fprintf(hexid, "F%c07\n", a[1]);
    // LD Vx, [I]
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 3 && b[0] == '[' && b[1] == 'I' && b[2] == ']')
           fprintf(hexid, "F%c65\n", a[1]);
    // LD Vx, K
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 1 && b[0] == 'K')
           fprintf(hexid, "F%c0A\n", a[1]);
    // LD Vx, Vy
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c0\n", a[1], b[1]);
    // LD Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
      (strlen(b) == 4 && strncmp(b, "0x", 2)) == 0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(hexid, "6%c%c%c\n", a[1], b[2], b[3]);
    // LD [I], Vx
    else
    if(strlen(a) == 3 && a[0] == '[' && a[1] == 'I' && a[2] == ']' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "F%c55\n", b[1]);
    // LD I, ad
    else
    if(strlen(a) == 1 && a[0] == 'I')
    {
       if((found = get(tree, b)))
           fprintf(hexid, "A%03X\n", found->address);
       else
           return 2;
    }
    else
        return 1;
    return 0;
}

static int or(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // OR Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c1\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int ret(FILE* hexid, char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    // RET
    fprintf(hexid, "00EE\n");
    return 0;
}

static int rnd(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // RND Vx, byte
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "C%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int se(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SE Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "5%c%c0\n", a[1], b[1]);
    // SE Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "3%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int shl(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SHL Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%cE\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int shr(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SHR Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c6\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int sknp(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ");
    // SKNP Vx
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]))
        fprintf(hexid, "E%cA1\n", a[1]);
    else
        return 1;
    return 0;
}

static int skp(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ");
    // SKP Vx
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]))
        fprintf(hexid, "E%c9E\n", a[1]);
    else
        return 1;
    return 0;
}

static int sne(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SNE Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "9%c%c0\n", a[1], b[1]);
    // SNE Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "4%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int sub(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SUB Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c5\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int subn(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SUBN Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c7\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int xor(FILE* hexid, char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // XOR Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c3\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int (*functions[])(FILE* hexid, char* operand, struct node* tree) = {
    add, and, call, cls, db, drw, end, jp, ld, or, ret, rnd, se,
    shl, shr, sknp, skp, sne, sub, subn, xor
};

static char* mnemonics[] = {
    "ADD","AND","CALL","CLS","DB","DRW","END","JP","LD","OR","RET","RND","SE",
    "SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","XOR"
};

static int compare(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

int assemble(FILE* hexid, char* mnemonic, char* operand, struct node* tree)
{
    #define size(array) sizeof(array) / sizeof(char*)
    char** found = (char**)bsearch(mnemonic, mnemonics, size(mnemonics), sizeof(char*), compare);
    if(!found)
        return 3;
    int index = found - mnemonics;
    return (*functions[index])(hexid, operand, tree);
}
