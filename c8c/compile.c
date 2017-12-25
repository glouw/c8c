#include "compile.h"

#include "io.h"
#include "str.h"
#include "util.h"

#include <stdlib.h>
#include <ctype.h>

// Variable names.
static int v;

static char* vars[16];

// Sprite names and function names.
static int l;

static char* labels[512];

// Counter for branches.
static int branch;

// BNF syntaxing requires terms and expressions be mutually dependable.
static char* term();

static void expression();

// Removes 'n' elements from the variable names.
static void pop(const int n)
{
    for(int i = 0; i < n; i++)
    {
        v--;
        free(vars[i]);
        vars[i] = NULL;
    }
}

// Removes all variable names.
static void reset()
{
    for(int i = 0; i < v; i++)
    {
        free(vars[i]);
        vars[i] = NULL;
    }
    v = 0;
}

// Prints all variable names.
static void dump()
{
    for(unsigned i = 0; i < len(vars); i++)
        io.print("%02X: %s", i, vars[i]);
}

// Removes all label names.
static void kill()
{
    for(int i = 0; i < l; i++)
    {
        free(labels[i]);
        labels[i] = NULL;
    }
    l = 0;
}

static void shutdown()
{
    dump();
    reset();
    kill();
}

static void init()
{
    atexit(shutdown);
}

// Asserts an L-value.
static void lcheck(const char* lv, const char* op)
{
    if(!str.isname(lv))
        io.bomb("'%s' must be lvalue for operator '%s'", lv, op);
}

// Returns the register number of a defined variable.
static int gdef(char* name)
{
    for(int i = 0; i < v; i++)
        if(str.eql(name, vars[i]))
            return i;
    io.bomb("name '%s' not defined\n", name);
    return 0;
}

// Returns true if a name is already defined, else exits
static int isndef(char* name)
{
    for(unsigned i = 0; i < len(vars); i++)
        if(str.eql(name, vars[i]))
            io.bomb("name '%s' already defined", name);
    for(unsigned i = 0; i < len(labels); i++)
        if(str.eql(name, labels[i]))
            io.bomb("name '%s' already defined", name);
    return 1;
}

// Decerment.
static void _dec(char* lv)
{
    io.print("\tSUB V%1X,0x01", gdef(lv));
}

// Increment.
static void _inc(char* lv)
{
    io.print("\tADD V%1X,0x01", gdef(lv));
}

// Negation.
static void _neg()
{
    io.print("\tXOR V%1X,0xFF", v);
    io.print("\tADD V%1X,0x01", v);
}

// Bitwise inversion.
static void _inv()
{
    io.print("\tXOR V%1X,0xFF", v);
}

// Logical not.
static void _notl()
{
    io.print("\tSNE V%1X,0x00", v);
    io.print("\tLD V%1X,0x01", v);
}

// Addition.
static void _add()
{
    io.print("\tADD V%1X,V%1X", v - 1, v);
}

// Subtraction.
static void _sub()
{
    io.print("\tSUB V%1X,V%1X", v - 1, v);
}

// Shift left.
static void _shl()
{
    io.print("\tSHL V%1X,V%1X", v - 1, v);
}

// Shift right.
static void _shr()
{
    io.print("\tSHR V%1X,V%1X", v - 1, v);
}

// Bitwise and.
static void _and()
{
    io.print("\tAND V%1X,V%1X", v - 1, v);
}

// Bitwise or.
static void _or()
{
    io.print("\tOR V%1X,V%1X", v - 1, v);
}

// Bitwise xor.
static void _xor()
{
    io.print("\tXOR V%1X,V%1X", v - 1, v);
}

// Not equal.
static void _neql()
{
    const int b = branch++;
    io.print("\tSE V%1X,V%1X", v - 1, v);
    io.print("\tJP ELS%d", b);
    io.print("\tLD V%1X,0x01", v - 1);
    io.print("\tJP END%d", b);
    io.print("ELS%d:", b);
    io.print("\tLD V%1X,0x00", v - 1);
    io.print("END%d:", b);
}

// Equal.
static void _eql()
{
    const int b = branch++;
    io.print("\tSE V%1X,V%1X", v - 1, v);
    io.print("\tJP ELS%d", b);
    io.print("\tLD V%1X,0x00", v - 1);
    io.print("\tJP END%d", b);
    io.print("ELS%d:", b);
    io.print("\tLD V%1X,0x01", v - 1);
    io.print("END%d:", b);
}

// Less than.
static void _lt()
{
    io.print("\tSUBN V%1X,V%1X", v - 1, v);
    io.print("\tLD V%1X,VF", v - 1);
}

// Greater than.
static void _gt()
{
    io.print("\tSUB V%1X,V%1X", v - 1, v);
    io.print("\tLD V%1X,VF", v - 1);
}

