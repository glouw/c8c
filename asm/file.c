#include "file.h"

#include <stdio.h>
#include <stdlib.h>

static FILE* fi;
static FILE* fo;

static FILE* input(void)
{
    return fi;
}

static FILE* output(void)
{
    return fo;
}

static void close(const int error, void* argv)
{
    if(fi)
        fclose(fi);
    if(fo)
        fclose(fo);
    switch(error)
    {
        case 0: break;
        case 1: break;
        case 2: remove(argv); break;
    }
}

static void open(char* argv[static 3])
{
    on_exit(close, argv[2]);
    fi = fopen(argv[1], "r");
    fo = fopen(argv[2], "w");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", argv[1]);
        exit(1);
    }
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", argv[2]);
        exit(2);
    }
}

const struct file file = {
    .input = input,
    .output = output,
    .open = open
};
