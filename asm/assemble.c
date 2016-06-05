#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tree.h"
#include "opcode.h"

/* Unsigned word (data type) */
static int _uword(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    /* UWORD */
    if(strlen(a)==6 && strncmp(a,"0x",2)==0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]) &&
       isxdigit(a[4]) &&
       isxdigit(a[5]))
           fprintf(hexidecimal, "%s", a+2);
    else return 1;
    return 0;
}

/* Unsigned byte (data type) */
static int _ubyte(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    /* UBYTE */
    if(strlen(a)==4 && strncmp(a,"0x",2)==0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]))
           fprintf(hexidecimal, "%s", a+2);
    else return 1;
    return 0;
}

/* Addition (+) */
static int add(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* ADD Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c4", a[1], b[1]);
    /* ADD I, Vx */
    else
    if(strlen(a)==1 && a[0]=='I' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c1E", b[1]);
    /* ADD Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b,"0x",2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexidecimal, "7%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* Bitwise and (&) */
static int and(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* AND Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c2", a[1], b[1]);
    else return 1;
    return 0;
}

/* Call subroutine */
static int call(char* operand, node* tree, FILE* hexidecimal)
{
    char* a = strtok(operand, "\t ,");
    node* found = get(tree, a);
    /* CALL ad */
    if(found)
        fprintf(hexidecimal, "2%03X", found->address);
    else return 2;
    return 0;
}

/* Clear display */
static int cls(char* operand, node* tree, FILE* hexidecimal)
{
    (void)o, (void)hexidecimal, (void)tree;
    /* CLS */
    fprintf(hexidecimal, "00E0");
    return 0;
}

/* Draw sprite */
static int drw(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    char* c = strtok(NULL, "\t ,");
    /* DRW Vx, Vy, nib */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) &&
       strlen(c)==3 && strncmp(c, "0x", 2)==0 && isxdigit(c[2]))
           fprintf(hexidecimal, "D%c%c%c", a[1], b[1], c[2]);
    else return 1;
    return 0;
}

/* Jump */
static int jp(char* operand, node* tree, FILE* hexidecimal)
{
    node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* JP V0, ad */
    if(strlen(a)==2 && a[0]=='V' && a[1]=='0' &&
       (found = get(tree, b)))
           fprintf(hexidecimal, "B%03X", found->address);
    /* JP ad */
    else
    if((found = get(tree, a)))
        fprintf(hexidecimal, "1%03X", found->address);
    else return 2;
    return 0;
}

