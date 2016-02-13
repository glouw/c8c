#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "tree.h"
#include "assemble.h"

/* unsigned int (data type) */
static int _uint(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    /* UINT */
    if(strlen(a)==6 && strncmp(a,"0x",2)==0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]) &&
       isxdigit(a[4]) &&
       isxdigit(a[5]))
           fprintf(hexid, "%s", a+2);
    else return 1;
    return 0;
}

/* addition */
static int add(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* ADD Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c4", a[1], b[1]);
    /* ADD I, Vx */
    else
    if(strlen(a)==1 && a[0]=='I' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c1E", b[1]);
    /* ADD Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b,"0x",2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "7%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* bitwise and */
static int and(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* AND Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c2", a[1], b[1]);
    else return 1;
    return 0;
}

/* call subroutine */
static int call(char* o, tnode* labels, FILE* hexid)
{
    char* a = strtok(o, "\t ,");
    tnode* found = tree.get(labels, a);
    /* CALL ad */
    if(found)
        fprintf(hexid, "2%03X", found->address);
    else return 2;
    return 0;
}

/* clear display */
static int cls(char* o, tnode* labels, FILE* hexid)
{
    (void)o, (void)hexid, (void)labels;
    /* CLS */
    fprintf(hexid, "00E0");
    return 0;
}

/* draw sprite */
static int drw(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    char* c = strtok(NULL, "\t ,");
    /* DRW Vx, Vy, nib */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) &&
       strlen(c)==3 && strncmp(c, "0x", 2)==0 && isxdigit(c[2]))
           fprintf(hexid, "D%c%c%c", a[1], b[1], c[2]);
    else return 1;
    return 0;
}

/* jump */
static int jp(char* o, tnode* labels, FILE* hexid)
{
    tnode* found;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* JP V0, ad */
    if(strlen(a)==2 && a[0]=='V' && a[1]=='0' &&
       (found = tree.get(labels, b)))
           fprintf(hexid, "B%03X", found->address);
    /* JP ad */
    else
    if((found = tree.get(labels, a)))
        fprintf(hexid, "1%03X", found->address);
    else return 2;
    return 0;
}

/* load */
static int ld(char* o, tnode* labels, FILE* hexid)
{
    tnode* found;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* LD DT, Vx */
    if(strlen(a)==2 && a[0]=='D' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c15", b[1]);
    /* LD ST, Vx */
    else
    if(strlen(a)==2 && a[0]=='S' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c18", b[1]);
    /* LD F, Vx */
    else
    if(strlen(a)==1 && a[0]=='F' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c29", b[1]);
    /* LD B, Vx */
    else
    if(strlen(a)==1 && a[0]=='B' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c33", b[1]);
    /* LD Vx, DT */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='D' && b[1]=='T')
           fprintf(hexid, "F%c07", a[1]);
    /* LD Vx, [I] */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==3 &&
       b[0]=='[' &&
       b[1]=='I' &&
       b[2]==']')
           fprintf(hexid, "F%c65", a[1]);
    /* LD Vx, K */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==1 && b[0]=='K')
           fprintf(hexid, "F%c0A", a[1]);
    /* LD Vx, Vy */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c0", a[1], b[1]);
    /* LD Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
      (strlen(b)==4 && strncmp(b, "0x", 2))==0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(hexid, "6%c%s", a[1], b+2);
    /* LD [I], Vx */
    else
    if(strlen(a)==3 &&
       a[0]=='[' &&
       a[1]=='I' &&
       a[2]==']' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "F%c55", b[1]);
    /* LD I, ad */
    else
    if(strlen(a)==1 && a[0]=='I')
    {
       if((found = tree.get(labels, b)))
           fprintf(hexid, "A%03X", found->address);
       else return 2;
    }
    else return 1;
    return 0;
}

/* bitwise or */
static int or(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* OR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c1", a[1], b[1]);
    else return 1;
    return 0;
}

/* return from subroutine */
static int ret(char* o, tnode* labels, FILE* hexid)
{
    (void)o, (void)hexid, (void)labels;
    /* RET */
    fprintf(hexid, "00EE");
    return 0;
}

/* random number with mask */
static int rnd(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* RND Vx, byte */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "C%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* skip instruction if... */
static int se(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SE Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "5%c%c0", a[1], b[1]);
    /* SE Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "3%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* shift left */
static int shl(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SHL Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%cE", a[1], b[1]);
    else return 1;
    return 0;
}

/* shift right */
static int shr(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SHR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c6", a[1], b[1]);
    else return 1;
    return 0;
}

/* skip instruction if keypress */
static int skp(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    /* SKP Vx */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(hexid, "E%c9E", a[1]);
    else return 1;
    return 0;
}

/* do not skip instruction if keypress */
static int sknp(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    /* SKNP Vx */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(hexid, "E%cA1", a[1]);
    else return 1;
    return 0;
}

/* do not skip instruction if... */
static int sne(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SNE Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "9%c%c0", a[1], b[1]);
    /* SNE Vx, byte */
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexid, "4%c%s", a[1], b+2);
    else return 1;
    return 0;
}

