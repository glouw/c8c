#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
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

// The current input file character.
static int now;

// The line number.
static int nline = 1;

// Input file (c8).
static FILE* fi;

static char* source;

// Output file (asm).
static FILE* fo;

static char* assembly;

// Line buffer.
static char* lbuff;

// Number of characters reads from input file.
static int reads;

// Failure signaler raised by bomb(). Used to remove .c8 file at cleanup.
static int failure;

// Returns 1 if two strings match, else 0.
static int eql(const char* a, const char* b)
{
    if(a == NULL || b == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

// Duplicates a string.
static char* dup(const char* s)
{
    return strcpy((char*) malloc(strlen(s) + 1), s);
}

// Writes to the output file. A new line is included.
static void print(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(fo, msg, args);
    fprintf(fo, "\n");
    va_end(args);
}

// Writes to standard error. A newline is included. Output is in red.
static void bomb(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "error: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
    failure = 1;
    exit(1);
}

// Closes eveyrthing done. Removes the assembly file
// if something went wrong during compilation.
static void fshutdown()
{
    free(lbuff);
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(failure)
        remove(assembly);
}

// Buffers a new character from the input file.
static void buff()
{
    lbuff[reads++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        reads = 0;
        print(";%s", lbuff);
    }
}

// Gets a new charcter from the input file.
// Does not ignore comments.
static void step()
{
    now = fgetc(fi);
    buff();
}

// Gets a new charcter from the input file.
// Ignore comments.
static void next()
{
    step();
    if(now == '/')
    {
        step();
        if(now != '/')
            bomb("expected '/'");
        while(now != '\n')
            step();
    }
}

// Skips white face.
static void skip()
{
    while(isspace(now))
        next();
}

static void finit(char* argv[])
{
    source = argv[1];
    assembly = argv[2];
    // Input
    fi = fopen(argv[1], "r");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", source);
        exit(1);
    }
    // Output
    fo = fopen(argv[2], "w");
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s cannot be made\n", assembly);
        exit(1);
    }
    atexit(fshutdown);
    lbuff = (char*) malloc(512 * sizeof(char));
    next();
    skip();
}

// Returns the current char of the input file.
static char peek()
{
    return now;
}

// Same as peek(), but in string form.
static char* peeks()
{
    char n[] = { (char) now, '\0' };
    return dup(n);
}

// Returns true if input file is now at the end of an expression.
static int isendexpr()
{
    return now == ';'
        || now == ')'
        || now == ']'
        || now == ',';
}

// Returns true if end of operator.
static int isendop()
{
    return isalnum(now) || isspace(now) || isendexpr();
}

// Exits if the current character does not match what is expected.
// Also advances the unput buffer by one.
static void match(const int c)
{
    skip();
    if(now != c)
        bomb("expected '%c'", c);
    next();
}

// Exits if the current string does not match what is expected.
// Also advances the unput buffer by the length of the expected string.
static void matches(const char* s)
{
    skip();
    for(char* i = (char*) s; *i; i++)
    {
        if(now != *i)
            bomb("expected '%s'", s);
        next();
    }
}

// Gets a number as a name.
static char* gname()
{
    skip();
    // First char must be alpha or underscore
    if(!(now == '_' || isalpha(now)))
        bomb("names must start with underscores or alpha chars");
    char* name = (char*) malloc(128 * sizeof(char));
    int i = 0;
    // But name can contain alpha an numeric chars.
    while(isalnum(now) || now == '_')
    {
        name[i++] = now;
        next();
    }
    name[i] = '\0';
    return name;
}

// Gets a number as a string.
static char* gnum()
{
    skip();
    if(!isdigit(now))
        bomb("expected value");
    char* num = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(isdigit(now) || isxdigit(now) || tolower(now) == 'x')
    {
        num[i++] = now;
        next();
    }
    num[i] = '\0';
    return num;
}

// Gets an operation as a string.
static char* gop()
{
    skip();
    char* op = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(!isendop())
    {
        op[i++] = now;
        next();
    }
    op[i] = '\0';
    return op;
}

// Returns true if end of file.
static int eof()
{
    return now == EOF;
}

static void incv()
{
    if(++v == 0xE) bomb("register stack overflow");
}

