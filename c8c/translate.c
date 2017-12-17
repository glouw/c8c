#include "translate.h"

#include "feed.h"
#include "io.h"
#include "ident.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Register Pointer: Registers make up the stack.
static int rp;
static void push() { if(rp++ > 0xD) io.bomb("register stack overflow");  }
static void pull() { if(rp-- < 0x0) io.bomb("register stack underflow"); }

// Branching label record.
static int ifs;
static int elses;
static int whiles;

// BNF recursion requires these functions entirely available to this file.
static void expression(), block();

static Node* lookup(char* const name)
{
    Node* const node = ident.find(name);
    if(!node)
    {
        io.print("%s not defined", name);
        free(name);
        io.bomb("exiting...");
    }
    free(name);
    return node;
}

// Pushes the stack to the stack frame.
static void spush()
{
    io.emit("LD F,VE");
    io.emit("LD [I],VE");
    // Incrememnts the stack frame pointer.
    io.emit("ADD VE,0x03");
}

// Pops a stack from the stack frame.
static void spop()
{
    // Decrements the stack frame pointer.
    io.emit("SUB VE,0x03");
    io.emit("LD F,VE");
    io.emit("LD VE,[I]");
    // Load return value.
    io.emit("LD VF,V%d", rp);
    // Return.
    io.emit("RET");
}

static void call(Node* const node)
{
    // Compute any argument expressions on local stack.
    feed.match('(');
    int args = 0;
    while(feed.peek() != ')')
    {
        expression();
        push();
        args++;
        if(feed.peek() == ')')
            break;
        else feed.match(',');
    }
    if(args != node->rp)
        io.bomb("argument mismatch");
    rp -= args;
    feed.match(')');
    // Push a copy of the local stack for the return.
    spush();
    // Move arguments.
    for(int i = 0; i < args; i++)
        io.emit("LD V%1X,V%1X", i, rp + i);
    // Call.
    io.emit("CALL %s", node->name);
    // Get return value once returned.
    io.emit("LD V%d,VF", rp);
}

// Forced expression.
static void fexpression()
{
    feed.match('(');
    expression();
    feed.match(')');
}

// Name assignment.
static void assign(Node* node)
{
    feed.match('=');
    expression();
    io.emit("LD V%1X,V%1X", node->rp, rp);
}

// Name load.
static void nload(Node* node)
{
    io.emit("LD V%1X,V%1X", rp, node->rp);
}

// Name operation.
static void nameop()
{
    Node* const node = lookup(feed.name());
    feed.peek() == '=' ? assign(node) : feed.peek() == '(' ? call(node) : nload(node);
}

// Value load.
static void vload()
{
    io.emit("LD V%1X,0x%02X", rp, feed.number());
}

// Computes a BNF term.
static void term()
{
    feed.peek() == '(' ? fexpression() : isalpha(feed.peek()) ? nameop() : vload();
}

char* gop(const int op)
{
    switch(op)
    {
        case '+': return "ADD";
        case '-': return "SUB";
        case '|': return "OR ";
        case '&': return "AND";
        case '^': return "XOR";
        case 'r': return "SHR";
        case 'l': return "SHL";
        case '<':
            io.bomb("chip8: operator (<) not supported");
            break;
        case '>':
            io.bomb("chip8: operator (>) not supported");
            break;
        default:
            io.bomb("operation (%c) not supported", op);
            break;
    }
    return NULL;
}

// Operates on a BNF term.
static void operation()
{
    push();
    int op = feed.peek();
    feed.match(op);
    if(op == '<' && feed.peek() == '<') feed.match('<'), op = 'l';
    if(op == '>' && feed.peek() == '>') feed.match('>'), op = 'r';
    term();
    io.emit("%s V%1X,V%1X", gop(op), rp - 1, rp);
    pull();
}

// <Expression> ::= <Term> [<Operation> <Term>]*;
static void expression()
{
    term();
    while(!feed.isends()) operation();
}

// <Identifier> ::= <Expression>
static void identifier()
{
    feed.matches("let");
    while(feed.peek() != ';')
    {
        char* name = feed.name();
        Node* found = ident.find(name);
        // Push anyway for exit time cleanup if not found.
        ident.push(ident.create(name, rp));
        if(found)
            io.bomb("%s already defined", name);
        feed.match('=');
        expression();
        push();
        if(feed.peek() == ';')
            break;
        else feed.match(',');
    }
    feed.match(';');
}

// Defines a function.
// def name(args, ...)
static void function()
{
    char* name = feed.name();
    Node* found = ident.find(name);
    Node* where = ident.push(ident.create(name, 0));
    if(found)
        io.bomb("%s already defined", name);
    io.print("%s:", name);
    feed.match('(');
    const int stamp = rp;
    while(feed.peek() != ')')
    {
        ident.push(ident.create(feed.name(), rp));
        push();
        if(feed.peek() == ')')
            break;
        else feed.match(',');
    }
    feed.match(')');
    where->rp = rp - stamp;
}