/* Load */
static int ld(char* operand, node* tree, FILE* hexidecimal)
{
    node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* LD DT, Vx */
    if(strlen(a)==2 && a[0]=='D' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c15", b[1]);
    /* LD ST, Vx */
    else
    if(strlen(a)==2 && a[0]=='S' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c18", b[1]);
    /* LD F, Vx */
    else
    if(strlen(a)==1 && a[0]=='F' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c29", b[1]);
    /* LD B, Vx */
    else
    if(strlen(a)==1 && a[0]=='B' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c33", b[1]);
    /* LD Vx, DT */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='D' && b[1]=='T')
           fprintf(hexidecimal, "F%c07", a[1]);
    /* LD Vx, [I] */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==3 &&
       b[0]=='[' &&
       b[1]=='I' &&
       b[2]==']')
           fprintf(hexidecimal, "F%c65", a[1]);
    /* LD Vx, K */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==1 && b[0]=='K')
           fprintf(hexidecimal, "F%c0A", a[1]);
    /* LD Vx, Vy */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c0", a[1], b[1]);
    /* LD Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
      (strlen(b)==4 && strncmp(b, "0x", 2))==0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(hexidecimal, "6%c%s", a[1], b+2);
    /* LD [I], Vx */
    else
    if(strlen(a)==3 &&
       a[0]=='[' &&
       a[1]=='I' &&
       a[2]==']' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c55", b[1]);
    /* LD I, ad */
    else
    if(strlen(a)==1 && a[0]=='I')
    {
       if((found = get(tree, b)))
           fprintf(hexidecimal, "A%03X", found->address);
       else return 2;
    }
    else return 1;
    return 0;
}

/* Bitwise or (|) */
static int or(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* OR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c1", a[1], b[1]);
    else return 1;
    return 0;
}

/* Return from subroutine */
static int ret(char* operand, node* tree, FILE* hexidecimal)
{
    (void)o, (void)hexidecimal, (void)tree;
    /* RET */
    fprintf(hexidecimal, "00EE");
    return 0;
}

/* Random number with mask */
static int rnd(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* RND Vx, byte */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexidecimal, "C%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* Skip instruction if equal */
static int se(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SE Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "5%c%c0", a[1], b[1]);
    /* SE Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexidecimal, "3%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* Shift left */
static int shl(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SHL Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%cE", a[1], b[1]);
    else return 1;
    return 0;
}

/* Shift right */
static int shr(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SHR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c6", a[1], b[1]);
    else return 1;
    return 0;
}

/* Skip instruction if keypress equal */
static int skp(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    /* SKP Vx */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(hexidecimal, "E%c9E", a[1]);
    else return 1;
    return 0;
}

/* Skip instruction if keypress not equal */
static int sknp(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    /* SKNP Vx */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(hexidecimal, "E%cA1", a[1]);
    else return 1;
    return 0;
}

/* Skip instruction if not equal */
static int sne(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SNE Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "9%c%c0", a[1], b[1]);
    /* SNE Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexidecimal, "4%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* Subtract (-) */
static int sub(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SUB Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c5", a[1], b[1]);
    else return 1;
    return 0;
}

/* Inverse subtract */
static int subn(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SUBN Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c7", a[1], b[1]);
    else return 1;
    return 0;
}

/* Exit program */
static int end(char* operand, node* tree, FILE* hexidecimal)
{
    (void)o, (void)tree, (void)hexidecimal;
    /* exit */
    fprintf(hexidecimal, "00EF");
    return 0;
}

/* Exslusive or (^) */
static int xor(char* operand, node* tree, FILE* hexidecimal)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* XOR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c3", a[1], b[1]);
    else return 1;
    return 0;
}

static int (*function[])(char* operand, node* tree, FILE* hexidecimal) =
{ /*   0     1      2     3     4     5    6    7    8     9    10   11    12    13     14    15    16    17     18      19      20    21 */
     add,  and,  call,  cls,  drw,  end,  jp,  ld,  or,  ret,  rnd,  se,  shl,  shr,  sknp,  skp,  sne,  sub,  subn, _ubyte, _uword,  xor
};
static char* mnemonic[] =
{ /*   0     1      2     3     4     5    6    7    8     9    10   11    12    13     14    15    16    17     18      19      20    21 */
    "ADD","AND","CALL","CLS","DRW","END","JP","LD","OR","RET","RND","SE","SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","UBYTE","UWORD","XOR"
};

static int execute(int (*_function)(char*,node*,FILE*), char* operand, node* tree, FILE* hexidecimal)
{
    return _function(o, tree, hexidecimal);
}

/* Bsearch callback */
static int compare(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

/* Assembles given a mnemonic m, an operand o, a label tree, and an output file; returns error code */
static int assemble(char* m, char* operand, node* tree, FILE* hexidecimal)
{
    /* Check if 'm' is a supported mnemonic */
    #define len(mnemonic) sizeof(mnemonic)/sizeof(*mnemonic)
    char** supported = bsearch(m, mnemonic, len(mnemonic), sizeof(char*), compare);
    /* If 'm' is not supported return an error */
    if(!supported)
        return 3;
    /* If the operand is missing and the operand is not CLS, END, or RET then return "a missing operand" error */
    int index = supported - mnemonic;
    if(o == NULL && (index != 3 && index != 5 && index != 9))
        return 4;
    /* Execute */
    int error = execute(function[index], o, tree, hexidecimal);
    /* Report any other errors */
    if(!error)
        fputc('\n', hexidecimal);
    return error;
}
