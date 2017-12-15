#include "translate.h"

#include "feed.h"
#include "io.h"
#include "ident.h"

#include <ctype.h>
#include <stdlib.h>

// Register Pointer: Registers make up the stack
static int rp = 0;

// Branching label record.
static int ifs = 0;
static int elses = 0;

// BNF recursive requires these functions entirely available to this file.
static void expression(), block();

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
            char* name = feed.name();
            const Node* const found = ident.find(name);
            if(!found)
            {
                io.print("%s: not defined", name);
                free(name);
                io.bomb("exiting...");
            }
            free(name);
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
    }
    rp--;
}

// <Expression> ::= <Term> [<Operation> <Term>]*;
static void expression()
{
    term();
    while(feed.isop())
        operate(feed.peek());
    feed.match(';');
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
    rp++;
}

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

static void skip(Node* a, char* opcode)
{
    if(isdigit(feed.peek()))
        io.emit("%s V%d,0x%02X",
            opcode, a->rp, feed.number());
    else
    {
        Node* b = lookup(feed.name());
        io.emit("%s V%d,V%d",
            opcode, a->rp, b->rp);
    }
}

// if(a == b)
static void condition()
{
    feed.matches("if");
    feed.match('(');
    if(!isalpha(feed.peek()))
        io.bomb("expected name before comparison operator");
    Node* a = lookup(feed.name());
    if(feed.peek() == '=')
    {
        feed.matches("==");
        skip(a, "SE");
    }
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
    int identifiers = 0;
    while(feed.peek() != '}')
    {
        switch(feed.peek())
        {
        case '{':
            block();
            break;
        case 'l':
            identifiers ++;
            identifier();
            break;
        case 'i':
            condition();
            io.print("EIF%d:", ifs);
            ifs++;
            break;
        default:
            break;
        }
    }
    feed.match('}');
    // Cleanup - just pop, don't care about contents.
    for(int i = 0; i < identifiers; i++)
    {
        rp--;
        ident.pop();
    }

}

// <Program> ::= [<Block>]*
static void program()
{
    while(!feed.end()) block();
}

const struct translate translate = { program };