static void decv()
{
    if(--v < 0x00) bomb("register stack overflow");
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

static void lshutdown()
{
    reset();
    kill();
}

static void linit()
{
    struct label baked[] = {
        { dup("draw"), 3 },
        { dup("cls" ), 0 },
    };
    for(unsigned i = 0; i < sizeof(baked) / sizeof(*baked); i++)
        labels[l++] = baked[i];
    atexit(lshutdown);
}

static int find(const char* name)
{
    for(int i = 0; i < l; i++)
        if(eql(name, labels[i].name))
            return i;
    return -1;
}

// Returns the register number of a defined variable.
static int gvar(char* name)
{
    for(int i = 0; i < v; i++)
        if(eql(name, vars[i]))
            return i;
    bomb("variable name '%s' not defined", name);
    return 0;
}

// Returns true if a name is already defined, else exits
static int isndef(char* name)
{
    // Checks vars.
    for(int i = 0; i < v; i++)
        if(eql(name, vars[i]))
            bomb("name '%s' already defined", name);
    // Checks labels.
    for(int i = 0; i < l; i++)
        if(eql(name, labels[i].name))
            bomb("label '%s' already defined", name);
    return 1;
}

static int tobyte(const char* value)
{
    return strtol(value, NULL, 0) % 256;
}

// Negation.
static void _neg()
{
    print("\tXOR V%1X,0xFF", v);
    print("\tADD V%1X,0x01", v);
}

// Bitwise inversion.
static void _inv()
{
    print("\tXOR V%1X,0xFF", v);
}

// Logical not.
static void _notl()
{
    // Converts a positive number into logical 1.
    print("\tSE V%1X,0x00", v);
    print("\tLD V%1X,0x01", v);
    // Xor the number with logical 1.
    print("\tLD VF,0x01");
    print("\tXOR V%1X,VF", v);
}

// Addition.
static void _add()
{
    print("\tADD V%1X,V%1X", v - 1, v);
}

// Subtraction.
static void _sub()
{
    print("\tSUB V%1X,V%1X", v - 1, v);
}

// Bitwise and.
static void _and()
{
    print("\tAND V%1X,V%1X", v - 1, v);
}

// Bitwise or.
static void _or()
{
    print("\tOR V%1X,V%1X", v - 1, v);
}

// Bitwise xor.
static void _xor()
{
    print("\tXOR V%1X,V%1X", v - 1, v);
}

static void _move()
{
    print("\tLD V%1X,V%1X", v - 1, v);
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
    print("\tLD VF,0x00");
    print("\tSE V%1X,V%1X", v - 1, v);
    print("\tLD VF,0x01");
    print("\tLD V%1X,VF", v - 1);
}

// Equal to.
static void _eqlto()
{
    print("\tLD VF,0x01");
    print("\tSE V%1X,V%1X", v - 1, v);
    print("\tLD VF,0x00");
    print("\tLD V%1X,VF", v - 1);
}

// Less than.
static void _lt()
{
    print("\tSUBN V%1X,V%1X", v - 1, v);
    print("\tLD V%1X,VF", v - 1);
}

// Greater than.
static void _gt()
{
    print("\tSUB V%1X,V%1X", v - 1, v);
    print("\tLD V%1X,VF", v - 1);
}

// Less than or equal to.
static void _lteqlto()
{
    _gt();
    print("\tXOR V%1X,0x01", v - 1);
}

// Greater than or equal to.
static void _gteqlto()
{
    _lt();
    print("\tXOR V%1X,0x01", v - 1);
}

// Copy.
static void _cp(char* ta)
{
    print("\tLD V%1X,V%1X", gvar(ta), v - 1);
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
    print("\tLD V%1X,V%1X", v, gvar(ta));
}

// Load number.
static void _lnum(char* tb)
{
    print("\tLD V%1X,0x%02X", v, tobyte(tb));
}

// Pushes the stack to the stack frame.
static void _fpush()
{
    print("\tLD F,VE");
    print("\tLD [I],VE");
    print("\tLD VF,0x03");
    print("\tADD VE,VF");
}

// Pops a stack from the stack frame.
static void _fpop()
{
    print("\tLD VF,0x03");
    print("\tSUB VE,VF");
    print("\tLD F,VE");
    print("\tLD VE,[I]");
    print("\tLD VF,V%1X", v);
    print("\tRET");
}

// Short circuit and (&&)
static void _scand(const int b)
{
    print("\tSNE V%1X,0x00", v - 1);
    print("\tJP END%d", b);
}

// Short circuit or (||)
static void _scor(const int b)
{
    print("\tSE V%1X,0x00", v - 1);
    print("\tJP END%d", b);
}

// Short circuit end.
static void _scend(const int b)
{
    print("\tSE V%1X,0x00", v);
    print("\tLD V%1X,0x01", v);
    print("END%d:", b);
}

static char* term();

// Prefix not.
static char* notl()
{
    match('!');
    char* ta = term();
    _notl();
    return ta;
}

// Prefix positive.
static char* pos()
{
    match('+');
    return dup("+");
}

// Prefix bitwise invert.
static char* inv()
{
    match('~');
    char* ta = term();
    _inv();
    return ta;
}

// Prefix negate.
static char* neg()
{
    match('-');
    char* ta = term();
    _neg();
    return ta;
}

static void expression();

// Force an expression.
static char* fexp()
{
    match('(');
    expression();
    match(')');
    return dup(")");
}

// Digit loading.
static char* lnum()
{
    char* tb = gnum();
    _lnum(tb);
    return tb;
}

// Saves a copy of the current stack frame.
// Moves local variables from current frame to next.
static void move(const int args)
{
    _fpush();
    for(int i = 0; i < args; i++)
        print("\tLD V%1X,V%1X", i, v + i);
}

// Returns last register.
static void ret()
{
    expression();
    match(';');
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
        match(',');
    }
    // The third argument is a label name for the sprite to draw.
    char* label = gname();
    const int index = find(label);
    if(index == -1)
        bomb("label '%s' not defined", label);
    print("\tLD I,%s", labels[index].name);
    // Optionally, appending square brackets to the label will index
    // the sprite by the size of the sprite. This is meant for 2D sprite arrays,
    // but will work on 1D sprite arrays, so be warned.
    skip();
    if(peek() == '[')
    {
        const int b = branch++;
        match('[');
        expression();
        match(']');
        print("WHILE%d:", b);
        print("\tSNE V%X,0x00", v);
        print("\tJP OUT%d", b);
        print("\tLD VF,0x%02X", labels[index].args);
        print("\tADD I,VF");
        print("\tLD VF,0x01");
        print("\tSUB V%X,VF", v);
        print("\tJP WHILE%d", b);
        print("OUT%d:", b);
    }
    print("\tDRW V%1X,V%1X,0x%1X", v - 2, v - 1, labels[index].args);
    free(label);
    v -= args;
}

