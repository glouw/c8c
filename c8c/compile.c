#include "compile.h"

#include "io.h"
#include "str.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Variable names.
static int v;

static char* vars[16];

// Array names and function names.
static int l;

static struct label
{
    char* name;
    int args;
}
labels[128];

// Counter for branches.
static int branch;

static void incv()
{
    if(++v == 0xE) io.bomb("register stack overflow");
}

static void decv()
{
    if(--v < 0x00) io.bomb("register stack overflow");
}

// Removes 'n' elements from the variable names.
static void pop(const int n)
{
    for(int i = 0; i < n; i++)
    {
        decv();
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
        { str.dup("draw"), 3 },
        { str.dup("cls" ), 0 },
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

// Returns the register number of a defined variable.
static int gvar(char* name)
{
    for(int i = 0; i < v; i++)
        if(str.eql(name, vars[i]))
            return i;
    io.bomb("variable name '%s' not defined", name);
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
    // Converts a positive number into logical 1.
    io.print("\tSE V%1X,0x00", v);
    io.print("\tLD V%1X,0x01", v);
    // Xor the number with logical 1.
    io.print("\tLD VF,0x01");
    io.print("\tXOR V%1X,VF", v);
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

static void _move()
{
    io.print("\tLD V%1X,V%1X", v - 1, v);
}

// Logical or.
static void _lor()
{
    _move();
}

// Logical and.
static void _land()
{
    _move();
}

// Not equal to.
static void _neqlto()
{
    io.print("\tLD VF,0x00");
    io.print("\tSE V%1X,V%1X", v - 1, v);
    io.print("\tLD VF,0x01");
    io.print("\tLD V%1X,VF", v - 1);
}

// Equal to.
static void _eqlto()
{
    io.print("\tLD VF,0x01");
    io.print("\tSE V%1X,V%1X", v - 1, v);
    io.print("\tLD VF,0x00");
    io.print("\tLD V%1X,VF", v - 1);
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

// Copy.
static void _cp(char* ta)
{
    io.print("\tLD V%1X,V%1X", gvar(ta), v - 1);
}

// Equal.
static void _eql(char* ta)
{
    _move();
    _cp(ta);
}

static void _addeql(char* ta)
{
    _add();
    _cp(ta);
}

static void _subeql(char* ta)
{
    _sub();
    _cp(ta);
}

static void _xoreql(char* ta)
{
    _xor();
    _cp(ta);
}

static void _andeql(char* ta)
{
    _and();
    _cp(ta);
}

static void _oreql(char* ta)
{
    _or();
    _cp(ta);
}

// Load name. Works on variable names only.
static void _lname(char* ta)
{
    io.print("\tLD V%1X,V%1X", v, gvar(ta));
}

// Load number.
static void _lnum(char* tb)
{
    io.print("\tLD V%1X,0x%02X", v, tobyte(tb));
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
    io.print("\tLD VF,V%1X", v);
    io.print("\tRET");
}

// Short circuit and (&&)
static void _scand(const int b)
{
    io.print("\tSNE V%1X,0x00", v - 1);
    io.print("\tJP END%d", b);
}

// Short circuit or (||)
static void _scor(const int b)
{
    io.print("\tSE V%1X,0x00", v - 1);
    io.print("\tJP END%d", b);
}

// Short circuit end.
static void _scend(const int b)
{
    io.print("\tSE V%1X,0x00", v);
    io.print("\tLD V%1X,0x01", v);
    io.print("END%d:", b);
}

static char* term();

// Prefix not.
static char* notl()
{
    io.match('!');
    char* ta = term();
    _notl();
    return ta;
}

// Prefix positive.
static char* pos()
{
    io.match('+');
    return str.dup("+");
}

// Prefix bitwise invert.
static char* inv()
{
    io.match('~');
    char* ta = term();
    _inv();
    return ta;
}

// Prefix negate.
static char* neg()
{
    io.match('-');
    char* ta = term();
    _neg();
    return ta;
}

static void expression();

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
    char* tb = io.gnum();
    _lnum(tb);
    return tb;
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
    // The first two arguments are expressions for x,y.
    for(int i = 0; i < args; i++)
    {
        expression();
        incv();
        io.match(',');
    }
    // The third argument is a label name for the sprite to draw.
    char* label = io.gname();
    const int index = find(label);
    if(index == -1)
        io.bomb("label '%s' not defined", label);
    io.print("\tLD I,%s", labels[index].name);
    // Optionally, appending square brackets to the label will index
    // the sprite by the size of the sprite. This is meant for 2D sprite arrays,
    // but will work on 1D sprite arrays, so be warned.
    io.skip();
    if(io.peek() == '[')
    {
        const int b = branch++;
        io.match('[');
        expression();
        io.match(']');
        io.print("WHILE%d:", b);
        io.print("\tSNE V%X,0x00", v);
        io.print("\tJP OUT%d", b);
        io.print("\tLD VF,0x%02X", labels[index].args);
        io.print("\tADD I,VF");
        io.print("\tLD VF,0x01");
        io.print("\tSUB V%X,VF", v);
        io.print("\tJP WHILE%d", b);
        io.print("OUT%d:", b);
    }
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
        incv();
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
    io.print("\tLD V%1X,VF", v);
}

// Name loading.
static char* lname()
{
    char* ta = io.gname();
    io.skip();
    io.peek() == '(' ? call(ta) : _lname(ta);
    return ta;
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
static void dop(char* op, char* ta)
{
    str.eql(op, "=" ) ? _eql     (ta) :
    str.eql(op, "+=") ? _addeql  (ta) :
    str.eql(op, "-=") ? _subeql  (ta) :
    str.eql(op, "^=") ? _xoreql  (ta) :
    str.eql(op, "&=") ? _andeql  (ta) :
    str.eql(op, "|=") ? _oreql   (ta) :
    str.eql(op, "+" ) ? _add     ()   :
    str.eql(op, "-" ) ? _sub     ()   :
    str.eql(op, "&" ) ? _and     ()   :
    str.eql(op, "^" ) ? _xor     ()   :
    str.eql(op, "|" ) ? _or      ()   :
    str.eql(op, "<" ) ? _lt      ()   :
    str.eql(op, "<=") ? _lteqlto ()   :
    str.eql(op, "!=") ? _neqlto  ()   :
    str.eql(op, "==") ? _eqlto   ()   :
    str.eql(op, "||") ? _lor     ()   :
    str.eql(op, "&&") ? _land    ()   :
    str.eql(op, "!" ) ? _notl    ()   :
    str.eql(op, ">" ) ? _gt      ()   :
    str.eql(op, ">=") ? _gteqlto ()   :
    io.bomb("unknown operator '%s'", op);
}

// Names start with alpha characters or underscores.
static int isname(const char* s)
{
    if(s == NULL)
        io.bomb("derefereced a null pointer.");
    return isalpha(s[0]) || s[0] == '_';
}

static void expression()
{
    bool lvalue = false;
    bool logical = false;
    const int b = branch++;
    char* ta = term();
    if(isname(ta))
        lvalue = true;
    while(!io.isendexpr())
    {
        incv();
        char* op = io.gop();
        if(str.eql(op, "=" )
        || str.eql(op, "+=")
        || str.eql(op, "-="))
        {
            if(lvalue == false)
                io.bomb("expected lvalue to the left of operator '%s'", op);
            expression();
        }
        else
        if(str.eql(op, "&&"))
        {
            logical = true;
            _scand(b);
            expression();
        }
        else
        if(str.eql(op, "||"))
        {
            logical = true;
            _scor(b);
            expression();
        }
        char* tb = term();
        lvalue = false;
        dop(op, ta);
        decv();
        free(op);
        free(ta);
        ta = tb;
    }
    if(logical)
        _scend(b);
    free(ta);
}

static int populate()
{
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
    return size;
}

// Declaring an array (sprite).
static void arr(char* name)
{
    isndef(name);
    io.print("%s:", name);
    io.match('[');
    io.match(']');
    io.skip();
    int size = 0;
    if(io.peek() == '[')
    {
        // 2D array.
        io.match('[');
        io.match(']');
        io.match('=');
        io.match('{');
        size = populate();
        io.skip();
        while(io.peek() == ',')
        {
            io.match(',');
            io.skip();
            if(io.peek() == '}')
                break;
            if(populate() != size)
                io.bomb("array elements must be same length");
            io.skip();
        }
        io.match('}');
    }
    else
    {
        io.match('=');
        size = populate();
    }
    io.match(';');
    struct label sprite = { name, size };
    labels[l++] = sprite;
}

// Declaring an identifier.
static void identifier()
{
    do
    {
        char* name = io.gname();
        isndef(name);
        vars[v] = name;
        io.match('=');
        expression();
        incv();
        if(io.peek() == ',')
            io.match(',');
    }
    while(io.peek() != ';');
    io.match(';');
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
        case 'a':
            io.matches("auto");
            identifier();
            idents++;
            break;
        case 'r':
            io.matches("return");
            if(!isspace(io.peek()))
                io.bomb("expected space after 'return'");
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
static void fun(char* name)
{
    isndef(name);
    io.match('(');
    io.skip();
    int args = 0;
    while(io.peek() != ')')
    {
        char* arg = io.gname();
        isndef(arg);
        io.skip();
        // Arguments can be separated by a comma or a closing paren.
        if(io.peek() == ',')
        {
            io.match(',');
            vars[v] = arg;
            incv();
            args++;
        }
        // If its a closing paren then there are no more args.
        else
        if(io.peek() == ')')
        {
            vars[v] = arg;
            incv();
            args++;
        }
        else io.bomb("unknown symbol in argument list");
        io.skip();
    }
    struct label label = { name, args };
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
        char* name = io.gname();
        io.skip();
        switch(io.peek())
        {
        case '[':
            arr(name);
            break;
        case '(':
            fun(name);
            break;
        default:
            io.bomb("symbol '%c' not known", io.peek());
            break;
        }
        io.skip();
    }
}

const struct compile compile = {
    init, program
};
