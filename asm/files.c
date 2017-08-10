#include "files.h"

#include "flags.h"

static FILE* fi;
static FILE* fo;
static char* assem;
static char* hexid;

static FILE* input()
{
    return fi;
}

static FILE* output()
{
    return fo;
}

static void destruct()
{
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(flags.any)
        remove(hexid);
}

static void construct()
{
    atexit(destruct);
}

static void open(char* argv[])
{
    assem = argv[1];
    hexid = argv[2];
    // Input
    fi = fopen(argv[1], "r");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", assem);
        exit(1);
    }
    // Output
    fo = fopen(argv[2], "w");
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s cannot be made\n", hexid);
        exit(1);
    }
}

static void reset()
{
    rewind(fi);
    rewind(fo);
}

const struct files files = { input, output, construct, open, reset };
