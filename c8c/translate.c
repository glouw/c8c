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
    if(isalpha(feed.peek()))
    {
        Node* const other = lookup(feed.name());
        io.emit("ADD V%1X,V%1X", node->rp, other->rp);
        io.emit("LD  V%1X,V%1x", rp, node->rp);
    }
    else
    {
        io.emit("ADD V%1X,0x%X", node->rp, feed.number());
        io.emit("LD  V%1X,V%1x", rp, node->rp);
    }
}

static void name()
{
    Node* const node = lookup(feed.name());
    switch(feed.peek())
    {
    case '=':
        feed.match('=');
        assign(node);
        break;
    case '(':
        feed.match('(');
        call(node);
        feed.match(')');
        break;
    case '+':
        feed.match('+');
        if(feed.peek() == '+')
        {
            feed.match('+');
            incpost(node);
        }
        else
        if(feed.peek() == '=')
        {
            feed.match('=');
            inc(node);
        }
        break;
    case '-':
        feed.match('-');
        if(feed.peek() == '-')
        {
            feed.match('-');
            decpost(node);
        }
        break;
    default:
        load(node);
        break;
    }
}

static void number()
{
    switch(feed.peek())
    {
    case '(':
        feed.match('(');
        expression();
        feed.match(')');
        break;
    case '~':
        feed.match('~');
        invert();
        break;
    case '+':
        feed.match('+');
        if(feed.peek() == '+')
        {
            feed.match('+');
            incpre();
        }
        else
        if(feed.peek() == '=')
        {
            feed.match('=');
            term();
        }
        else term();
        break;
    case '-':
        feed.match('-');
        if(feed.peek() == '-')
        {
            feed.match('-');
            decpre();
        }
        else twoscomp();
        break;
    default:
        nload();
        break;
    }
}

// Computes a BNF term.
static void term()
{
    isalpha(feed.peek()) ? name() : number();
}

// Operates on a BNF term.
static void operation()
{
    push();
    int op = feed.peek();
    feed.match(op);
    if(op == '<' && feed.peek() == '<') feed.match('<'), op = 'l';
    if(op == '>' && feed.peek() == '>') feed.match('>'), op = 'r';
    if(op == '!' && feed.peek() == '=') feed.match('='), op = 'n';
    if(op == '=' && feed.peek() == '=') feed.match('='), op = 'e';
    term();
    switch(op)
    {
    case '+': io.emit("ADD V%1X,V%1X", rp - 1, rp); break;
    case '-': io.emit("SUB V%1X,V%1X", rp - 1, rp); break;
    case '|': io.emit("OR  V%1X,V%1X", rp - 1, rp); break;
    case '&': io.emit("AND V%1X,V%1X", rp - 1, rp); break;
    case '^': io.emit("XOR V%1X,V%1X", rp - 1, rp); break;
    case 'r': io.emit("SHR V%1X,V%1X", rp - 1, rp); break; /* >> */
    case 'l': io.emit("SHL V%1X,V%1X", rp - 1, rp); break; /* << */
    case '<':
        io.emit("SUB V%1X,V%1X", rp - 1, rp);
        io.emit("LD V%1X,VF", rp);
        break;
    case '>':
        io.emit("SUBN V%1X,V%1X", rp - 1, rp);
        io.emit("LD V%1X,VF", rp);
        break;
    case 'n': /* != */
        io.emit("XOR V%1X,V%1X", rp - 1, rp);
        break;
    case 'e': /* == */
        io.emit("XOR V%1X,V%1X", rp - 1, rp);
        io.emit("XOR V%1X,0xFF", rp);
        break;
    default:
        io.bomb("operation (%c) not supported", op);
        break;
    }
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
            puts("here");
            feed.matches("return");
            puts("here");
            expression();
            puts("here");
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
    io.print("%s;%d / %d bytes used%s\n", GRN, io.size(), 0x1000, NRM);
}

const struct translate translate = { program };
