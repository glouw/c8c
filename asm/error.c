#include "public.h"

/* reading undeclared label */
int errora(char* iname, int address, char* label)
{
    int linenum = (address-0x0200) / 2;
    fprintf(stderr, "%s: line %d: error: %s label undeclared\n",
            iname, linenum, label);
    return 'a';
}

/* missing 0x */
int errorb(char* iname, int address)
{
    int linenum = (address-0x0200) / 2;
    fprintf(stderr, "%s: line %d: error: immediate value missing 0x prefix\n",
            iname, linenum);
    return 'b';
}

/* missing semicolon */
int errorc(char* iname, int address) 
{
    int linenum = (address-0x0200)/2;
    fprintf(stderr, "%s: line %d: error: missing semicolon\n",
            iname, linenum);
    return 'c';
}

/* label with tab */
int errord(char* iname, int address)
{
    int linenum = (address-0x0200)/2;
    fprintf(stderr, "%s: line %d: error: label with tab\n",
            iname, linenum);
    return 'd';
}

/* missing tab */
int errore(char* iname, int address)
{
    int linenum = (address-0x0200)/2;
    fprintf(stderr, "%s: line %d: error: missing tab\n",
            iname, linenum);
    return 'e';
}

/* label already exists */
int errorf(char* iname, int address, int paddress, char* l)
{
    int linenum = (address-0x0200)/2; // line number
    int plinenum = (paddress-0x0200)/2; // past line number
    fprintf(stderr, "%s: line %d: error: label %s already exists at line %d\n",
            iname, linenum, l, plinenum);
    return 'f';
}

/* not a valid mnem */
int errorg(char* iname, int address, char* m)
{
    int linenum = (address-0x0200)/2;
    if(*m == '\n')
        fprintf(stderr, "%s: line %d: error: no mnem specified\n",
                iname, linenum);
    else
        fprintf(stderr, "%s: line %d: error: %s is not valid mnem\n",
                iname, linenum, m);
    return 'g';
}

/* missing entry point ename that must be MAIN */
int errorh(char* iname, char* ename)
{
    fprintf(stderr, "%s: fatal error: cannot find entry point label %s\n",
            iname, ename);
    return 'h';
}

/* too few nibbles */
int errori(char* iname, int address, int nibs)
{
    int linenum = (address-0x0200)/2;
    fprintf(stderr, "%s: line %d: error: immediate value must have %d nibbles\n",
            iname, linenum, nibs);
    return 'i';
}