// Less than or equal to.
static void _lteql()
{
    _gt();
    io.print("\tXOR V%1X,0x01", v - 1);
}

// Greater than or equal to.
static void _gteql()
{
    _lt();
    io.print("\tXOR V%1X,0x01", v - 1);
}

// Copy an R-value to its L-value.
static void _cp()
{
    io.print("\tLD V%1X,V%1X", v - 1, v);
}

// Question mark.
static void _q(const int b)
{
    io.print("\tSNE V%1X,0x00", v);
    io.print("\tJP ELS%d", b);
}

// Colon.
static void _c(const int b)
{
    io.print("\tJP END%d", b);
    io.print("ELS%d", b);
}

// Add equal.
static void _addeql(char* lv)
{
    io.print("\tADD V%1X,V%1X", gdef(lv), v);
    _add();
}

// Minus equal.
static void _subeql(char* lv)
{
    io.print("\tSUB V%1X,V%1X", gdef(lv), v);
    _sub();
}

// Shift left equal.
static void _shleq(char* lv)
{
    io.print("\tSHL V%1X,V%1X", gdef(lv), v);
    _shl();
}

// Shift right equal.
static void _shreq(char* lv)
{
    io.print("\tSHR V%1X,V%1X", gdef(lv), v);
    _shr();
}

// And equal.
static void _andeq(char* lv)
{
    io.print("\tAND V%1X,V%1X", gdef(lv), v);
    _and();
}

// Or equal.
static void _oreq(char* lv)
{
    io.print("\tOR V%1X,V%1X", gdef(lv), v);
    _or();
}

// Xor equal
static void _xoreq(char* lv)
{
    io.print("\tXOR V%1X,V%1X", gdef(lv), v);
    _xor();
}

// Load name.
static void _lname(char* lv)
{
    io.print("\tLD V%1X,V%1X", v, gdef(lv));
}

// Load number.
static void _lnum(char* rv)
{
    io.print("\tLD V%1X,0x%02X", v, strtol(rv, NULL, 0));
}

// Logical short-circuit and.
static void _scandl(const int b)
{
    io.print("\tSNE V%1X,0x00", v);
    io.print("\tJP END%d", b);
}

// Logical short-circuit or.
static void _scorl(const int b)
{
    io.print("\tSE V%1X,0x00", v);
    io.print("\tJP END%d", b);
}

// Prefix increment an L-value. The L-value is checked.
static char* inc()
{
    io.match('+');
    char* lv = io.gname();
    lcheck(lv, "++");
    _inc(lv);
    _lname(lv);
    return lv;
}

// Prefix not.
static char* notl()
{
    io.match('!');
    char* lv = term();
    _notl();
    return lv;
}

// Prefix positive.
static char* pos()
{
    io.match('+');
    if(io.peek() == '+')
        return inc();
    return str.dup("+");
}

// Prefix bitwise invert.
static char* inv()
{
    io.match('~');
    char* lv = term();
    _inv();
    return lv;
}

// Prefix decrement an L-value. The L-value is checked.
static char* dec()
{
    io.match('-');
    char* lv = io.gname();
    lcheck(lv, "--");
    _dec(lv);
    _lname(lv);
    return lv;
}

// Prefix negate.
static char* neg()
{
    io.match('-');
    if(io.peek() == '-')
        return dec();
    char* lv = term();
    _neg();
    return lv;
}

// Force an expression.
static char* fexp()
{
    io.match('(');
    expression();
    io.match(')');
    return str.dup(")");
}

// Digit loading.
static char* lnum()
{
    char* rv = io.gnum();
    _lnum(rv);
    return rv;
}

// Name loading.
static char* lname()
{
    char* lv = io.gname();
    _lname(lv);
    return lv;
}

// Terms may start with a prefix modifier, an alpha for a name load
// or a digit for a number load. A term may be computed in advanced
// when enclosing brackets are present.
static char* term()
{
    io.skip();
    return
    // Modifiers.
    io.peek() == '~'   ? inv  () :
    io.peek() == '+'   ? pos  () :
    io.peek() == '!'   ? notl () :
    io.peek() == '-'   ? neg  () :
    // Enclosed in brackets.
    io.peek() == '('   ? fexp () :
    // Name load.
    isalpha(io.peek()) ? lname() :
    // Digit load.
    isdigit(io.peek()) ? lnum () :
    // Will return the string if nothing was done.
    io.peeks();
}

