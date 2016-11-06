#include "file.h"

#include <stdio.h>
#include <stdlib.h>

FILE* input;
FILE* output;

static void close(int type, void* hexidecimal)
{
    switch(type)
    {
        // Happy ending
        case 0:
            fclose(input);
            fclose(output);
            break;
        // Unhappy ending
        case 1:
            fclose(input);
            fclose(output);
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
    input = fopen(assembly, "r");
    if(input == NULL)
    {
        fprintf(stderr, "error: could not open input %s\n", assembly);
        exit(2);
    }
    // Try opening the output file
    output = fopen(hexidecimal, "w");
    if(output == NULL)
    {
        fprintf(stderr, "error: could not open output file %s\n", hexidecimal);
        fclose(input);
        exit(2);
    }
}

const struct file file = { open };
