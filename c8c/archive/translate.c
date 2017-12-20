#include "translate.h"

#include "feed.h"
#include "io.h"
#include "ident.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// BNF recursion requires these functions entirely available to this file.
static void expression(), block(), term();

// Register Pointer: Registers make up the stack.
static int rp;

// Branching label record.
static int ifs;
static int elses;
static int whiles;

static void push()
{
    if(rp++ > 0xD) io.bomb("register stack overflow"); 
}

static void pull()
{
    if(rp-- < 0x0) io.bomb("register stack underflow");
}

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

static void store(const int args)
{
    spush();
    for(int i = 0; i < args; i++)
        io.emit("LD V%1X,V%1X", i, rp + i);
}

static void call(Node* const node)
{
    // Compute any argument expressions on local stack.
    int args = 0;
    while(feed.peek(0) != ')')
    {
        expression();
        push();
        args++;
        if(feed.peek(0) == ')')
            break;
        else feed.match(',');
    }
    if(args != node->rp)
        io.bomb("argument mismatch");
    rp -= args;
    store(args);
    // Call.
    io.emit("CALL %s", node->name);
    // Get return value once returned.
    io.emit("LD V%d,VF", rp);
}

static void twoscomp()
{
    term();
    io.emit("XOR V%1X,0xFF", rp);
    io.emit("ADD V%1X,0x01", rp);
}

static void incpre()
{
    Node* const node = lookup(feed.name());
    io.emit("ADD V%1X,0x01", node->rp);
    io.emit("LD  V%1X,V%1x", rp, node->rp);
}

static void decpre()
{
    Node* const node = lookup(feed.name());
    io.emit("SUB V%1X,0x01", node->rp);
    io.emit("LD  V%1X,V%1x", rp, node->rp);
}

static void invert()
{
    term();
    io.emit("XOR V%1X,0xFF", rp);
}

static void nload()
{
    io.emit("LD V%1X,0x%X", rp, feed.number());
}

static void incpost(Node* const node)
{
    io.emit("LD  V%1X,V%1x", rp, node->rp);
    io.emit("ADD V%1X,0x01", node->rp);
}

static void decpost(Node* const node)
{
    io.emit("LD  V%1X,V%1x", rp, node->rp);
    io.emit("SUB V%1X,0x01", node->rp);
}

static void assign(Node* const node)
{
    expression();
    io.emit("LD V%1X,V%1X", node->rp, rp);
}

static void load(Node* const node)
{
    io.emit("LD V%1X,V%1X", rp, node->rp);
}

static void inc(Node* const node)
{
    expression();
    io.emit("ADD V%1X,V%1X", node->rp, rp);
}

static void dec(Node* const node)
{
    expression();
    io.emit("SUB V%1X,V%1X", node->rp, rp);
}

static void shlas(Node* const node)
{
    expression();
    io.emit("SHL V%1X,V%1X", node->rp, rp);
}

static void shras(Node* const node)
{
    expression();
    io.emit("SHR V%1X,V%1X", node->rp, rp);
}

static void name()
{
    Node* const node = lookup(feed.name());
    if(feed.peek(0) == '='
    && feed.peek(1) != '=')
    {
        feed.match('=');
        assign(node);
    }
    else
    if(feed.peek(0) == '(')
    {
        feed.match('(');
        call(node);
        feed.match(')');
    }
    else
    if(feed.peek(0) == '+')
    {
        if(feed.peek(1) == '+')
        {
            feed.matches("++");
            incpost(node);
        }
        else
        if(feed.peek(1) == '=')
        {
            feed.matches("+=");
            inc(node);
        }
    }
    else
    if(feed.peek(0) == '-')
    {
        if(feed.peek(1) == '-')
        {
            feed.matches("--");
            decpost(node);
        }
        else
        if(feed.peek(1) == '=')
        {
            feed.matches("-=");
            dec(node);
        }
    }
    else
    if(feed.peek(0) == '<'
    && feed.peek(1) == '<'
    && feed.peek(2) == '=')
    {
        feed.matches("<<=");
        shlas(node);
    }
    else
    if(feed.peek(0) == '>'
    && feed.peek(1) == '>'
    && feed.peek(2) == '=')
    {
        feed.matches(">>=");
        shras(node);
    }
    else load(node);
}

