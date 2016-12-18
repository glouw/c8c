#include "files.h"

#include "flags.h"

static FILE* fi;
static FILE* fo;
static char* assem;
static char* hexid;

static FILE* input(void)
{
    return fi;
}

static FILE* output(void)
{
    return fo;
}

static void destruct(void)
{
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(flags.any)
        remove(hexid);
}

static void construct(void)
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

const struct files files = {
    .input = input,
    .output = output,
    .construct = construct,
    .open = open
};
