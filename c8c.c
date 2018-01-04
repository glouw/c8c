//   ____   ___    ____
//  /  __) ( _ )  /  __)
// (  (__  / _ \ (  (__
//  \____) \___/  \____)
//
// C8C is a highly portable C-like compiler for the chip8 platform.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

static char* term();

static void expression(char* overrider, const bool shortable);

static void dblock();

// Variable names.
static int v;

static char* vars[16];

// Array names and function names.
static int l;

static struct label
{
    char* name;
    int args;
    int height;
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

// Output file (asm).
static FILE* fo;

// Input file name.
static char* c8src;

// Output file name.
static char* assem;

// Line buffer.
static char* line;

// Failure signaler raised by bomb(). Used to remove .c8 file at cleanup.
static bool failure;

// Writes to the output file. A new line is included.
static void print(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(fo, msg, args);
    fprintf(fo, "\n");
    va_end(args);
}

// Writes to standard error. A newline is included.
static void bomb(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "error: line %d: ", nline);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
    failure = true;
    exit(1);
}

// Buffers a new character from the input file.
static void buffer()
{
    static int reads;
    line[reads++] = now == '\n' ? '\0' : now;
    if(now == '\n')
    {
        nline++;
        reads = 0;
        print(";%s", line);
    }
}

// Gets a new charcter from the input file. Does not ignore comments.
static void step()
{
    now = fgetc(fi);
    buffer();
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

// Gets a new charcter from the input file. Ignores (//) style comments.
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

// Skips sequential white space.
static void skip()
{
    while(isspace(now)) next();
}

// Duplicates a string.
static char* dup(const char* s)
{
    return strcpy((char*) malloc(strlen(s) + 1), s);
}

// Shuts down everything. Removes assembly file if something went wrong.
static void shutdown()
{
    reset();
    kill();
    free(line);
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(failure)
        remove(assem);
}

// Initializes everything.
static void init(char* argv[])
{
    struct label baked[] = {
        { dup("draw"   ), 3, 0 },
        { dup("cls"    ), 0, 0 },
        { dup("sizeof" ), 1, 0 },
    };
    for(unsigned i = 0; i < sizeof(baked) / sizeof(*baked); i++)
        labels[l++] = baked[i];
    c8src = argv[1];
    assem = argv[2];
    // Input file.
    fi = fopen(argv[1], "r");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", c8src);
        exit(1);
    }
    // Output file.
    fo = fopen(argv[2], "w");
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s cannot be made\n", assem);
        exit(1);
    }
    line = (char*) malloc(512 * sizeof(char));
    next();
    skip();
    atexit(shutdown);
}

