#include "compile.h"

#include "io.h"
#include "str.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Variable names.
static int v;

static char* vars[16];

// Sprite names and function names.
static int l;

enum type { VOID, BYTE, SPRITE, TYPES };

static struct label
{
    char* name;
    int args;
    enum type type;
}
labels[128];

// When identifying this flag is set high to aid error checking.
int identing;

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
        free(vars[v]);
        vars[v] = NULL;
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

// Removes all label names.
static void kill()
{
    for(int i = 0; i < l; i++)
    {
        free(labels[i].name);
        labels[i].name = NULL;
    }
    l = 0;
}

static void shutdown()
{
    reset();
    kill();
}

static void init()
{
    struct label baked[] = {
        // byte draw(byte x, byte y, sprite s);
        { str.dup("draw"), 3, BYTE },
        // void cls();
        { str.dup("cls" ), 0, VOID },
    };
    for(unsigned i = 0; i < len(baked); i++)
        labels[l++] = baked[i];
    atexit(shutdown);
}

static int find(const char* name)
{
    for(int i = 0; i < l; i++)
        if(str.eql(name, labels[i].name))
            return i;
    return -1;
}

// Ensures L-value is a name.
static void lcheck(const char* lv, const char* op)
{
    if(!str.isname(lv))
        io.bomb("'%s' must be lvalue for operator '%s'", lv, op);
}

// Ensures a space exists between token and name.
static void tcheck()
{
    if(!isspace(io.peek()))
        io.bomb("expected space between token and name");
}

// Returns the register number of a defined variable.
static int gvar(char* name)
{
    for(int i = 0; i < v; i++)
        if(str.eql(name, vars[i]))
            return i;
    io.bomb("variable name '%s' not defined\n", name);
    return 0;
}

// Returns true if a name is already defined, else exits
static int isndef(char* name)
{
    // Checks vars.
    for(int i = 0; i < v; i++)
        if(str.eql(name, vars[i]))
            io.bomb("name '%s' already defined", name);
    // Checks labels.
    for(int i = 0; i < l; i++)
        if(str.eql(name, labels[i].name))
            io.bomb("label '%s' already defined", name);
    return 1;
}

static int tobyte(const char* value)
{
    return strtol(value, NULL, 0) % 256;
}

/*
 * Code generation functions are prefixed with a single underscore
 */

// Decerment.
static void _dec(char* lv)
{
    io.print("\tLD VF,0x01");
    io.print("\tSUB V%1X,VF", gvar(lv));
}

// Increment.
static void _inc(char* lv)
{
    io.print("\tADD V%1X,0x01", gvar(lv));
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
    io.print("\tSHL V%1X,V%1X", v - 1, v - 1);
}

