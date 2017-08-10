#include "translate.h"

#include "feed.h"
#include "io.h"

// This compiler uses the sixteen general purpose registers
// of the chip8 as a stack pointed. The register pointer points
// to a register:
static int rp;

static void push() { rp++; }
static void pull() { rp--; }

// Terms contruct expressions. Terms load a register pointed
// to by the register pointer. Terms are expressions if
// enclosed by parentheses:
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

// Expresions are denoted as a forever long train of
// operations and terms. The expression must start with a term:
// <Expression> ::= <Term> [<Operation> <Term>]*
static void operate(char op)
{
    push();
    feed.match(op);
    term();
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

static void expression()
{
    term();
    while(feed.isop()) operate(feed.peek());
}

const struct translate translate = { expression };