// Returns true if two strings match, else false.
static bool eql(const char* a, const char* b)
{
    if(a == NULL || b == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

// Retrusn a string of the 'now' character.
static char* peeks()
{
    char n[] = { (char) now, '\0' };
    return dup(n);
}

// Returns true if 'now' at the end of an expression.
static bool isendexpr()
{
    return now == ';' || now == ')' || now == ']' || now == ',';
}

// Returns true if 'now' at end of operator.
static bool isendop()
{
    return isalnum(now) || isspace(now) || isendexpr();
}

// Skips all white space and comments, then checks if 'now' matches what is expected.
// Feed is advanced.
static void match(const int c)
{
    skip();
    if(now != c)
        bomb("expected '%c'", c);
    next();
}

// Returns a string name.
static char* name()
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

// Returns a string digit.
static char* dig()
{
    skip();
    if(!isdigit(now))
        bomb("expected value");
    char* d = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(isdigit(now) || isxdigit(now) || tolower(now) == 'x')
    {
        d[i++] = now;
        next();
    }
    d[i] = '\0';
    return d;
}

// Returns a string operator.
static char* op()
{
    skip();
    char* o = (char*) malloc(128 * sizeof(char));
    int i = 0;
    while(!isendop())
    {
        o[i++] = now;
        next();
    }
    o[i] = '\0';
    return o;
}

// Increments the v register.
static void incv()
{
    if(++v == 0xE) bomb("register stack overflow");
}

// Decrements the v register.
static void decv()
{
    if(--v < 0x00) bomb("register stack underflow");
}

// Removes 'n' variable names.
static void pop(const int n)
{
    for(int i = 0; i < n; i++)
    {
        decv();
        free(vars[v]);
        vars[v] = NULL;
    }
}

// Returns a positive integer if a label name was found.
static int find(const char* name)
{
    for(int i = 0; i < l; i++)
        if(eql(name, labels[i].name))
            return i;
    return -1;
}

// Returns the v-register number of a declared variable.
// Exits if not found.
static int var(char* name)
{
    for(int i = 0; i < v; i++)
        if(eql(name, vars[i]))
            return i;
    bomb("variable name '%s' not defined", name);
    return 0; /* Keep compiler quiet. */
}

// Returns true if a name is already declared, else exits.
// Note that this checks both automatic variables and label names.
static bool isndef(char* name)
{
    // Checks vars.
    for(int i = 0; i < v; i++)
        if(eql(name, vars[i]))
            bomb("name '%s' already defined", name);
    // Checks labels.
    for(int i = 0; i < l; i++)
        if(eql(name, labels[i].name))
            bomb("label '%s' already defined", name);
    return true;
}

// Turns a string digit into a long digit.
// All integers are unsigned byte sized and thus clamped 0-255 inclusive.
static uint8_t tobyte(const char* value)
{
    return strtoul(value, NULL, 0) % 256;
}

// Generate negation (-).
static void gneg()
{
    print("\tLD VF,0xFF");
    print("\tXOR V%1X,VF", v);
    print("\tADD V%1X,0x01", v);
}

// Generate bitwise inversion (~).
static void ginv()
{
    print("\tLD VF,0xFF");
    print("\tXOR V%1X,VF", v);
}

// Generate logical not (!).
static void gnotl()
{
    // Converts a positive number into logical 1.
    print("\tSE V%1X,0x00", v);
    print("\tLD V%1X,0x01", v);
    // Xor the number with logical 1.
    print("\tLD VF,0x01");
    print("\tXOR V%1X,VF", v);
}

// Generate addition (+).
static void gadd()
{
    print("\tADD V%1X,V%1X", v - 1, v);
}

// Generate subtraction (-).
static void gsub()
{
    print("\tSUB V%1X,V%1X", v - 1, v);
}

// Generate bitwise and (&).
static void gand()
{
    print("\tAND V%1X,V%1X", v - 1, v);
}

// Generate bitwise or (|).
static void gor()
{
    print("\tOR V%1X,V%1X", v - 1, v);
}

// Generate bitwise xor (^).
static void gxor()
{
    print("\tXOR V%1X,V%1X", v - 1, v);
}

// Generate move.
static void gmove()
{
    print("\tLD V%1X,V%1X", v - 1, v);
}

// Generate not equal to (!=).
static void gneqlto()
{
    print("\tLD VF,0x00");
    print("\tSE V%1X,V%1X", v - 1, v);
    print("\tLD VF,0x01");
    print("\tLD V%1X,VF", v - 1);
}

// Generate equal to (==).
static void geqlto()
{
    print("\tLD VF,0x01");
    print("\tSE V%1X,V%1X", v - 1, v);
    print("\tLD VF,0x00");
    print("\tLD V%1X,VF", v - 1);
}

// Generate less than (<).
static void glt()
{
    print("\tSUBN V%1X,V%1X", v - 1, v);
    print("\tLD V%1X,VF", v - 1);
}

// Generate greater than (>).
static void ggt()
{
    print("\tSUB V%1X,V%1X", v - 1, v);
    print("\tLD V%1X,VF", v - 1);
}

// Generate less than or equal to (<=).
static void glteqlto()
{
    ggt();
    print("\tXOR V%1X,0x01", v - 1);
}

// Generate greater than or equal to (>=).
static void ggteqlto()
{
    glt();
    print("\tXOR V%1X,0x01", v - 1);
}

// Generate copy.
static void gcp(char* ta)
{
    print("\tLD V%1X,V%1X", var(ta), v - 1);
}

// Generate equal (=).
static void geql(char* ta)
{
    gmove();
    gcp(ta);
}

// Generate add equal (+=).
static void gaddeql(char* ta)
{
    gadd();
    gcp(ta);
}

// Generate subtract equal (-=)
static void gsubeql(char* ta)
{
    gsub();
    gcp(ta);
}

// Generate xor equal (^=)
static void gxoreql(char* ta)
{
    gxor();
    gcp(ta);
}

// Generate and equal (&=)
static void gandeql(char* ta)
{
    gand();
    gcp(ta);
}

// Generate or equal (|=)
static void goreql(char* ta)
{
    gor();
    gcp(ta);
}

// Generate load name.
static void glname(char* ta)
{
    print("\tLD V%1X,V%1X", v, var(ta));
}

// Generate load digit.
static void gldig(char* tb)
{
    print("\tLD V%1X,0x%02X", v, tobyte(tb));
}

// Generate frame push.
static void gfpush()
{
    print("\tLD F,VE");
    print("\tLD [I],VE");
    print("\tLD VF,0x03");
    print("\tADD VE,VF");
}

// Generate frame pop.
static void gfpop()
{
    print("\tLD VF,0x03");
    print("\tSUB VE,VF");
    print("\tLD VF,V%1X", v);
    print("\tLD F,VE");
    print("\tLD VE,[I]");
    print("\tRET");
}

// Unary not.
static char* notl()
{
    match('!');
    char* ta = term();
    gnotl();
    return ta;
}

// Unary positive.
static char* pos()
{
    match('+');
    return term();
}

// Unary bitwise invert.
static char* inv()
{
    match('~');
    char* ta = term();
    ginv();
    return ta;
}

// Unary negate.
static char* neg()
{
    match('-');
    char* ta = term();
    gneg();
    return ta;
}

// Force an expression.
static char* fexp()
{
    match('(');
    expression(NULL, true);
    match(')');
    return dup(")");
}

// Load digit.
static char* ldig()
{
    char* tb = dig();
    gldig(tb);
    return tb;
}

// Saves a copy of this frame then moves this frame to next.
static void move(const int args)
{
    gfpush();
    for(int i = 0; i < args; i++)
        print("\tLD V%1X,V%1X", i, v + i);
}

// Return.
static void sret()
{
    expression(NULL, true);
    match(';');
    gfpop();
}

// Draw. Hardcoded inline for performance.
static void draw()
{
    const int args = 2;
    // The first two arguments are expressions for x, y.
    for(int i = 0; i < args; i++)
    {
        expression(NULL, true);
        incv();
        match(',');
    }
    // The third argument is a label name for the sprite to draw.
    // Draw is the only function that can take a label as an argument.
    char* label = name();
    const int index = find(label);
    if(index == -1)
        bomb("label '%s' not defined", label);
    print("\tLD I,%s", labels[index].name);
    // Optionally, appending square brackets to the label will index the sprite.
    skip();
    if(now == '[')
    {
        const int b = branch++;
        match('[');
        expression(NULL, true);
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

// Clear screen. Inlined for performance.
static void clear()
{
    print("\tCLS");
}

// Generate function call.
static void gfcall(const char* name)
{
    const int i = find(name);
    if(i == -1)
        bomb("function '%s' not defined", name);
    int args = 0;
    while(now != ')')
    {
        expression(NULL, true);
        incv();
        args++;
        skip();
        if(now == ',')
            match(',');
        else
        if(now == ')')
            break;
        else bomb("unknown symbol in argument list");
    }
    if(labels[i].args != args)
        bomb("argument mismatch when calling '%s'", name);
    v -= args;
    move(args);
    print("\tCALL %s", name);
}

// Generate size.
static void szof()
{
    char* n = name();
    const int index = find(n);
    if(index == -1)
        bomb("expected label but got %s", n);
    free(n);
    skip();
    if(now == '[')
    {
        match('[');
        match(']');
        print("\tLD VF,0x%02X", labels[index].height);
    }
    else
        print("\tLD VF,0x%02X", labels[index].args);
}

// Function call.
static void fcall(const char* name)
{
    match('(');
    // Built in functions will inline.
    eql(name, "draw")   ? draw() :
    eql(name, "sizeof") ? szof() :
    eql(name, "clear")  ? clear() : gfcall(name);
    match(')');
    // Load return value.
    print("\tLD V%1X,VF", v);
}

// Returns true if the string is a token.
static bool istoken(char* s)
{
    return eql(s, "while") || eql(s, "if") || eql(s, "auto") || eql(s, "return");
}

// Load name.
static char* lname()
{
    char* ta = name();
    if(istoken(ta))
        return ta;
    skip();
    now == '(' ? fcall(ta) : glname(ta);
    return ta;
}

// Terms may start with a prefix modifier, an alpha for a name load
// or a digit for a digit load. A term may be forced into an expression
// with surrounding round brackets.
static char* term()
{
    skip();
    return
    // Prefix modifiers.
    now == '~' ? inv  () :
    now == '+' ? pos  () :
    now == '!' ? notl () :
    now == '-' ? neg  () :
    // Name load.
    isalpha(now) ? lname() :
    // Digit load.
    isdigit(now) ? ldig () :
    // Term is expression (enclosed in brackets).
    now == '(' ? fexp() : peeks();
    /* Returns the string in all cases, even if nothing was done. */
}

// Operate.
// Operators work on chained variables from V0 to VD (inclusive).
static void operate(char* o, char* t)
{
    eql(o, "=" ) ? geql     (t) :
    eql(o, "+=") ? gaddeql  (t) :
    eql(o, "-=") ? gsubeql  (t) :
    eql(o, "^=") ? gxoreql  (t) :
    eql(o, "&=") ? gandeql  (t) :
    eql(o, "|=") ? goreql   (t) :
    eql(o, "+" ) ? gadd     ()  :
    eql(o, "-" ) ? gsub     ()  :
    eql(o, "&" ) ? gand     ()  :
    eql(o, "^" ) ? gxor     ()  :
    eql(o, "|" ) ? gor      ()  :
    eql(o, "<" ) ? glt      ()  :
    eql(o, "<=") ? glteqlto ()  :
    eql(o, "!=") ? gneqlto  ()  :
    eql(o, "==") ? geqlto   ()  :
    eql(o, "||") ? gmove    ()  :
    eql(o, "&&") ? gmove    ()  :
    eql(o, "!" ) ? gnotl    ()  :
    eql(o, ">" ) ? ggt      ()  :
    eql(o, ">=") ? ggteqlto ()  : bomb("unknown operator '%s'", o);
    /* Exits if operator is not found. */
}

// Names start with alpha characters or underscores.
static bool isname(const char* s)
{
    if(s == NULL)
        bomb("derefereced a null pointer.");
    return isalpha(s[0]) || s[0] == '_';
}

// TA O TB O TA ...
static void expression(char* overrider, const bool shortable)
{
    // Assume term is not an L-value.
    bool lvalue = false;
    // Goes high when using logical operators.
    bool shorting = false;
    const int b = branch++;
    char* ta = overrider ? overrider : term();
    // If term is a name then it is an L-value.
    if(isname(ta))
        lvalue = true;
    // If the name is a label then it is not an L-value.
    if(find(ta) != -1)
        lvalue = false;
    skip();
    while(!isendexpr())
    {
        incv();
        char* o = op();
        // If the operator is an assignment operator, then
        // a new expression is computed. lvalue is first checked.
        if(eql(o, "=" )
        || eql(o, "+=")
        || eql(o, "-=")
        || eql(o, "^=")
        || eql(o, "&=")
        || eql(o, "|="))
        {
            if(lvalue == false)
                bomb("expected lvalue to the left of operator '%s'", o);
            expression(NULL, true);
        }
        // If the operator is a short circuit operator a new
        // expression is computed. Given this, the expression will turn
        // logical at the end of each subsequent expression.
        else if(eql(o, "||") || eql(o, "&&"))
        {
            shorting = true;
            print("\t%s V%1X,0x00", eql(o, "||") ? "SE" : "SNE", v - 1);
            print("\tJP END%d", b);
            expression(NULL, false);
        }
        char* tb = term();
        // If this is the second term of the expression, then there is
        // no way that this expression is l-value correct in case a future
        // assignment operator is encountered.
        lvalue = false;
        // Operate on the two terms and let the magic happen.
        operate(o, ta);
        decv();
        free(o);
        free(ta);
        ta = tb;
    }
    if(shorting)
    {
        print("END%d:", b);
        if(shortable)
            print("\tSE V%1X,0x00", v),
            print("\tLD V%1X,0x01", v);
    }
    free(ta);
}

// Generate array.
// Returns the number of elements in the array.
static int garr()
{
    match('{');
    int size = 0;
    skip();
    while(now != '}')
    {
        char* d = dig();
        print("\tDB 0x%02X", tobyte(d));
        size++;
        free(d);
        skip();
        // Bytes are separated by commas.
        // The last byte can be comma but does not have to be.
        if(now == ',')
        {
            match(',');
            skip();
            if(now == '}') break;
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
    skip();
    int size = 0;
    int height = 0;
    // Array of sprite arrays.
    // eg. sprite[] = { { 0x01, 0x02 ,0x03 }, { 0x01, 0x02 ,0x03 } };
    // In this case, height = 2, and size = 3.
    if(now == '[')
    {
        match('[');
        match(']');
        match('=');
        match('{');
        size = garr();
        skip();
        height++;
        while(now == ',')
        {
            match(',');
            skip();
            if(now == '}')
                break;
            if(garr() != size)
                bomb("array elements must be same length");
            height++;
            skip();
        }
        match('}');
    }
    // Simple sprite array.
    // eg. sprite = { 0x01, 0x02, 0x03 };
    // In this case, height = 1, and size = 3.
    else
    {
        match('=');
        height = 1;
        size = garr();
    }
    match(';');
    struct label sprite = { name, size, height };
    labels[l++] = sprite;
}

// Declaring an identifier.
// I really hate pointer integers arguments.
static void dident(int* const idents)
{
    do
    {
        char* n = name();
        isndef(n);
        vars[v] = n;
        // Note equal signs are required when creating identifiers.
        // This will prevent uninitialized variables from cropping up.
        match('=');
        expression(NULL, true);
        incv();
        *idents += 1;
        // If identifiers are separated by commas, keep going.
        if(now == ',') match(',');
    }
    while(now != ';');
    match(';');
}

// Declaring a while loop statement.
static void swhile()
{
    const int b = branch++;
    match('(');
    print("WHILE%d:", b);
    expression(NULL, true);
    print("\tSNE V%1X,0x00", v);
    print("\tJP END%d", b);
    match(')');
    dblock();
    print("\tJP WHILE%d", b);
    print("END%d:", b);
}

// Declaring an if statement.
static void sif()
{
    const int b = branch++;
    match('(');
    expression(NULL, true);
    print("\tSNE V%1X,0x00", v);
    print("\tJP ELSE%d", b);
    match(')');
    dblock();
    print("\tJP END%d", b);
    print("ELSE%d:", b);
    skip();
    // The else statement.
    if(now == 'e')
    {
        char* e = name();
        if(!eql(e, "else"))
            bomb("expected 'else'");
        free(e);
        skip();
        // If statements can come right after an else.
        if(now == 'i')
        {
            char* i = name();
            if(!eql(i, "if"))
                bomb("expected 'if'");
            free(i);
            sif();
        }
        else dblock();
    }
    print("END%d:", b);
}

// Declaring a block.
// Blocks hold statements but also declare and define identifiers.
static void dblock()
{
    int idents = 0;
    match('{');
    skip();
    while(now != '}')
    {
        char* ta = term();
        eql(ta, "{")      ? (free(ta), dblock ()       ) :
        eql(ta, "while")  ? (free(ta), swhile ()       ) :
        eql(ta, "if")     ? (free(ta), sif    ()       ) :
        eql(ta, "auto")   ? (free(ta), dident (&idents)) :
        eql(ta, "return") ? (free(ta), sret   ()       ) : (expression(ta, true), match(';'));
        /* An expression is computed as a last restort. */
        skip();
    }
    match('}');
    pop(idents);
}

// Defining a function.
static void fun(char* n)
{
    isndef(n);
    match('(');
    skip();
    int args = 0;
    while(now != ')')
    {
        char* arg = name();
        isndef(arg);
        skip();
        // Arguments can be separated by a comma or a closing paren.
        // If its a closing paren then there are no more args.
        if(now == ',' || now == ')')
        {
            if(now == ',') match(',');
            vars[v] = arg;
            incv();
            args++;
        }
        else bomb("unknown symbol in argument list");
        skip();
    }
    // Labels contain a 'height' field. This only pertains
    // to a collection of sprite arrays for a label.
    struct label label = { n, args, 0 };
    labels[l++] = label;
    print("%s:", n);
    match(')');
    dblock();
    // Note the popping here occurs after the block statement. The block statement
    // clears up any internal identifiers, so fpop will return V0 into VF.
    gfpop();
    reset();
}

// Declaring a program.
// Programs are defined by functions and spirte arrays.
static void program()
{
    while(now != EOF)
    {
        char* n = name();
        skip();
        switch(now)
        {
        case '[':
        case '=':
            arr(n);
            break;
        case '(':
            fun(n);
            break;
        default:
            bomb("symbol '%c' not known", now);
            break;
        }
        skip();
    }
}

// Rock and Roll, baby.
int main(int argc, char* argv[])
{
    if(argc != 3)
        bomb("expected input and output file");
    init(argv);
    program();
}