static void vcmp(const Node* a, const char* opcode)
{
    expression();
    io.emit("%s V%d,V%d", opcode, a->rp, rp);
}

static void ncmp(const Node* a, const char* opcode)
{
    const Node* b = lookup(feed.name());
    io.emit("%s V%d,V%d", opcode, a->rp, b->rp);
}

static void cmp(const Node* a, const char* opcode)
{
    isalpha(feed.peek()) ? ncmp(a, opcode) : vcmp(a, opcode);
}

static void se(Node* a)
{
    feed.matches("=="), cmp(a, "SE");
}

static void sne(Node* a)
{
    feed.matches("!="), cmp(a, "SNE");
}

static void comparison(Node* a)
{
    feed.peek() == '=' ? se(a) : feed.peek() == '!' ? sne(a) : io.bomb("chip8 supports (!=) (==)");
}

// if(a == b)
// if(a != b)
// These comparisons are not treated as stack entities
// as the chip8 uses specific Skip if Equal (SE) or Skip if Not
// Equal (SNE) opcode for branching.
// There is no less than or greater than comparison operators.
static void condition()
{
    feed.matches("if");
    feed.match('(');
    if(!isalpha(feed.peek()))
        io.bomb("names are required before the comparison operator");
    Node* a = lookup(feed.name());
    comparison(a);
    feed.match(')');
    // First "if" statment.
    io.emit("JP ELS%d", elses);
    block();
    io.emit("JP EIF%d", ifs);
    // First "else" statement.
    io.print("ELS%d:", elses);
    if(feed.peek() == 'e')
    {
        feed.matches("else");
        elses++;
        // Continued "else if" statements.
        if(feed.peek() == 'i')
            condition();
        else block();
    }
}

static void wloop()
{
    const int end = whiles;
    feed.matches("while");
    io.print("WH%d:", whiles++);
    feed.match('(');
    if(!isalpha(feed.peek()))
        io.bomb("names are required before the comparison operator");
    Node* a = lookup(feed.name());
    comparison(a);
    feed.match(')');
    io.emit("JP EWH%d", end);
    block();
    io.emit("JP WH%d", end);
    io.print("EWH%d:", end);
}

// <Block> ::= [<Statement>]*
static void block()
{
    feed.match('{');
    int idents = 0;
    while(feed.peek() != '}')
    {
        switch(feed.peek())
        {
        // Blocks within blocks.
        case '{':
            block();
            break;
        // If statements.
        case 'i':
            condition();
            io.print("EIF%d:", ifs);
            ifs++;
            break;
        // Return statements.
        case 'r':
            feed.matches("return");
            expression();
            feed.match(';');
            spop();
            break;
        // Let statements for identifiers.
        case 'l':
            idents++;
            identifier();
            break;
        // While loop.
        case 'w':
            wloop();
            break;
        // Default expressions without identifiers.
        default:
            expression();
            feed.match(';');
            break;
        }
    }
    feed.match('}');
    // Pop identifiers.
    for(int i = 0; i < idents; i++)
    {
        pull();
        ident.pop();
    }
}

// Defines a top level definition.
static void definition()
{
    if(feed.peek() != 'd')
        io.bomb("expected function definition");
    feed.matches("def");
    function();
    block();
    spop();
    // Pop arguments.
    while(rp)
    {
        pull();
        ident.pop();
    }
}

// There is no chip8 stack for stack frames,
// but one can be emulated by statically reserving
// space from 0x200 onwards. Due to the fact that sprites are
// one byte wide and five bytes tall, opcode FX29 can
// be exploited to set I by multiples of 5
// within the range of 0x000 - 0x4FB. Addresses 0x000 to 0x200
// are reserved for the character sprite data, as well as the supposed
// interpreter, and 0x4FB is far too large, so addresses
// 0x200 - 0x32C will be used for the stack.
//
// The chip8 uses instructions FX55 and FX65 to write and
// read registers V0-VX to memory starting at the address value
// stored in I. Since flag VF is used for flags by the chip8,
// 15 registers (V0-VE) are available for the stack frames.
// This register payload is a multiple of 5,
// and VE will be used as the stack frame pointer,
// starting at 0x200 and ending at 0x32C (300 bytes big).
static void init()
{
    io.print("STACK:");
    const int start = 0x200;
    const int bytes = 300;
    for(int i = 0; i < bytes; i++)
        io.emit("DB 0x00");
    io.print("start:");
    const int payload = 15;
    io.emit("LD VE,0x%02X", start / payload);
    spush();
    io.emit("CALL main");
    // Will never leave start.
    io.print("DONE:");
    io.emit("JP DONE");
}

// <Program> ::= [<Definition>]*
static void program()
{
    init();
    while(!feed.end()) definition();
    // The program size is printed at the very end.
    // Two bytes per instruction.
    io.print(";%d / %d bytes used\n", io.size(), 0x1000);
}

const struct translate translate = { program };
