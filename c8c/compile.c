#include "compile.h"

#include <stdlib.h>
#include <ctype.h>

#include "io.h"
#include "str.h"
#include "util.h"

static int rp;

static int label;

static char* names[16];

static char* term();

static void expression();

static void kill()
{
    for(int i = 0; i < rp; i++)
        free(names[i]);
}

static void dump()
{
    for(unsigned i = 0; i < len(names); i++)
        io.print("%1X: %s", i, names[i]);
    io.print("--");
}

static void shutdown()
{
    dump();
    kill();
}

static void init()
{
    atexit(shutdown);
}

static void lcheck(const char* lv, const char* op)
{
    if(!str.isname(lv))
        io.bomb("'%s' must be lvalue for operator '%s'", lv, op);
}

static int gdef(char* name)
{
    for(int i = 0; i < rp; i++)
        if(str.eql(name, names[i]))
            return i;
    io.bomb("name '%s' not defined\n", name);
    return 0;
}

static int isndef(char* name)
{
    for(unsigned i = 0; i < len(names); i++)
        if(str.eql(name, names[i]))
            io.bomb("name '%s' already defined", name);
    return 1;
}

// Chain operators.
static void _add() { io.print("\tADD V%1X,V%1X", rp - 1, rp); }
static void _sub() { io.print("\tSUB V%1X,V%1X", rp - 1, rp); }
static void _shl() { io.print("\tSHL V%1X,V%1X", rp - 1, rp); }
static void _shr() { io.print("\tSHR V%1X,V%1X", rp - 1, rp); }
static void _and() { io.print("\tAND V%1X,V%1X", rp - 1, rp); }
static void _or()  { io.print("\tOR  V%1X,V%1X", rp - 1, rp); }
static void _xor() { io.print("\tXOR V%1X,V%1X", rp - 1, rp); }
static void _pad() {
    io.print("\tJP L%1X", label);
    io.print("\tLD V%1X,0x00", rp - 1);
    io.print("\tJP L%1X", label + 1);
    io.print("L%1X:", label);
    io.print("\tLD V%1X,0x01", rp - 1);
    io.print("L%1X:", label + 1);
    label += 2;
}
static void _neql()
{
    io.print("\tSE V%1X,V%1X", rp - 1, rp);
    _pad();
}
static void _eql()
{
    io.print("\tSNE V%1X,V%1X", rp - 1, rp);
    _pad();
}
static void _lt()
{
    io.print("\tSUBN V%1X,V%1X", rp - 1, rp);
    io.print("\tLD V%1X,VF", rp - 1);
}
static void _gt()
{
    io.print("\tSUB V%1X,V%1X", rp - 1, rp);
    io.print("\tLD V%1X,VF", rp - 1);
}
static void _lteql()
{
    _gt();
}
static void _gteql()
{
    _lt();
}
// L-value unary operators.
static void _dec(char* lv) { io.print("\tSUB V%1X,0x01", gdef(lv)); }
static void _inc(char* lv) { io.print("\tADD V%1X,0x01", gdef(lv)); }

// L-value and R-value unary operators.
static void _neg()
{
    io.print("\tXOR V%1X,0xFF", rp);
    io.print("\tADD V%1X,0x01", rp);
}
static void _inv() { io.print("\tXOR V%1X,0xFF", rp); }
static void _not()
{
    rp++;
    io.print("\tLD V%1X,0x00", rp);
    _neql();
    rp--;
}
// L-value operators.
static void _addeql(char* lv) { io.print("\tADD V%1X,V%1X", gdef(lv), rp); _add(); }
static void _subeql(char* lv) { io.print("\tSUB V%1X,V%1X", gdef(lv), rp); _sub(); }
static void _shleq (char* lv) { io.print("\tSHL V%1X,V%1X", gdef(lv), rp); _shl(); }
static void _shreq (char* lv) { io.print("\tSHR V%1X,V%1X", gdef(lv), rp); _shr(); }
static void _andeq (char* lv) { io.print("\tAND V%1X,V%1X", gdef(lv), rp); _and(); }
static void _oreq  (char* lv) { io.print("\tOR  V%1X,V%1X", gdef(lv), rp); _or();  }
static void _xoreq (char* lv) { io.print("\tXOR V%1X,V%1X", gdef(lv), rp); _xor(); }
static void _lname (char* lv) { io.print("\tLD  V%1X,V%1X", rp, gdef(lv));         }