// The clear screen is inlined for performance.
static void cls()
{
    print("\tCLS");
}

static void _call(const char* name)
{
    const int i = find(name);
    if(i == -1)
        bomb("function '%s' not defined", name);
    int args = 0;
    while(peek() != ')')
    {
        expression();
        incv();
        args++;
        skip();
        if(peek() == ',')
            match(',');
        else
        if(peek() == ')')
            break;
        else bomb("unknown symbol in argument list");
    }
    if(labels[i].args != args)
        bomb("argument mismatch when calling '%s'", name);
    v -= args;
    move(args);
    print("\tCALL %s", name);
}

static void call(const char* name)
{
    match('(');
    // Built in functions will inline.
    if(eql(name, "draw"))
        draw();
    else
    if(eql(name, "clear"))
        cls();
    // Runtime function call.
    else
        _call(name);
    match(')');
    // Load return value.
    print("\tLD V%1X,VF", v);
}

// Name loading.
static char* lname()
{
    char* ta = gname();
    skip();
    peek() == '(' ? call(ta) : _lname(ta);
    return ta;
}

// Terms may start with a prefix modifier, an alpha for a name load
// or a digit for a number load. A term may be computed in advanced
// when enclosing brackets are present.
static char* term()
{
    skip();
    return
    // Modifiers.
    peek() == '~'   ? inv  () :
    peek() == '+'   ? pos  () :
    peek() == '!'   ? notl () :
    peek() == '-'   ? neg  () :
    // Enclosed in brackets.
    peek() == '('   ? fexp () :
    // Name load.
    isalpha(peek()) ? lname() :
    // Digit load.
    isdigit(peek()) ? lnum () :
    // Will return the string if nothing was done.
    peeks();
}

// Do chain operator.
// Chain operators populate the stack from V0 upwards.
static void dop(char* op, char* ta)
{
    eql(op, "=" ) ? _eql     (ta) :
    eql(op, "+=") ? _addeql  (ta) :
    eql(op, "-=") ? _subeql  (ta) :
    eql(op, "^=") ? _xoreql  (ta) :
    eql(op, "&=") ? _andeql  (ta) :
    eql(op, "|=") ? _oreql   (ta) :
    eql(op, "+" ) ? _add     ()   :
    eql(op, "-" ) ? _sub     ()   :
    eql(op, "&" ) ? _and     ()   :
    eql(op, "^" ) ? _xor     ()   :
    eql(op, "|" ) ? _or      ()   :
    eql(op, "<" ) ? _lt      ()   :
    eql(op, "<=") ? _lteqlto ()   :
    eql(op, "!=") ? _neqlto  ()   :
    eql(op, "==") ? _eqlto   ()   :
    eql(op, "||") ? _lor     ()   :
    eql(op, "&&") ? _land    ()   :
    eql(op, "!" ) ? _notl    ()   :
    eql(op, ">" ) ? _gt      ()   :
    eql(op, ">=") ? _gteqlto ()   :
    bomb("unknown operator '%s'", op);
}