// Do chain operator.
// Chain operators populate the stack from V0 upwards.
static void dcop(char* op)
{
    str.eql(op, "+" ) ? _add  () :
    str.eql(op, "-" ) ? _sub  () :
    str.eql(op, "<<") ? _shl  () :
    str.eql(op, ">>") ? _shr  () :
    str.eql(op, "&" ) ? _and  () :
    str.eql(op, "&&") ? _cp   () :
    str.eql(op, "|" ) ? _or   () :
    str.eql(op, "?" ) ? _cp   () :
    str.eql(op, ":" ) ? _cp   () :
    str.eql(op, "||") ? _cp   () :
    str.eql(op, "^" ) ? _xor  () :
    str.eql(op, "==") ? _eql  () :
    str.eql(op, "!=") ? _neql () :
    str.eql(op, "<" ) ? _lt   () :
    str.eql(op, ">" ) ? _gt   () :
    str.eql(op, "<=") ? _lteql() :
    str.eql(op, ">=") ? _gteql() :
    io.bomb("unknown operator '%s'", op);
}

// Do assignment operator.
// Assignment operators directly operate on an L-value.
// The L-value will be checked before the assignment operator is done.
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
    io.bomb("unknown lvalue operator '%s' on lvalue '%s'", op, lv);
}

// Do postfix operator.
static char* dpop(char* lv, char* op)
{
    lcheck(lv, op);
    str.eql(op, "++" ) ? _inc(lv) :
    str.eql(op, "--" ) ? _dec(lv) :
    io.bomb("unknown postfix operator '%s'", op);
    free(op);
    return io.gop();
}

// Do logical operator.
static void dlop(char* op, const int b)
{
    str.eql(op, "&&") ? _scandl(b) :
    str.eql(op, "||") ? _scorl (b) :
    io.bomb("unknown logical operator");
}

// Expression ends logically.
static void exendl()
{
    io.print("\tSE V%1X,0x00", v);
    io.print("\tLD V%1X,0x01", v);
}

// Do ternary operation.
static void dtop(char* op, const int b)
{
    str.eql(op, "?") ? _q(b) :
    str.eql(op, ":") ? _c(b) :
    io.bomb("unknown ternary operator '%s'", op);
}

// End of statement.
static void ends(const int b)
{
    io.print("END%d:", b);
}

// L-value Operator R-value Operator L-value...
static void expression()
{
    const int b = branch++;
    // L-Value.
    char* lv = term();
    int logical = 0;
    while(!io.isendexpr())
    {
        // Operator.
        char* op = io.gop();
        if(str.islogical(op))
        {
            dlop(op, b);
            logical = 1;
        }
        else
        if(str.istern(op))
            dtop(op, b);
        else
        if(str.ispostfix(op))
            op = dpop(lv, op);
        // R-Value.
        v++;
        char* rv = term();
        if(str.isassign(op))
            daop(op, lv);
        else
        if(str.ischain(op))
            dcop(op);
        v--;
        free(op);
        free(lv);
        // As the chain progresses,
        // the L-value becomes the R-value.
        lv = rv;
    }
    free(lv);
    if(logical)
        exendl();
    ends(b);
}

// Declaring a sprite.
static void _sprite()
{
    char* name = io.gname();
    isndef(name);
    io.print("%s:", labels[l++] = name);
    io.match('=');
    io.match('{');
    while(io.peek() != '}')
    {
        char* num = io.gnum();
        io.print("\tDB 0x%02X", strtol(num, NULL, 0));
        free(num);
        io.skip();
        // Bytes are separated by commas.
        // The last byte can be comma but does not have to be.
        if(io.peek() == ',')
        {
            io.match(',');
            io.skip();
            if(io.peek() == '}')
                break;
        }
    }
    io.match('}');
    io.match(';');
}

// Declaring an identifier.
static void identifier()
{
    char* name = io.gname();
    isndef(name);
    io.match('=');
    expression();
    io.match(';');
    vars[v++] = name;
}

// Declaring a block.
static void block()
{
    int idents = 0;
    io.match('{');
    while(io.peek() != '}')
    {
        io.skip();
        switch(io.peek())
        {
        case '{':
            block();
            break;
        case 'b':
            io.matches("byte");
            identifier();
            idents++;
            break;
        default:
            expression();
            break;
        }
        io.skip();
    }
    io.match('}');
    pop(idents);
}

// Declaring a function.
static void _fun()
{
    char* name = io.gname();
    isndef(name);
    io.match('(');
    io.match(')');
    block();
    labels[l++] = name;
}

// Ensures a space is left after a name.
static void check()
{
    if(!isspace(io.peek()))
        io.bomb("expected a space between type and name");
}

// Declaring a program.
static void program()
{
    while(!io.eof())
    {
        io.skip();
        switch(io.peek())
        {
        case 'v':
            io.matches("void");
            check();
            _fun();
            break;
        case 'b':
            io.matches("byte");
            check();
            _fun();
            break;
        case 's':
            io.matches("sprite");
            check();
            _sprite();
            break;
        default:
            io.bomb("unsupported type starting with '%c'", io.peek());
            break;
        }
        io.skip();
    }
}

const struct compile compile = {
    init, program
};