// Shift right.
static void _shr()
{
    io.print("\tSHR V%1X,V%1X", v - 1, v - 1);
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

// Not equal to.
static void _neqlto()
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

// Equal to.
static void _eqlto()
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
static void _lteqlto()
{
    _gt();
    io.print("\tXOR V%1X,0x01", v - 1);
}

// Greater than or equal to.
static void _gteqlto()
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

// Equal.
static void _eql(char* lv)
{
    io.print("\tLD V%1X,V%1X", gvar(lv), v);
}

// Add equal.
static void _addeql(char* lv)
{
    io.print("\tADD V%1X,V%1X", gvar(lv), v);
    _add();
}

// Minus equal.
static void _subeql(char* lv)
{
    io.print("\tSUB V%1X,V%1X", gvar(lv), v);
    _sub();
}

// Shift left equal.
static void _shleq(char* lv)
{
    const int _v = gvar(lv);
    io.print("\tSHL V%1X,V%1X", _v, _v);
    _shl();
}

// Shift right equal.
static void _shreq(char* lv)
{
    const int _v = gvar(lv);
    io.print("\tSHR V%1X,V%1X", _v, _v);
    _shr();
}

// And equal.
static void _andeq(char* lv)
{
    io.print("\tAND V%1X,V%1X", gvar(lv), v);
    _and();
}

// Or equal.
static void _oreq(char* lv)
{
    io.print("\tOR V%1X,V%1X", gvar(lv), v);
    _or();
}

// Xor equal
static void _xoreq(char* lv)
{
    io.print("\tXOR V%1X,V%1X", gvar(lv), v);
    _xor();
}

// Load name. Works on variable names only.
static void _lname(char* lv)
{
    io.print("\tLD V%1X,V%1X", v, gvar(lv));
}

// Load number.
static void _lnum(char* rv)
{
    io.print("\tLD V%1X,0x%02X", v, tobyte(rv));
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

// Pushes the stack to the stack frame.
static void _fpush()
{
    io.print("\tLD F,VE");
    io.print("\tLD [I],VE");
    io.print("\tLD VF,0x03");
    io.print("\tADD VE,VF");
}

// Pops a stack from the stack frame.
static void _fpop()
{
    io.print("\tLD VF,0x03");
    io.print("\tSUB VE,VF");
    io.print("\tLD F,VE");
    io.print("\tLD VE,[I]");
    io.print("\tLD VF,V%d", v);
    io.print("\tRET");
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

// Saves a copy of the current stack frame.
// Moves local variables from current frame to next.
static void move(const int args)
{
    _fpush();
    for(int i = 0; i < args; i++)
        io.print("\tLD V%1X,V%1X", i, v + i);
}

// Returns last register.
static void ret()
{
    expression();
    io.match(';');
    _fpop();
}

// The drawing function is inlined for performance.
static void draw()
{
    const int args = 2;
    for(int i = 0; i < args; i++)
    {
        expression();
        v++;
        io.match(',');
    }
    char* label = io.gname();
    const int index = find(label);
    if(index == -1)
        io.bomb("label '%s' not defined", label);
    io.print("\tLD I,%s", labels[index].name);
    io.print("\tDRW V%1X,V%1X,0x%1X", v - 2, v - 1, labels[index].args);
    free(label);
    v -= args;
}

// The clear screen is inlined for performance.
static void cls()
{
    io.print("\tCLS");
}

static void _call(const char* name)
{
    const int i = find(name);
    if(i == -1)
        io.bomb("function '%s' not defined", name);
    int args = 0;
    while(io.peek() != ')')
    {
        expression();
        v++;
        args++;
        io.skip();
        if(io.peek() == ',')
            io.match(',');
        else
        if(io.peek() == ')')
            break;
        else io.bomb("unknown symbol in argument list");
    }
    if(labels[i].args != args)
        io.bomb("argument mismatch when calling '%s'", name);
    if(labels[i].type == VOID && identing)
        io.bomb("term '%s' is of void return type", name);
    v -= args;
    move(args);
    io.print("\tCALL %s", name);
}

static void call(const char* name)
{
    io.match('(');
    // Built in functions will inline.
    if(str.eql(name, "draw"))
        draw();
    else
    if(str.eql(name, "clear"))
        cls();
    // Runtime function call.
    else
        _call(name);
    io.match(')');
    // Load return value.
    io.print("\tLD V%d,VF", v);
}

// Name loading.
static char* lname()
{
    char* lv = io.gname();
    io.skip();
    io.peek() == '(' ? call(lv) : _lname(lv);
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
    str.eql(op, "+" ) ? _add     () :
    str.eql(op, "-" ) ? _sub     () :
    str.eql(op, "<<") ? _shl     () :
    str.eql(op, ">>") ? _shr     () :
    str.eql(op, "&" ) ? _and     () :
    str.eql(op, "&&") ? _cp      () :
    str.eql(op, "|" ) ? _or      () :
    str.eql(op, "?" ) ? _cp      () :
    str.eql(op, ":" ) ? _cp      () :
    str.eql(op, "||") ? _cp      () :
    str.eql(op, "^" ) ? _xor     () :
    str.eql(op, "==") ? _eqlto   () :
    str.eql(op, "!=") ? _neqlto  () :
    str.eql(op, "<" ) ? _lt      () :
    str.eql(op, ">" ) ? _gt      () :
    str.eql(op, "<=") ? _lteqlto () :
    str.eql(op, ">=") ? _gteqlto () :
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
    str.eql(op, "="  ) ? _eql   (lv) :
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
        {
            if((str.eql(op, "<<=") || str.eql(op, ">>=")) && !str.eql(rv, "1"))
                io.bomb("assignment shifts must be left or right by constant 1");
            daop(op, lv);
        }
        else
        if(str.ischain(op))
        {
            if((str.eql(op, "<<") || str.eql(op, ">>")) && !str.eql(rv, "1"))
                io.bomb("values may only be shifted left or right by constant 1");
            dcop(op);
        }
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
static void sprite()
{
    char* name = io.gname();
    isndef(name);
    io.print("%s:", name);
    io.match('=');
    io.match('{');
    int size = 0;
    while(io.peek() != '}')
    {
        char* num = io.gnum();
        io.print("\tDB 0x%02X", tobyte(num));
        size++;
        free(num);
        io.skip();
        // Bytes are separated by commas.
        // The last byte can be comma but does not have to be.
        if(io.peek() == ',')
        {
            io.match(',');
            io.skip();
            if(io.peek() == '}') break;
        }
    }
    if(size > 0xF)
        io.bomb("too many elements in sprite");
    io.match('}');
    io.match(';');
    struct label sprite = { name, size, SPRITE };
    labels[l++] = sprite;
}

// Declaring an identifier.
static void identifier()
{
    identing = 1;
    char* name = io.gname();
    isndef(name);
    io.match('=');
    expression();
    io.match(';');
    vars[v++] = name;
    identing = 0;
}

static void block();

// Declaring a while loop
static void _while()
{
    const int b = branch++;
    io.match('(');
    io.print("WHILE%d:", b);
    expression();
    io.print("\tSNE V%1X,0x00", v);
    io.print("\tJP END%d", b);
    io.match(')');
    block();
    io.print("\tJP WHILE%d", b);
    io.print("END%d:", b);
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
        case 'w':
            io.matches("while");
            _while();
            break;
        case 'b':
            io.matches("byte");
            identifier();
            idents++;
            break;
        case 'r':
            io.matches("return");
            tcheck();
            ret();
            break;
        case '}':
            break;
        default:
            expression();
            io.match(';');
            break;
        }
        io.skip();
    }
    io.match('}');
    pop(idents);
}

// Declaring a function.
static void fun(const enum type type)
{
    char* name = io.gname();
    isndef(name);
    io.match('(');
    io.skip();
    int args = 0;
    while(io.peek() != ')')
    {
        io.matches("byte");
        tcheck();
        char* arg = io.gname();
        isndef(arg);
        io.skip();
        // Arguments can be separated by a comma or a closing paren.
        if(io.peek() == ',')
        {
            io.match(',');
            vars[v++] = arg;
            args++;
        }
        // If its a closing paren then there are no more args.
        else
        if(io.peek() == ')')
        {
            vars[v++] = arg;
            args++;
        }
        else io.bomb("unknown symbol in argument list");
        io.skip();
    }
    struct label label = {
        name,
        args,
        type,
    };
    labels[l++] = label;
    io.print("%s:", name);
    io.match(')');
    block();
    _fpop();
    reset();
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
            tcheck();
            fun(VOID);
            break;
        case 'b':
            io.matches("byte");
            tcheck();
            fun(BYTE);
            break;
        case 's':
            io.matches("sprite");
            tcheck();
            sprite();
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
