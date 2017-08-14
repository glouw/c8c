#include "translate.h"

#include "feed.h"
#include "io.h"

#include <ctype.h>

// Register Pointer: Registers make up the stack
static int rp;

// Single character variable names
static char name[128];

static void names()
{
    for(int i = 'a'; isalpha(i); i++)
        io.print("%c: V%1X", i, name[i]);
}

static void push() { rp++; }
static void pull() { rp--; } // Pop

static void term()
{
    if(feed.peek() == '(')
    {
        feed.match('(');
        translate.expression();
        feed.match(')');
    }
    else io.emit("LD V%1X,0x%02X", rp, feed.number());
}

static void operate(const char op)
{
    push();
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
    }
    pull();
}

// <Expression> ::= <Term> [<Operation> <Term>]*
static void expression()
{
    term();
    while(feed.isop())
        operate(feed.peek());
}

// <Ident> ::= <Expression>
static void ident()
{
    feed.match('!');
    name[(int) feed.name()] = rp;
    feed.match('=');
    expression();
    // Assignments must end with a new line
    feed.match('\n');
    // Saving a name pushes the stack
    push();
}

const struct translate translate = { expression, ident, names };
