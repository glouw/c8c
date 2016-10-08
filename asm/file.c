#include "file.h"

#include <stdio.h>
#include <stdlib.h>

static void close(int type, void* hexidecimal)
{
    switch(type)
    {
        // Good ending
        case 0:
            fclose(file.input);
            fclose(file.output);
            break;
        // Bad ending
        case 1:
            fclose(file.input);
            fclose(file.output);
            remove(hexidecimal);
            break;
        // Internal to this: input and output failed to open
        case 2:
            break;
    }
}

static void open(int argc, char* argv[])
{
    char* assembly = argv[1];
    char* hexidecimal = argv[2];
    on_exit(close, hexidecimal);
    if(argc != 3)
    {
        fprintf(stderr, "error: requires an input .asm file and an output .hex file\n");
        exit(2);
    }
    // Try opening the input file
    file.input = fopen(assembly, "r");
    if(file.input == NULL)
    {
        fprintf(stderr, "error: could not open input %s\n", assembly);
        exit(2);
    }
    // Try opening the output file
    file.output = fopen(hexidecimal, "w");
    if(file.output == NULL)
    {
        fprintf(stderr, "error: could not open output file %s\n", hexidecimal);
        fclose(file.input);
        exit(2);
    }
}

struct file file = {
    .open = open,
    .input = NULL,
    .output = NULL
};
