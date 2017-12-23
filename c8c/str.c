#include "str.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static int eql(const char* a, const char* b)
{
    if(a == NULL || b == NULL)
        return 0;
    return strcmp(a, b) == 0;
}

static char* dup(const char* s)
{
    char* dup = (char*) malloc(strlen(s) + 1);
    return dup ? strcpy(dup, s) : NULL;
}

static int ispostfix(const char* s)
{
    return strstr(s, "++")
        || strstr(s, "--");
}

static int isassign(const char* s)
{
    return strstr(s, "+=")
        || strstr(s, "-=")
        || strstr(s, "&=")
        || strstr(s, "^=")
        || strstr(s, "|=")
        || strstr(s, "<<=")
        || strstr(s, ">>=");
}

static int ischain(const char* s)
{
    return strstr(s, "+")
        || strstr(s, "-")
        || strstr(s, "&")
        || strstr(s, "^")
        || strstr(s, "|")
        || strstr(s, "<")
        || strstr(s, "=")
        || strstr(s, "!")
        || strstr(s, ">");
}

static int isname(const char* lv)
{
    return isalpha(lv[0]);
}

const struct str str = {
    eql, dup, ispostfix, isassign, ischain, isname
};
