#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "tree.h"
#include "scan.h"

static struct node* tree;
static FILE* assem;
static FILE* hexid;

static void clean(int error, void* arg)
{
    char* oput = (char*)arg;
    burn(tree);
    fclose(assem);
    fclose(hexid);
    if(error)
        remove(oput);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "error: requires both an input and output file\n");
        return 1; // Do not exit as there is nothing to clean
    }
    char* iput = argv[1];
    char* oput = argv[2];
    on_exit(clean, oput);
    assem = fopen(iput, "r");
    hexid = fopen(oput, "w");
    // First pass
    tree = scan(NULL, assem, hexid);
    // Reset vector
    struct node* reset = get(tree, "MAIN");
    if(reset)
        fprintf(hexid, "1%03X\n", reset->address);
    else
    {
        fprintf(stderr, "error: entry point not found");
        exit(1);
    }
    // Second pass
    rewind(assem);
    scan(tree, assem, hexid);
    // Done
    exit(0);
}
