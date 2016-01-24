/* int functions return 0 if there are no errors */

#include "public.h"

const char* const delima = ";, \t"; // till comma
const char* const delimb = "; \t";  // till semicol

int errorj(char* iname, int address, char* a, char* b)
{
    int linenum = (address-0x0200) / 2;
    fprintf(stderr, "%s: line %d: review operands: %s %s\n", iname, linenum, a, b);
    return 'j';
}

int add(char* iname, FILE* fp, char* o, int address) // 0
{
    char* a = strtok(o,    delima);
    char* b = strtok(NULL, delimb);
    if(strlen(a)==2         && // ADD Vx, Vy
       a[0]=='V'            &&
       isxdigit(a[1])       &&
       strlen(b)==2         &&
       b[0]=='V'            &&
       isxdigit(b[1]))
         fprintf(fp, "8%c%c4", a[1], b[1]);
    else
    if(strlen(a)==1         && // ADD I, Vx
       a[0]=='I'            &&
       strlen(b)==2         &&
       b[0]=='V'            &&
       isxdigit(b[1]))
         fprintf(fp, "F%c1E", b[1]);
    else
    if(strlen(a)==2         && // ADD Vx, byte
       a[0]=='V'            &&
       isxdigit(a[1])       &&
       strncmp(b,"0x",2)==0 &&
       strlen(b+2)==2)
         fprintf(fp, "7%c%s", a[1], b+2);
    else
        return errorj(iname, address, a, b);
    return 0;
}

int and(char* iname, FILE* fp, char* o, int address) // 1
{
    char* a = strtok(o,    delima);
    char* b = strtok(NULL, delimb);
    if(strlen(a)==2         &&
       a[0]=='V'            &&
       isxdigit(a[1])       &&
       strlen(b)==2         &&
       b[0]=='V'            &&
       isxdigit(b[1]))
         fprintf(fp, "8%c%c2", a[1], b[1]); // AND Vx, Vy
    else
        return errorj(iname, address, a, b);
    return 0;
}

int call(char* iname, FILE* fp, char* o, int address, struct node* list) // 2
{
    char* a = strtok(o, delima);
    int paddress = find(list, a);
    if(paddress == -1)
    {
        return 1;
    }
    else
    {
        fprintf(fp, "2%X", paddress); // CALL ad
    }
    return 0;
}

void cls(FILE* fp) // 3
{
    fprintf(fp, "00E0"); // CLS
}

