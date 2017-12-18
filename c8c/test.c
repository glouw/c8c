#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

FILE* i;
FILE* o;

int now;
int rp;

char names[16][128];

char* operator = names[14];
char* lastname = names[15];

int streql(char* a, char* b)
{
    return strcmp(a, b) == 0;
}

void print()
{
    for(int i = 0; i < 16; i++)
        printf("%1X: %s\n", i, names[i]);
}

void spin()
{
    if(isspace(now = fgetc(i))) spin();
}

void match(const char ch)
{
    if(ch != now)
    {
        fprintf(stderr, "error: expected %c\n", ch);
        exit(1);
    }
    spin();
}

void shutdown()
{
    print();
    if(i) fclose(i);
    if(o) fclose(o);
}

void init()
{
    i = fopen("tests/test.c8", "r");
    o = stdout;
    atexit(shutdown);
    spin();
}

int isname()
{
    return isalnum(now) || now == '_';
}

int isop()
{
    return now == '+' || now == '-';
}

void getop()
{
    int i = 0;
    while(isop())
    {
        operator[i++] = now;
        spin();
    }
    operator[i] = '\0';
}

void getname()
{
    int i = 0;
    while(isname())
    {
        lastname[i++] = now;
        spin();
    }
    lastname[i] = '\0';
}

int isnum()
{
    return isdigit(now) || isxdigit(now) || tolower(now) == 'x';
}

int getnumber()
{
    char value[128];
    int i = 0;
    while(isnum())
    {
        value[i++] = now;
        spin();
    }
    value[i] = '\0';
    return strtol(value, NULL, 0);
}

void vload()
{
    fprintf(o, "\tLD V%1X,0x%X\n", rp, getnumber());
}

void nload(const int r)
{
    fprintf(o, "\tLD V%1X,V%1X\n", rp, r);
}

int find()
{
    for(int i = 0; i < rp; i++)
        if(streql(names[i], lastname))
            return i;
    fprintf(stderr, "error: %s: name not found\n", lastname);
    exit(1);
}

void posterm()
{
    if(streql(operator, "+"))
        fprintf(o, "\tADD V%1X,V%X\n", rp - 1, rp);
    else
    if(streql(operator, "++"))
        fprintf(o, "\tADD V%1X,0x1\n", find());
    else
    {
        fprintf(stderr, "error: %s: unknown post-term operator\n", operator);
        exit(1);
    }
}

void preinc()
{
    fprintf(o, "\tADD V%1X,0x1\n", find());
}

void twoscomp()
{
    fprintf(o, "\tXOR V%1X,0xFF\n", rp);
    fprintf(o, "\tADD V%1X,0x01\n", rp);
}

void preop()
{
    if(streql(operator, "++"))
    {
        getname();
        preinc();
        nload(find());
    }
    else
    if(streql(operator, "-"))
    {
        getname();
        nload(find());
        twoscomp();
    }
    else
    if(streql(operator, "+"))
    {
        getname();
        nload(find());
    }
    else
    {
        fprintf(stderr, "error: %s: unknown pre-term operator\n", operator);
        exit(1);
    }
}

void term()
{
    if(isalpha(now))
    {
        getname();
        nload(find());
    }
    else
    if(isop())
    {
        getop();
        preop();
    }
    else vload();
}

void expression()
{
    term();
    while(now != ';')
    {
        rp++;
        getop();
        term();
        posterm();
        rp--;
    }
}

void ident()
{
    match('l');
    match('e');
    match('t');
    getname();
    match('=');
    strcpy(names[rp], lastname);
    expression();
    rp++;
    match(';');
}

int main()
{
    init();
    while(now != EOF)
        ident();
    exit(0);
}
