#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

FILE* in;
FILE* out;

// Character in the feed.
int now;

// Register Pointer.
int rp;

// Line number.
int nline = 1;

char* names[16];

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
    fprintf(stderr, "\x1B[31merror: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\x1B[0m\n");
    va_end(args);
    exit(1);
}

// String equal.
int seql(char* a, char* b)
{
    if(a == NULL || b == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

// String duplicate.
char* sdup(char* s)
{
    char* dup = malloc(strlen(s) + 1);
    return strcpy(dup, s);
}

void kill()
{
    for(int i = 0; i < rp; i++)
        free(names[i]);
}

void dump()
{
    for(unsigned i = 0; i < sizeof(names) / sizeof(*names); i++)
        print("%1X: %s", i, names[i]);
    print("--");
}

// Buffers a line and prints. Handy for mixed asm output.
void buff()
{
    static char lbuff[512];
    static int i;
    lbuff[i++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        i = 0;
        // Mageneta color print.
        print("\x1B[35m;%s\x1B[0m", lbuff);
    }
}

void next()
{
    now = fgetc(in);
    buff();
}

// Skips white space.
void skip()
{
    while(isspace(now))
        next();
}

void match(const char ch)
{
    if(ch != now)
        bomb("expected '%c'", ch);
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
    skip();
}

// Returns the register pointer of a defined name in question.
int deffed(char* name)
{
    for(int i = 0; i < rp; i++)
        if(seql(name, names[i]))
            return i;
    bomb("name '%s' not defined\n", name);
    return 0;
}

// Returns 1 if a name is not defined. Exits if the name is defined.
int ndeffed(char* name)
{
    for(unsigned i = 0; i < sizeof(names) / sizeof(*names); i++)
        if(seql(name, names[i]))
            bomb("name '%s' already defined", name);
    return 1;
}

char* gname()
{
    skip();
    char* name = malloc(128);
    int i = 0;
    while(isalpha(now))
    {
        name[i++] = now;
        next();
    }
    name[i] = '\0';
    return name;
}

char* gnum()
{
    skip();
    if(!isdigit(now))
        bomb("expected value");
    char* num = malloc(128);
    int i = 0;
    while(isdigit(now))
    {
        num[i++] = now;
        next();
    }
    num[i] = '\0';
    return num;
}

int endexpr(char e)
{
    return e == ';'
        || e == ')'
        || e == ',';
}

char* gop()
{
    skip();
    char* op = malloc(128);
    int i = 0;
    while(!isalnum(now) && !isspace(now) && !endexpr(now))
    {
        op[i++] = now;
        next();
    }
    op[i] = '\0';
    return op;
}

/* Prefix and post fix operators. */
void _dec(char* lv)
{
    if(!isalpha(lv[0]))
        bomb("'%s' must be lvalue", lv);
    print("\tSUB V%1X,0x01", deffed(lv));
}

void _inc(char* lv)
{
    if(!isalpha(lv[0]))
        bomb("'%s' must be lvalue", lv);
    print("\tADD V%1X,0x01", deffed(lv));
}

/* Unary operators. */
void _neg(char* lv)
{
    if(!isalpha(lv[0]))
        bomb("'%s' must be lvalue", lv);
    print("\tXOR V%1X,0xFF", rp);
    print("\tADD V%1X,0x01", rp);
}

void _inv()
{
    print("\tXOR V%1X,0xFF", rp);
}

// Loads a name (l-value).
void lname(char* lv)
{
    print("\tLD V%1X,V%1X", rp, deffed(lv));
}

// Loads a number (r-value).
void lnum(char* rv)
{
    print("\tLD V%1X,0x%X", rp, strtol(rv, NULL, 0));
}

int ispostfix(char* op)
{
    return strstr(op, "++")
        || strstr(op, "--");
}

int isassign(char* op)
{
    return strstr(op, "=") && !strstr(op, "==");
}

int ischain(char* op)
{
    return strstr(op, "+")
        || strstr(op, "-")
        || strstr(op, "&")
        || strstr(op, "^")
        || strstr(op, "|")
        || strstr(op, "<")
        || strstr(op, ">");
}

int isname(char* lv)
{
    return isalpha(lv[0]);
}

// L-value check. Passes in an additional op for diagnostics.
void lcheck(char* lv, char* op)
{
    if(!isname(lv))
        bomb("'%s' must be lvalue for operator '%s'", lv, op);
}

void expression();

char* term()
{
    skip();
    if(now == '~')
    {
        match('~');
        char* lv = term();
        _inv();
        return lv;
    }
    else
    if(now == '+')
    {
        match('+');
        if(now == '+')
        {
            match('+');
            char* lv = gname();
            lcheck(lv, "++");
            _inc(lv);
            lname(lv);
            return lv;
        }
        return sdup("++");
    }
    else
    if(now == '-')
    {
        match('-');
        if(now == '-')
        {
            match('-');
            char* lv = gname();
            lcheck(lv, "--");
            _dec(lv);
            lname(lv);
            return lv;
        }
        char* lv = term();
        _neg(lv);
        return lv;
    }
    else
    if(now == '(')
    {
        match('(');
        expression();
        match(')');
        return sdup(")");
    }
    else
    if(isalpha(now))
    {
        char* lv = gname();
        lname(lv);
        return lv;
    }
    else
    if(isdigit(now))
    {
        char* rv = gnum();
        lnum(rv);
        return rv;
    }
    // Catches end of expression chars.
    else
    {
        char na[] = { now, '\0'};
        print(";warning: unknown char '%s' found in term()", na);
        return sdup(na);
    }
}

/* C++ reserverse these names, so an undercore is used. */

void _add()
{
    print("\tADD V%1X,V%1X", rp - 1, rp);
}

void _sub()
{
    print("\tSUB V%1X,V%1X", rp - 1, rp);
}

void _shl()
{
    print("\tSHL V%1X,V%1X", rp - 1, rp);
}

void _shr()
{
    print("\tSHR V%1X,V%1X", rp - 1, rp);
}

void _and()
{
    print("\tAND V%1X,V%1X", rp - 1, rp);
}

void _or()
{
    print("\tOR  V%1X,V%1X", rp - 1, rp);
}

void _xor()
{
    print("\tXOR V%1X,V%1X", rp - 1, rp);
}

void _addeql(char* lv)
{
    print("\tADD V%1X,V%1X", deffed(lv), rp);
    _add();
}

void _subeql(char* lv)
{
    print("\tSUB V%1X,V%1X", deffed(lv), rp);
    _sub();
}

void _shleq(char* lv)
{
    print("\tSHL V%1X,V%1X", deffed(lv), rp);
    _shl();
}

void _shreq(char* lv)
{
    print("\tSHR V%1X,V%1X", deffed(lv), rp);
    _shr();
}

void _andeq(char* lv)
{
    print("\tAND V%1X,V%1X", deffed(lv), rp);
    _and();
}

void _oreq(char* lv)
{
    print("\tOR  V%1X,V%1X", deffed(lv), rp);
    _or();
}

void _xoreq(char* lv)
{
    print("\tXOR V%1X,V%1X", deffed(lv), rp);
    _xor();
}

// Do chain operator.
void dcop(char* op)
{
    if(seql(op, "+"))
        _add();
    else
    if(seql(op, "-"))
        _sub();
    else
    if(seql(op, "<<"))
        _shl();
    else
    if(seql(op, ">>"))
        _shr();
    else
    if(seql(op, "&"))
        _and();
    else
    if(seql(op, "|"))
        _or();
    else
    if(seql(op, "^"))
        _xor();
    else
        bomb("unknown operator '%s' %c", op, now);
}

// Do assignment operator.
void daop(char* op, char* lv)
{
    if(seql(op, "+="))
        _addeql(lv);
    else
    if(seql(op, "-="))
        _subeql(lv);
    else
    if(seql(op, "<<="))
        _shleq(lv);
    else
    if(seql(op, ">>="))
        _shreq(lv);
    else
    if(seql(op, "&="))
        _andeq(lv);
    else
    if(seql(op, "|="))
        _oreq(lv);
    else
    if(seql(op, "^="))
        _xoreq(lv);
    else
    if(seql(op, "++"))
        _inc(lv);
    else
    if(seql(op, "--"))
        _dec(lv);
    else
        bomb("unknown lvalue operator '%s' on lvalue '%s'", op, lv);
}

// A op B op A ... and so on
void expression()
{
    char* lv = term();
    while(!endexpr(now))
    {
        rp++;
        char* op = gop();
        // Postfix operators.
        if(ispostfix(op))
        {
            lcheck(lv, op);
            daop(op, lv);
            free(op);
            op = gop();
        }
        // Assignment operators.
        char* rv = term();
        if(isassign(op))
        {
            lcheck(lv, op);
            daop(op, lv);
        }
        // Chain operators.
        else
        if(ischain(op))
            dcop(op);
        free(op);
        free(lv);
        lv = rv;
        rp--;
    }
    free(lv);
}

// Identifier.
void ident()
{
    match('l');
    match('e');
    match('t');
    match(' ');
    // For any whitespace.
    char* name = gname();
    ndeffed(name);
    skip();
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
        skip();
    }
    exit(0);
}
