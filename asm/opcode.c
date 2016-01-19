#include "public.h"

/* error message for reading wrong label */
static void error(int address, char* label)
{
    int linenum = (address-0x0200) / 2;
    fprintf(stderr, "line %d: error: %s label undeclared\n", linenum, label);
}

/* 0 ADD I,  Vx   */
/*   ADD Vx, Vy   */
/*   ADD Vx, byte */
void add(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    if(a[0]=='V' && b[0]=='V')
        fprintf(fp, "8%c%c4", a[1], b[1]);
    else if(a[0]=='I')
        fprintf(fp, "F%c1E", b[1]);
    else
        fprintf(fp, "7%c%c%c", a[1], b[0], b[1]);
}
/* 1 AND Vx, Vy */
void and(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c2", a[1], b[1]);
}
/* 2 CALL ad */
bool call(FILE* fp, char* o, struct node* l, int address)
{
    int paddress = find(l, o);
    if(paddress == -1)
    {
        error(address, o);
        return true;
    }
    else fprintf(fp, "2%X", paddress);
    return false;
}
/* 3 CLS */
void cls(FILE* fp)
{
    fprintf(fp, "00E0");
}
/* 4 DRW Vx, Vy, nibble */
void drw(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, ",");
    char* c = strtok(NULL, "\t ;");
    fprintf(fp, "D%c%c%c", a[1], b[1], c[0]);
}
/* 5 JUMP V0, ad */
/*        ad     */
bool jump(FILE* fp, char* o, struct node* l, int address)
{
    char* a = strtok(o, ",\t ;");
    char* b = strtok(NULL, "\t ;");
    if(a[0]=='V')
    {
        int paddress = find(l, b);
        if(paddress == -1)
        {
            error(address, b);
            return true;
        }
        else fprintf(fp, "B%X", paddress);
    }
    else
    {
        int paddress = find(l, a);
        if(paddress == -1)
        {
            error(address, a);
            return true;
        }
        else fprintf(fp, "1%X", paddress);
    }
    return false;
}
/* 6 LD  DT, Vx   */
/*   LD  ST, Vx   */
/*   LD  F,  Vx   */
/*   LD  B,  Vx   */
/*   LD  Vx, DT   */
/*   LD  Vx  I    */
/*   LD  Vx  K    */
/*   LD  Vx  Vy   */
/*   LD  Vx  byte */
/*   LD  I,  Vx   */
/*   LD  I,  ad   */
bool ld(FILE* fp, char* o, struct node* l, int address)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    switch(a[0])
    {
        case 'V':
            switch(b[0])
            {
                case 'D': fprintf(fp, "F%c07", a[1]); break;
                case 'I': fprintf(fp, "F%c65", a[1]); break;
                case 'K': fprintf(fp, "F%c0A", a[1]); break;
                case 'V': fprintf(fp, "8%c%c0",  a[1], b[1]); break;
                default : fprintf(fp, "6%c%c%c", a[1], b[0], b[1]); break;
            }
            break;
        case 'I':
            if(b[0]=='V')
                fprintf(fp, "F%c55", b[1]);
            else
            {
                int paddress = find(l, b);
                if(paddress == -1)
                {
                    error(address, b);
                    return true;
                }
                else fprintf(fp, "A%X", paddress);
            }
            break;
        case 'D': fprintf(fp, "F%c15", b[1]); break;
        case 'S': fprintf(fp, "F%c18", b[1]); break;
        case 'F': fprintf(fp, "F%c29", b[1]); break;
        case 'B': fprintf(fp, "F%c33", b[1]); break;
    }
    return false;
}
/* 7 OR Vx, Vy */
void or(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c1", a[1], b[1]);
}
/* 8 RET */
void ret(FILE* fp)
{
    fprintf(fp, "00EE");
}
/* 9 RND Vx, byte */
void rnd(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "C%c%c%c", a[1], b[0], b[1]);
}
/* 10 SE Vx, Vy   */
/*    SE Vx, byte */
void se(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    if(b[0]=='V')
        fprintf(fp, "5%c%c0",  a[1], b[1]);
    else
        fprintf(fp, "3%c%c%c", a[1], b[0], b[1]);
}
/* 11 SHL Vx, Vy */
void shl(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%cE", a[1], b[1]);
}
/* 12 SHR Vx, Vy */
void shr(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c6", a[1], b[1]);
}
/* 14 SKP Vx */
void skp(FILE* fp, char* o)
{
    fprintf(fp, "E%c9E", o[1]);
}
/* 13 SKNP Vx */
void skpn(FILE* fp, char* o)
{
    fprintf(fp, "E%cA1", o[1]);
}
/* 15 SNE Vx, Vy   */
/*    SNE Vx, byte */
void sne(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    if(b[0]=='V')
        fprintf(fp, "9%c%c0",  a[1], b[1]);
    else
        fprintf(fp, "4%c%c%c", a[1], b[0], b[1]);
}
/* 16 SUB Vx, Vy */
void sub(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c5", a[1], b[1]);
}
/* 17 SUBN Vx, Vy */
void subn(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c7", a[1], b[1]);
}
/* 18 XOR Vx, Vy */
void xor(FILE* fp, char* o)
{
    char* a = strtok(o, ",");
    char* b = strtok(NULL, "\t ;");
    fprintf(fp, "8%c%c3", a[1], b[5]);
}
