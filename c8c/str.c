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

const struct str str = {
    eql, dup
};
