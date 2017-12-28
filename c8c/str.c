#include "str.h"

#include "io.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

static int ispostfix(const char* s)
{
    return eql(s, "++")
        || eql(s, "--");
}

static int isassign(const char* s)
{
    return eql(s, "+=" )
        || eql(s, "-=" )
        || eql(s, "<<=")
        || eql(s, ">>=")
        || eql(s, "&=" )
        || eql(s, "|=" )
        || eql(s, "^=" );
}

static int ischain(const char* s)
{
    return eql(s, "+" )
        || eql(s, "-" )
        || eql(s, "?" )
        || eql(s, ":" )
        || eql(s, "&" )
        || eql(s, "&&")
        || eql(s, "^" )
        || eql(s, "|" )
        || eql(s, "||")
        || eql(s, "<" )
        || eql(s, "<=")
        || eql(s, "=" )
        || eql(s, "==")
        || eql(s, "<<")
        || eql(s, ">>")
        || eql(s, "!" )
        || eql(s, ">" )
        || eql(s, ">=");
}

// Names start with alpha characters or underscores.
static int isname(const char* s)
{
    if(s == NULL)
        io.bomb("derefereced a null pointer.");
    return isalpha(s[0]) || s[0] == '_';
}

static int islogical(const char* s)
{
    return eql(s, "&&" )
        || eql(s, "||" );
}

// Ternary operators.
static int istern(const char* s)
{
    return eql(s, "?" )
        || eql(s, ":" );
}

const struct str str = {
    eql, dup, ispostfix, isassign, ischain, isname, islogical, istern
};