// R-value operators.
static void _lnum  (char* rv) { io.print("\tLD V%1X,0x%X", rp, strtol(rv, NULL, 0)); }

static char* inc()
{
    io.match('+');
    char* lv = io.gname();
    lcheck(lv, "++");
    _inc(lv);
    _lname(lv);
    return lv;
}

static char* not()
{
    io.match('!');
    char* lv = term();
    _not();
    return lv;
}

static char* pos()
{
    io.match('+');
    if(io.peek() == '+')
        return inc();
    return str.dup("+");
}

static char* inv()
{
    io.match('~');
    char* lv = term();
    _inv();
    return lv;
}

static char* dec()
{
    io.match('-');
    char* lv = io.gname();
    lcheck(lv, "--");
    _dec(lv);
    _lname(lv);
    return lv;
}

static char* neg()
{
    io.match('-');
    if(io.peek() == '-')
        return dec();
    char* lv = term();
    _neg();
    return lv;
}

static char* fexp()
{
    io.match('(');
    expression();
    io.match(')');
    return str.dup(")");
}

static char* lnum()
{
    char* rv = io.gnum();
    _lnum(rv);
    return rv;
}

static char* lname()
{
    char* lv = io.gname();
    _lname(lv);
    return lv;
}

static char* term()
{
    io.skip();
    return
    io.peek()== '~'    ? inv  () :
    io.peek()== '+'    ? pos  () :
    io.peek()== '!'    ? not  () :
    io.peek()== '-'    ? neg  () :
    io.peek()== '('    ? fexp () :
    isalpha(io.peek()) ? lname() :
    isdigit(io.peek()) ? lnum () :
    io.peeks();
}

// Do chain operator.
static void dcop(char* op)
{
    str.eql(op, "+" )  ? _add  () :
    str.eql(op, "-" )  ? _sub  () :
    str.eql(op, "<<")  ? _shl  () :
    str.eql(op, ">>")  ? _shr  () :
    str.eql(op, "&" )  ? _and  () :
    str.eql(op, "|" )  ? _or   () :
    str.eql(op, "^" )  ? _xor  () :
    str.eql(op, "==" ) ? _eql  () :
    str.eql(op, "!=" ) ? _neql () :
    str.eql(op, "<" )  ? _lt   () :
    str.eql(op, ">" )  ? _gt   () :
    str.eql(op, "<=" ) ? _lteql() :
    str.eql(op, ">=" ) ? _gteql() :
    io.bomb("unknown operator '%s'", op);
}

// Do assignment operator.
static void daop(char* op, char* lv)
{
    lcheck(lv, op);
    str.eql(op, "+=" ) ? _addeql(lv) :
    str.eql(op, "-=" ) ? _subeql(lv) :
    str.eql(op, "<<=") ? _shleq (lv) :
    str.eql(op, ">>=") ? _shreq (lv) :
    str.eql(op, "&=" ) ? _andeq (lv) :
    str.eql(op, "|=" ) ? _oreq  (lv) :
    str.eql(op, "^=" ) ? _xoreq (lv) :
    str.eql(op, "++" ) ? _inc   (lv) :
    str.eql(op, "--" ) ? _dec   (lv) :
    io.bomb("unknown lvalue operator '%s' on lvalue '%s'", op, lv);
}

// Do postfix operator.
static char* dpop(char* lv, char* op)
{
    lcheck(lv, op);
    daop(op, lv);
    free(op);
    return io.gop();
}

// A op B op A ... and so on
static void expression()
{
    char* lv = term();
    while(!io.isendexpr())
    {
        rp++;
        char* op = io.gop();
        // Postfix.
        if(str.ispostfix(op))
            op = dpop(lv, op);
        // Assignment or chain operator.
        char* rv = term();
        str.isassign(op) ? daop(op, lv) : str.ischain(op) ? dcop(op) : (void) 0;
        free(op);
        free(lv);
        lv = rv;
        rp--;
    }
    free(lv);
}

// Identifier.
static void ident()
{
    io.match('b');
    io.match('y');
    io.match('t');
    io.match('e');
    io.match(' ');
    // For any whitespace.
    char* name = io.gname();
    isndef(name);
    io.skip();
    io.match('=');
    expression();
    io.match(';');
    names[rp++] = name;
}

static void program()
{
    while(!io.end())
    {
        ident();
        io.skip();
    }
}

const struct compile compile = {
    init, program
};
