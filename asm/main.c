#include "file.h"
#include "tree.h"
#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "error: requires input.asm, output.hex\n");
        return 1;
    }
    file.open(argv);
    // First pass
    struct node* labels = scanner.scan(NULL);
    // Reset vector
    const char* entry = "MAIN";
    const struct node* reset = tree.get(labels, entry);
    if(!reset)
    {
        fprintf(stderr, "error: entry point %s not found\n", entry);
        tree.burn(labels);
        exit(2);
    }
    fprintf(file.output(), "1%03X\n", reset->address);
    // Second pass
    rewind(file.input());
    scanner.scan(labels);
    // All's well that ends well
    tree.burn(labels);
    exit(0);
}