static void number()
{
    if(feed.peek(0) == '(')
    {
        feed.match('(');
        expression();
        feed.match(')');
    }
    else
    if(feed.peek(0) == '~')
    {
        feed.match('~');
        invert();
    }
    else
    if(feed.peek(0) == '+')
    {
        if(feed.peek(1) == '+')
        {
            feed.matches("++");
            incpre();
        }
        else
        {
            feed.match('+');
            term();
        }
    }
    else
    if(feed.peek(0) == '-')
    {
        if(feed.peek(1) == '-')
        {
            feed.matches("--");
            decpre();
        }
        else
        {
            feed.match('-');
            twoscomp();
        }
    }
    else nload();
}

// Computes a BNF term.
static void term()
{
    isalpha(feed.peek(0)) ? name() : number();
}

static int table(const int a, const int b, const int c)
{
    if(a == '>' && b == '>' && c != '=') { feed.matches(">>"); return 0; }
    if(a == '<' && b == '<' && c != '=') { feed.matches("<<"); return 1; }
    if(a == '=' && b == '=') { feed.matches("=="); return 2; }
    if(a == '!' && b == '=') { feed.matches("!="); return 3; }
    if(a == '+' && b != '=') { feed.match('+'); return  4; }
    if(a == '-' && b != '=') { feed.match('-'); return  5; }
    if(a == '|' && b != '=') { feed.match('|'); return  6; }
    if(a == '&' && b != '=') { feed.match('&'); return  7; }
    if(a == '^' && b != '=') { feed.match('^'); return  8; }
    if(a == '<' && b != '=') { feed.match('<'); return  9; }
    if(a == '>' && b != '=') { feed.match('>'); return 10; }
    /* Not found. */
    return -1;
}

static void shr()
{
    io.emit("SHR V%1X,V%1X", rp - 1, rp);
}

static void shl()
{
    io.emit("SHL V%1X,V%1X", rp - 1, rp);
}

static void eql()
{
    io.emit("XOR V%1X,V%1X", rp - 1, rp);
    io.emit("XOR V%1X,0xFF", rp - 1);
}

static void neql()
{
    io.emit("XOR V%1X,V%1X", rp - 1, rp);
}

static void add()
{
    io.emit("ADD V%1X,V%1X", rp - 1, rp);
}

static void sub()
{
    io.emit("SUB V%1X,V%1X", rp - 1, rp);
}

static void or()
{
    io.emit("OR V%1X,V%1X", rp - 1, rp);
}

static void and()
{
    io.emit("AND V%1X,V%1X", rp - 1, rp);
}

static void xor()
{
    io.emit("XOR V%1X,V%1X", rp - 1, rp);
}

static void lt()
{
    io.emit("SUB V%1X,V%1X", rp - 1, rp);
    io.emit("LD V%1X,VF", rp);
}

static void gt()
{
    io.emit("SUBN V%1X,V%1X", rp - 1, rp);
    io.emit("LD V%1X,VF", rp);
}

static void (*ops[])() = {
    shr, shl, eql, neql, add, sub, or, and, xor, lt, gt
};

static void operation()
{
    push();
    const int which = table(feed.peek(0), feed.peek(1), feed.peek(2));
    if(which == -1)
        io.bomb("no such operation");
    term();
    ops[which]();
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
    while(feed.peek(0) != ';')
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
        if(feed.peek(0) == ';')
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
    while(feed.peek(0) != ')')
    {
        ident.push(ident.create(feed.name(), rp));
        push();
        if(feed.peek(0) == ')')
            break;
        else feed.match(',');
    }
    feed.match(')');
    where->rp = rp - stamp;
}

// Executes a block if not zero.
static void condition()
{
    feed.matches("if");
    feed.match('(');
    expression();
    feed.match(')');
    // First "if" statment.
    io.emit("SNE V%1X,0x00", rp);
    io.emit("JP ELS%d", elses);
    block();
    io.emit("JP EIF%d", ifs);
    // First "else" statement.
    io.print("ELS%d:", elses);
    if(feed.peek(0) == 'e')
    {
        feed.matches("else");
        elses++;
        // Continued "else if" statements.
        if(feed.peek(0) == 'i')
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
    expression();
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
    while(feed.peek(0) != '}')
    {
        switch(feed.peek(0))
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
    if(feed.peek(0) != 'd')
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
    io.print("%s;%d / %d bytes used%s\n", GRN, io.size(), 0x1000, NRM);
}

const struct translate translate = { program };
