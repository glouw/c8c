#include "translate.h"

#include "feed.h"
#include "io.h"
#include <ctype.h>

// Register Pointer: Registers make up the stack
static int rp;

// Single character variable names
char names[128];

static void print_names() { for(unsigned i = 0; i < sizeof(names); i++) if(isalpha(i)) io.print("%c: %d", i, names[i]); }

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
    else io.emit("LD V%1X, 0x%02X", rp, feed.number());
}

static void operate(const char op)
{
    push();
    feed.match(op);
    term();
    // Operations associate left to right
    switch(op)
    {
        case '+': io.emit("ADD V%1X, V%1X", rp - 1, rp); break;
        case '-': io.emit("SUB V%1X, V%1X", rp - 1, rp); break;
        case '|': io.emit("OR  V%1X, V%1X", rp - 1, rp); break;
        case '&': io.emit("AND V%1X, V%1X", rp - 1, rp); break;
        case '^': io.emit("XOR V%1X, V%1X", rp - 1, rp); break;
    }
    pull();
}

// <Expression> ::= <Term> [<Operation> <Term>]*
static void expression() { term(); while(feed.isop()) operate(feed.peek()); }

// <Ident> ::= <Expression>
static void ident()
{
    feed.match(':');
    names[(int) feed.name()] = rp;
    feed.match('=');
    expression();
    // Saving a name pushes the stack
    push();
    // Assignments must end with a new line
    feed.match('\n');
}

const struct translate translate = { expression, ident, print_names };
