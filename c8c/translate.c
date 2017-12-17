#include "translate.h"

#include "feed.h"
#include "io.h"
#include "ident.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Register Pointer: Registers make up the stack
static int rp;

// Branching label record.
static int ifs;
static int elses;

// BNF recursive requires these functions entirely available to this file.
static void expression(), block();

Node* lookup(char* name)
{
    Node* node = ident.find(name);
    if(!node)
    {
        io.print("%s not defined", name);
        free(name);
        io.bomb("exiting...");
    }
    free(name);
    return node;
}

static void push()
{
    io.emit("LD F,VE");
    io.emit("LD [I],VE");
    io.emit("ADD VE,0x03");
}

static void pop()
{
    io.emit("SUB VE,0x03");
    io.emit("LD F,VE");
    io.emit("LD VE,[I]");
    io.emit("LD VF,V%d", rp); /* Load return value. */
    io.emit("RET");
}

static void call(char* name)
{
    // Push arguments.
    feed.match('(');
    int args = 0;
    while(feed.peek() != ')')
    {
        expression();
        rp++;
        args++;
        if(feed.peek() == ')')
            break;
        else feed.match(',');
    }
    rp -= args;
    feed.match(')');
    push();
    for(int i = 0; i < args; i++)
        io.emit("LD V%1X,V%1X", i, rp + i);
    io.emit("CALL %s", name);
    io.emit("LD V%d,VF", rp); /* Get return value. */
}

static void term()
{
    if(feed.peek() == '(')
    {
        feed.match('(');
        expression();
        feed.match(')');
    }
    else
    {
        // Name lookup.
        if(isalpha(feed.peek()))
        {
            Node* found = lookup(feed.name());
            // Assign name.
            if(feed.peek() == '=')
            {
                feed.match('=');
                expression();
                io.emit("LD V%1X,V%1X", found->rp, rp);
            }
            // Call name.
            else
            if(feed.peek() == '(')
                call(found->name);
            // Load name.
            else
                io.emit("LD V%1X,V%1X", rp, found->rp);
        }
        // Direct load.
        else
            io.emit("LD V%1X,0x%02X", rp, feed.number());
    }
}

static void operate(const int op)
{
    rp++;
    feed.match(op);
    term();
    // Operations associate left to right
    switch(op)
    {
        case '+': io.emit("ADD V%1X,V%1X", rp - 1, rp); break;
        case '-': io.emit("SUB V%1X,V%1X", rp - 1, rp); break;
        case '|': io.emit("OR  V%1X,V%1X", rp - 1, rp); break;
        case '&': io.emit("AND V%1X,V%1X", rp - 1, rp); break;
        case '^': io.emit("XOR V%1X,V%1X", rp - 1, rp); break;
        // What about these guys?
        // <<
        // >>
        // And these guys?
        // +=
        // -=
        // |=
        // &=
        // ^=
        // >>=
        // <<=
        // ++
        // --
    }
    rp--;
}

// <Expression> ::= <Term> [<Operation> <Term>]*;
static void expression()
{
    term();
    while(feed.isop())
        operate(feed.peek());
}

static void function()
{
    char* name = feed.name();
    Node* found = ident.find(name);
    ident.push(ident.create(name, 0));
    if(found)
        io.bomb("%s already defined", name);
    io.print("%s:", name);
    feed.match('(');
    while(feed.peek() != ')')
    {
        ident.push(ident.create(feed.name(), rp++));
        if(feed.peek() == ')')
            break;
        else feed.match(',');
    }
    feed.match(')');
}

// <Identifier> ::= <Expression>
static void identifier()
{
    feed.matches("let");
    char* name = feed.name();
    Node* found = ident.find(name);
    // Push anyway for exit time cleanup if not found.
    ident.push(ident.create(name, rp));
    if(found)
        io.bomb("%s already defined", name);
    feed.match('=');
    expression();
    feed.match(';');
    rp++;
}

static void skip(Node* a, char* opcode)
{
    if(isdigit(feed.peek()))
        io.emit("%s V%d,0x%02X", opcode, a->rp, feed.number());
    else
    {
        Node* b = lookup(feed.name());
        io.emit("%s V%d,V%d", opcode, a->rp, b->rp);
    }
}

// if(a == b)
// if(a != b)
// These comparisons are not treated as stack entities
// as the chip8 uses a specific Skip if Equal or Skip if Not
// Equal opcode for its branching. There is no less than or greater
// than comparison operators.
static void condition()
{
    feed.matches("if");
    feed.match('(');
    if(!isalpha(feed.peek()))
        io.bomb("expected name before comparison operator");
    Node* a = lookup(feed.name());
    // Skip if Equal.
    if(feed.peek() == '=')
    {
        feed.matches("==");
        skip(a, "SE");
    }
    // Skip if Not Equal
    else
    {
        feed.matches("!=");
        skip(a, "SNE");
    }
    feed.match(')');
    // First "if" statment
    io.emit("JP ELS%d", elses);
    block();
    io.emit("JP EIF%d", ifs);
    // First "else" statement
    io.print("ELS%d:", elses);
    if(feed.peek() == 'e')
    {
        feed.matches("else");
        elses++;
        // Continued "else if" statements
        if(feed.peek() == 'i')
            condition();
        else block();
    }
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
        case '{':
            block();
            break;
        case 'i':
            condition();
            io.print("EIF%d:", ifs);
            ifs++;
            break;
        case 'r':
            feed.matches("return");
            expression();
            feed.match(';');
            pop();
            break;
        case 'l':
            idents++;
            identifier();
            break;
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
        rp--;
        ident.pop();
    }
}

static void definition()
{
    if(feed.peek() != 'd')
        io.bomb("expected function definition");
    feed.matches("def");
    function();
    block();
    pop();
    // Pop arguments
    while(rp)
    {
        rp--;
        ident.pop();
    }
}

// There is no chip8 stack for stack frames,
// but one can be emulated by statically reserving
// space from 0x200 onwards. Opcode FX29 can then cleverly
// be exploited using the sprite pointer due to the fact that sprites are
// one byte wide and five bytes tall, allowing for I to be set by multiples of 5
// within the range of 0x000 - 0x4FB. Addresses 0x000 to 0x200
// are reserved for the character sprite data, as well as the supposed
// interpreter, and 0x4FB is far too large, so addresses
// 0x200 - 0x32C will be used for the stack.
//
// The chip8 uses instructions FX55 and FX65 to write and
// read registers V0-VX to memory starting at the address value
// stored in I. Since flag VF is used for flags by the chip8,
// fifteen registers (V0-VE) are available for the stack frames.
// VE will be used as the stack frame pointer,
// starting at 0x200 and ending at 0x32C (300 bytes big).
static void init()
{
    io.print("STACK:");
    const int start = 0x200;
    const int bytes = 300;
    for(int i = 0; i < bytes; i++)
        io.emit("DB 0x00");
    io.print("start:");
    // VE will always hold the previous block.
    io.emit("LD VE,0x%02X", start / 15);
    push();
    io.emit("CALL main");
    // Will never leave start.
    io.print("DONE:");
    io.emit("JP DONE");
}

// <Program> ::= [<Definition>]*
static void program()
{
    init();
    while(!feed.end())
        definition();
}

const struct translate translate = { program };
