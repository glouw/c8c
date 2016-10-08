#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "tree.h"
#include "scanner.h"

int main(int argc, char* argv[])
{
    file.open(argc, argv);
    // First pass
    struct node* labels = scanner.scan(NULL);
    // Reset vector
    struct node* reset = tree.get(labels, "MAIN");
    if(!reset)
    {
        fprintf(stderr, "error: entry point MAIN not found\n");
        tree.burn(labels);
        exit(1);
    }
    fprintf(file.output, "1%03X\n", reset->address);
    // Second pass
    rewind(file.input);
    scanner.scan(labels);
    // Cleanup
    tree.burn(labels);
    exit(0);
}