int drw(char* iname, FILE* fp, char* o, int address) // 4
{
    char* a = strtok(o, delima);
    char* b = strtok(NULL, delima);
    char* c = strtok(NULL, delimb);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) && strncmp(c, "0x", 2)==0) {
        char* nib = c+2;
        unsigned nibs = 1;
        if(strlen(nib) != nibs)
        {
            return errori(iname, address, nibs);
        }
        else
        {
            fprintf(fp, "D%c%c%c", a[1], b[1], nib[0]); // DRW Vx, Vy, nib
        }
    }
    else // handle errors
    {
        if(strstr(c, "0x")==NULL)
        {
            return errorb(iname, address);
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int jump(char* iname, FILE* fp, char* o, int address, struct node* l) // 5
{
    char* a = strtok(o, delima);
    char* b = strtok(NULL, delimb);
    if(a && b && strlen(a)==2 && a[0]=='V' && a[1]=='0')
    {
        int paddress = find(l, b);
        if(paddress == -1)
        {
            return errora(iname, address, b);
        }
        else
        {
            fprintf(fp, "B%X", paddress); // JUMP V0, ad
        }
    }
    else if(a && !b)
    {
        int paddress = find(l, a);
        if(paddress == -1)
        {
            return errora(iname, address, a);
        }
        else
        {
            fprintf(fp, "1%X", paddress); // JUMP ad
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

int ld(char* iname, FILE* fp, char* o, int address, struct node* l) // 6
{
    int flag = 0;
    char* a = strtok(o, delima);
    char* b = strtok(NULL, delimb);
    if(strlen(a)==2 && a[0]=='D' && a[1]=='T' && strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
    {
        fprintf(fp, "F%c15", b[1]); // LD DT, Vx
    }
    else if(strlen(a)==2 && a[0]=='S' && a[1]=='T' && strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
    {
        fprintf(fp, "F%c18", b[1]); // LD ST, Vx
    }
    else if(strlen(a)==1 && a[0]=='F' && strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
    {
        fprintf(fp, "F%c29", b[1]); // LD F, Vx
    }
    else if(strlen(a)==1 && a[0]=='B' && strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
    {
        fprintf(fp, "F%c33", b[1]); // LD B, Vx
    }
    else if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) && strlen(b)==2 && b[0]=='D' && b[1]=='T')
    {
        fprintf(fp, "F%c07", a[1]); // LD Vx, DT
    }
    else if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) && strlen(b)==1 && b[0]=='I')
    {
        fprintf(fp, "F%c65", a[1]); // LD Vx, I
    }
    else if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) && strlen(b)==1 && b[0]=='K')
    {
        fprintf(fp, "F%c0A", a[1]); // LD Vx, K
    }
    else if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) && strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
    {
        fprintf(fp, "8%c%c0", a[1], b[1]); // LD Vx, Vy
    }
    else if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) && (flag=strncmp(b, "0x", 2))==0)
    {
        unsigned nibs = 2;
        char* nib = b+2;
        if(strlen(nib) != nibs)
        {
            return errori(iname, address, nibs);
        }
        else
        {
            fprintf(fp, "6%c%s", a[1], nib); // LD Vx, byte
        }
    }
    else if(strlen(a)==1 && a[0]=='I' && strlen(b)==2 && b[0]=='V' && isalpha(b[1]))
    {
        fprintf(fp, "F%c55", b[1]); // LD I, Vx
    }
    else if(strlen(a)==1 && a[0]=='I')
    {
        int paddress = find(l, b);
        if(paddress == -1)
        {
            return errora(iname, address, b);
        }
        fprintf(fp, "A%X", paddress); // LD I, ad
    }
    else // handle errors
    {
        if(flag)
        {
            return errorb(iname, address);
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void or(FILE* fp, char* o)             // 7
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%c1", a[1], b[1]); // OR Vx, Vy
}                                      //

void ret(FILE* fp)                     // 8
{                                      //
    fprintf(fp, "00EE");               // RET
}                                      //

int rnd(char* iname, FILE* fp, char* o, int address)
{                                                      // 9
    char* a = strtok(o, delima);                       //
    char* b = strtok(NULL, delimb);                    //
    if(strncmp(b, "0x", 2) == 0)                       //
    {                                                  //
        unsigned nibs = 2;                                  //
        char* nib = b + strlen(b) - nibs;              //
        if(index(nib, 'x'))                            //
        {                                              //
            return errori(iname, address, nibs);       //
        }                                              //
        fprintf(fp, "C%c%c%c", a[1], nib[0], nib[1]);  // RND Vx, byte
    }
    else
    {
        return errorb(iname, address);
    }
    return 0;
}

int se(char* iname, FILE* fp, char* o, int address)       // 10 SE Vx, Vy
{                                                         //    SE Vx, byte
    char* a = strtok(o, delima);                          //
    char* b = strtok(NULL, delimb);                       //
    if(b[0]=='V')                                         //
        fprintf(fp, "5%c%c0", a[1], b[1]);                //
    else                                                  //
    {                                                     //
        if(strncmp(b, "0x", 2) == 0)                      //
        {                                                 //
            int nibs = 2;                                 //
            char* nib = b + strlen(b) - nibs;             //
            if(index(nib, 'x'))                           //
            {                                             //
                return errori(iname, address, nibs);      //
            }                                             //
            fprintf(fp, "3%c%c%c", a[1], nib[0], nib[1]); //
        }                                                 //
        else                                              //
        {                                                 //
            return errorb(iname, address);                //
        }                                                 //
    }                                                     //
    return 0;                                             //
}                                                         //

void shl(FILE* fp, char* o)            // 11
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%cE", a[1], b[1]); // SHL Vx, Vy
}                                      //

void shr(FILE* fp, char* o)            // 12
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%c6", a[1], b[1]); // SHR Vx, Vy
}                                      //

void skp(FILE* fp, char* o)            // 14
{                                      //
    char* a = strtok(o, delima);       //
    fprintf(fp, "E%c9E", a[1]);        // SKP Vx
}                                      //

void skpn(FILE* fp, char* o)           // 13
{                                      //
    char* a = strtok(o, delima);       //
    fprintf(fp, "E%cA1", a[1]);        // SKNP Vx
}                                      //

int sne(char* iname, FILE* fp, char* o, int address)  // 15 SNE Vx, Vy
{                                                     //    SNE Vx, byte
    char* a = strtok(o, delima);                      //
    char* b = strtok(NULL, delimb);                   //
    if(b[0]=='V')                                     //
    {                                                 //
        fprintf(fp, "9%c%c0", a[1], b[1]);            //
    }                                                 //
    else                                              //
    {                                                 //
        if(strncmp(b, "0x", 2) == 0)                  //
        {                                             //
            fprintf(fp, "4%c%c%c", a[1], b[2], b[3]); //
        }                                             //
        else                                          //
        {                                             //
            return errorb(iname, address);            //
        }                                             //
    }                                                 //
    return 0;                                         //
}                                                     //

void sub(FILE* fp, char* o)            // 16
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%c5", a[1], b[1]); // SUB Vx, Vy
}                                      //

void subn(FILE* fp, char* o)           // 17
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%c7", a[1], b[1]); // SUBN Vx, Vy
}                                      //

void xor(FILE* fp, char* o)            // 18
{                                      //
    char* a = strtok(o, delima);       //
    char* b = strtok(NULL, delimb);    //
    fprintf(fp, "8%c%c3", a[1], b[1]); // XOR Vx, Vy
}                                      //