// Names start with alpha characters or underscores.
static int isname(const char* s)
{
    if(s == NULL)
        bomb("derefereced a null pointer.");
    return isalpha(s[0]) || s[0] == '_';
}

// TERM A _ OP _ TERM B _ OP _ TERM A _ OP...
static void expression()
{
    bool lvalue = false;
    bool logical = false;
    const int b = branch++;
    char* ta = term();
    if(isname(ta))
        lvalue = true;
    while(!isendexpr())
    {
        incv();
        char* op = gop();
        if(eql(op, "=" )
        || eql(op, "+=")
        || eql(op, "-="))
        {
            if(lvalue == false)
                bomb("expected lvalue to the left of operator '%s'", op);
            expression();
        }
        else
        if(eql(op, "&&"))
        {
            logical = true;
            _scand(b);
            expression();
        }
        else
        if(eql(op, "||"))
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
    match('{');
    int size = 0;
    while(peek() != '}')
    {
        char* num = gnum();
        print("\tDB 0x%02X", tobyte(num));
        size++;
        free(num);
        skip();
        // Bytes are separated by commas.
        // The last byte can be comma but does not have to be.
        if(peek() == ',')
        {
            match(',');
            skip();
            if(peek() == '}') break;
        }
    }
    if(size > 0xF)
        bomb("too many elements in sprite");
    match('}');
    return size;
}

// Declaring an array (sprite).
static void arr(char* name)
{
    isndef(name);
    print("%s:", name);
    match('[');
    match(']');
    skip();
    int size = 0;
    if(peek() == '[')
    {
        // 2D array.
        match('[');
        match(']');
        match('=');
        match('{');
        size = populate();
        skip();
        while(peek() == ',')
        {
            match(',');
            skip();
            if(peek() == '}')
                break;
            if(populate() != size)
                bomb("array elements must be same length");
            skip();
        }
        match('}');
    }
    else
    {
        match('=');
        size = populate();
    }
    match(';');
    struct label sprite = { name, size };
    labels[l++] = sprite;
}

// Declaring an identifier.
static void identifier()
{
    do
    {
        char* name = gname();
        isndef(name);
        vars[v] = name;
        match('=');
        expression();
        incv();
        if(peek() == ',')
            match(',');
    }
    while(peek() != ';');
    match(';');
}

static void block();

// Declaring a while loop
static void _while()
{
    const int b = branch++;
    match('(');
    print("WHILE%d:", b);
    expression();
    print("\tSNE V%1X,0x00", v);
    print("\tJP END%d", b);
    match(')');
    block();
    print("\tJP WHILE%d", b);
    print("END%d:", b);
}

// Declaring a block.
// Blocks hold statements.
static void block()
{
    int idents = 0;
    match('{');
    while(peek() != '}')
    {
        skip();
        switch(peek())
        {
        case '{':
            block();
            break;
        case 'w':
            matches("while");
            _while();
            break;
        case 'a':
            matches("auto");
            identifier();
            idents++;
            break;
        case 'r':
            matches("return");
            if(!isspace(peek()))
                bomb("expected space after 'return'");
            ret();
            break;
        case '}':
            break;
        default:
            expression();
            match(';');
            break;
        }
        skip();
    }
    match('}');
    pop(idents);
}

// Declaring a function.
static void fun(char* name)
{
    isndef(name);
    match('(');
    skip();
    int args = 0;
    while(peek() != ')')
    {
        char* arg = gname();
        isndef(arg);
        skip();
        // Arguments can be separated by a comma or a closing paren.
        if(peek() == ',')
        {
            match(',');
            vars[v] = arg;
            incv();
            args++;
        }
        // If its a closing paren then there are no more args.
        else
        if(peek() == ')')
        {
            vars[v] = arg;
            incv();
            args++;
        }
        else bomb("unknown symbol in argument list");
        skip();
    }
    struct label label = { name, args };
    labels[l++] = label;
    print("%s:", name);
    match(')');
    block();
    _fpop();
    reset();
}

// Declaring a program.
static void program()
{
    while(!eof())
    {
        char* name = gname();
        skip();
        switch(peek())
        {
        case '[':
            arr(name);
            break;
        case '(':
            fun(name);
            break;
        default:
            bomb("symbol '%c' not known", peek());
            break;
        }
        skip();
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
        bomb("expected input and output file");
    finit(argv);
    linit();
    program();
}
