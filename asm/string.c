#include "string.h"

#include <string.h>
#include <stdlib.h>

static char* duplicate(char* s)
{
    int len = strlen(s) + 1;
    char* p = (char*) malloc(len);
    return p ? (char*) memcpy(p, s, len) : NULL;
}

const struct string string = { duplicate };
