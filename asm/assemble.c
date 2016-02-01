/* int functions return 0; if there are no aerrors */

#include "public.h"

static char* arg = ", \t\0";

static int add(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
        fprintf(p.out, "8%c%c4", a[1], b[1]); // ADD Vx, Vy
    else
    if(strlen(a)==1 && a[0]=='I' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c1E", b[1]); // ADD I, Vx
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strncmp(b,"0x",2)==0 && strlen(b+2)==2)
         fprintf(p.out, "7%c%s", a[1], b+2); // ADD Vx, byte
    else
        return 1;
    return 0;
}

static int and(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 &&
       a[0]=='V' && isxdigit(a[1]) && strlen(b)==2 &&
       b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c2", a[1], b[1]); // AND Vx, Vy
    else
        return 1;
    return 0;
}

static int call(struct pack p)
{
    int paddress = -1;
    char* a = strtok(p.o, arg);
    if((paddress = find(p.list, a)) != -1)
        fprintf(p.out, "2%X", paddress); // CALL ad
    else
        return 2;
    return 0;
}

static int cls(struct pack p)
{
    (void)p;
    fprintf(p.out, "00E0"); // CLS
    return 0;
}

static int drw(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    char* c = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) &&
       strlen(c)==3 && strncmp(c, "0x", 2)==0)
         fprintf(p.out, "D%c%s", a[1], c+2); // DRW Vx, Vy, nib
    else
        return 1;
    return 0;
}

static int jump(struct pack p) // 5
{
    int paddress = -1;
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(a && strlen(a)==2 && a[0]=='V' && a[1]=='0' &&
       b && (paddress=find(p.list, b)) != -1)
        fprintf(p.out, "B%X", paddress); // JUMP V0, ad
    else
    if(a && (paddress=find(p.list, a)) != -1)
        fprintf(p.out, "1%X", paddress); // JUMP ad
    else
        return 2;
    return 0;
}

static int ld(struct pack p) // 6
{
    int paddress = -1;
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='D' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c15", b[1]); // LD DT, Vx
    else
    if(strlen(a)==2 && a[0]=='S' && a[1]=='T' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c18", b[1]); // LD ST, Vx
    else
    if(strlen(a)==1 && a[0]=='F' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c29", b[1]); // LD F, Vx
    else
    if(strlen(a)==1 && a[0]=='B' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c33", b[1]); // LD B, Vx
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='D' && b[1]=='T')
         fprintf(p.out, "F%c07", a[1]); // LD Vx, DT
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==1 && b[0]=='I')
         fprintf(p.out, "F%c65", a[1]); // LD Vx, I
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==1 && b[0]=='K')
         fprintf(p.out, "F%c0A", a[1]); // LD Vx, K
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
        fprintf(p.out, "8%c%c0", a[1], b[1]); // LD Vx, Vy
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
      (strlen(b)==4 && strncmp(b, "0x", 2))==0)
        fprintf(p.out, "6%c%s", a[1], b+2); // LD Vx, byte
    else
    if(strlen(a)==1 && a[0]=='I' &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "F%c55", b[1]); // LD I, Vx
    else
    if(strlen(a)==1 && a[0]=='I')
    {
       if((paddress = find(p.list, b)) != -1)
           fprintf(p.out, "A%X", paddress); // LD I, ad
       else
           return 2;
    }
    else
        return 1;
    return 0;
}

static int or(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c1", a[1], b[1]); // OR Vx, Vy
    else
        return 1;
    return 0;
}

static int ret(struct pack p)
{
    (void)p; // no need to unpack, nothing is needed
    fprintf(p.out, "00EE");
    return 0;
}

static int rnd(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0)
         fprintf(p.out, "C%c%s", a[1], b+2); // RND Vx, byte
    else
        return 1;
    return 0;
}

static int se(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
        fprintf(p.out, "5%c%c0", a[1], b[1]); // SE Vx, Vy
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0)
         fprintf(p.out, "3%c%s", a[1], b+2); // Se Vx, byte
    else
        return 1;
    return 0;
}

static int shl(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%cE", a[1], b[1]); // SHL Vx, Vy
    else
        return 1;
    return 0;
}

static int shr(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c6", a[1], b[1]); // SHR Vx, Vy
    else
        return 1;
    return 0;
}

static int skp(struct pack p)
{
    char* a = strtok(p.o, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(p.out, "E%c9E", a[1]); // SKP Vx
    else
        return 1;
    return 0;
}

static int skpn(struct pack p)
{
    char* a = strtok(p.o, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
        fprintf(p.out, "E%cA1", a[1]); // SKNP Vx
    else
        return 1;
    return 0;
}

static int sne(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
        fprintf(p.out, "9%c%c0", a[1], b[1]); // SNE Vx, Vy
    else
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==4 && strncmp(b, "0x", 2)==0)
        fprintf(p.out, "4%c%s", a[1], b+2); // SNE Vx, byte
    else
        return 1;
    return 0;
}

static int sub(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c5", a[1], b[1]); // SUB Vx, Vy
    else
        return 1;
    return 0;
}

static int subn(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c7", a[1], b[1]); // SUBN Vx, Vy
    else
        return 1;
    return 0;
}

static int xor(struct pack p)
{
    char* a = strtok(p.o, arg);
    char* b = strtok(NULL, arg);
    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
         fprintf(p.out, "8%c%c3", a[1], b[1]); // XOR Vx, Vy
    else
        return 1;
    return 0;
}

static int (*mnemfp[])(struct pack) =
{
    add, and, call, cls, drw, jump, ld, or, ret, ret, rnd, se, shl,
    shr, skp, skpn, sne, sub, subn, xor
};

int assemble(int mnum, struct pack p)
{
    int (*_assemble)(struct pack) = mnemfp[mnum];
    int aerror = _assemble(p);
    if(!aerror)
        fputc('\n', p.out);
    return aerror;
}