/* subtract */
static int sub(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SUB Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c5", a[1], b[1]);
    else return 1;
    return 0;
}

/* reverse subtract */
static int subn(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* SUBN Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c7", a[1], b[1]);
    else return 1;
    return 0;
}

/* exit program */
static int end(char* o, tnode* labels, FILE* hexid)
{
    (void)o, (void)labels, (void)hexid;
    /* exit */
    fprintf(hexid, "00EF");
    return 0;
}

/* exslusive or */
static int xor(char* o, tnode* labels, FILE* hexid)
{
    (void)labels;
    char* a = strtok(o, "\t ,");
    char* b = strtok(NULL, "\t ,");
    /* XOR Vx, Vy */
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
           fprintf(hexid, "8%c%c3", a[1], b[1]);
    else return 1;
    return 0;
}

static int (*function[])(char* o, tnode* labels, FILE* hexid) =
{ /*   0     1      2     3     4     5    6    7    8     9   10    11    12     13    14    15    16     17    18     19    20  */
     add,  and,  call,  cls,  drw,  end,  jp,  ld,  or,  ret,  rnd,  se,  shl,  shr,  sknp,  skp,  sne,  sub,  subn, _uint,  xor
};
const char* const mnemonic[] =
{ /*   0     1      2     3     4     5    6    7    8     9   10    11    12     13    14    15    16     17    18     19    20  */
    "ADD","AND","CALL","CLS","DRW","END","JP","LD","OR","RET","RND","SE","SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","UINT","XOR"
};

static int execute(int (*_function)(char*,tnode*,FILE*), char* o, tnode* labels, FILE* hexid)
{
    return _function(o, labels, hexid);
}

/* for use with bsearch */
static int compare(const void* a, const void* b)
{
    return strcmp((const char*)a, *(const char**)b);
}

/* assembles given a mnemonic m, an operand o, a label tree, and an output file; returns error code */
int assemble(char* m, char* o, tnode* labels, FILE* hexid)
{
    int error = 0;
    /* check if 'm' is a supported mnemonic */
    const char* const* supported = bsearch(m, mnemonic, sizeof(mnemonic)/sizeof(char*), sizeof(char*), compare);
    /* if 'm' is not supported return an error */
    if(!supported) return error = 3;
    /* if the operand is missing and the operand is not CLS, END, or RET then return "a missing operand" error */
    int index = supported - mnemonic;
    if(o == NULL && (index != 3 && index != 5 && index != 9)) return error = 4;
    /* execute */
    error = execute(function[index], o, labels, hexid);
    /* ... and report any other errors */
    if(!error) fputc('\n', hexid);
    return error;
}
