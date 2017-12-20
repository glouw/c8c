#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#define REGS 16
#define len(a) ((int) (sizeof(a) / sizeof(*a)))

FILE* in;
FILE* out;

int now;
int rp;

int nline = 1;

char* names[REGS];

char lbuff[512];

void print(char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(out, msg, args);
    fprintf(out, "\n");
    va_end(args);
}

void bomb(char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "error: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

int streql(char* a, char* b)
{
    if(a == NULL || b == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

void kill()
{
    for(int i = 0; i < rp; i++)
        free(names[i]);
}

void dump()
{
    for(int i = 0; i < REGS; i++)
        print("%1X: %s", i, names[i]);
    print("--");
}

void buff()
{
    static int i;
    lbuff[i++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        i = 0;
        print("%s", lbuff);
    }
}

void next()
{
    now = fgetc(in);
    buff();
}

void spin()
{
    while(isspace(now))
        next();
}

void match(const char ch)
{
    if(ch != now)
        bomb("expected %c", ch);
    next();
}

void shutdown()
{
    dump();
    kill();
    if(in)
        fclose(in);
    if(out)
        fclose(out);
}

void init()
{
    in = fopen("tests/test.c8", "r");
    out = stdout;
    atexit(shutdown);
    next();
    spin();
}

int endexpr()
{
    return now == ';' || now == ',' || now == ')';
}

char* gop()
{
    spin();
    char* op = malloc(128);
    int i = 0;
    while(!isalnum(now) && !isspace(now) && !endexpr())
    {
        op[i++] = now;
        next();
    }
    op[i] = '\0';
    return op;
}

char* gterm()
{
    spin();
    char* name = malloc(128);
    int i = 0;
    while(isalnum(now) && !isspace(now) && !endexpr())
    {
        name[i++] = now;
        next();
    }
    name[i] = '\0';
    return name;
}

int deffed(char* name)
{
    for(int i = 0; i < rp; i++)
        if(streql(name, names[i]))
            return i;
    bomb("name '%s' not defined\n", name);
    return 0;
}

int ndeffed(char* name)
{
    for(int i = 0; i < len(names); i++)
        if(streql(name, names[i]))
            bomb("name '%s' already defined", name);
    return 1;
}

void dop(char* op)
{
    if(streql(op, "+"))
        print("\tADD V%1X,V%1X", rp - 1, rp);
    else
    if(streql(op, "-"))
        print("\tSUB V%1X,V%1X", rp - 1, rp);
    else
        bomb("operation '%s' not recognized", op);
}

void gen(char* tok, char* op)
{
    if(tok && op)
    {
        // LVALUE
        if(isalpha(tok[0]))
        {
            print("\tLD V%1X,V%1X", rp, deffed(tok));
        }
        // RVALUE
        else
            print("\tLD V%1X,0x%X", rp, strtol(tok, NULL, 0));
        dop(op);
    }
    else
    if(tok)
    {
        if(strlen(tok) == 0)
            print("\tLD V%1X,0x0", rp);
        // LVALUE
        else
        if(isalpha(tok[0]))
            print("\tLD V%1X,V%1X", rp, deffed(tok));
        // RVALUE
        else
            print("\tLD V%1X,0x%X", rp, strtol(tok, NULL, 0));
    }
    else
        bomb("no token found");
}

int isfix(char* fix)
{
    return streql("++", fix) || streql("--", fix);
}


typedef struct
{
    char* pref;
    char* term;
    char* posf;
}
Term;

Term tnew()
{
    Term term;
    term.pref = gop();
    term.term = gterm();
    term.posf = gop();
    return term;
}

void tdel(Term term)
{
    free(term.pref);
    free(term.term);
    free(term.posf);
}

void tprint(Term term)
{
    printf("pref: %s\n", term.pref);
    printf("term: %s\n", term.term);
    printf("posf: %s\n\n", term.posf);
}

// a op b op a ...
void expression()
{
    Term a = tnew();
    /* Work with term a here */
    tprint(a);
    spin();
    while(!endexpr())
    {
        rp++;
        char* op = isfix(a.posf) ? gop() : strdup(a.posf); /* GNU99 for strdup - fix pls for C99 */
        puts(op);
        Term b = tnew();
        /* Work with op and term b here */
        tprint(b);
        free(op);
        tdel(a);
        a = b;
        rp--;
    }
    tdel(a);
}

void ident()
{
    match('l');
    match('e');
    match('t');
    spin();
    char* name = gterm();
    ndeffed(name);
    spin();
    match('=');
    expression();
    match(';');
    names[rp++] = name;
}

int main()
{
    init();
    while(now != EOF)
    {
        ident();
        // For the new line.
        spin();
    }
    exit(0);
}
