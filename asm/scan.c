#include "scan.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assemble.h"

struct node* scan(struct node* tree, FILE* assem, FILE* hexid)
{
    unsigned size = 0;
    unsigned address = 0x0202;
    char* line = NULL;
    bool grow = tree ? false : true;
    for(unsigned linenumber = 1; getline(&line, &size, assem) != -1; linenumber++)
    {
        char* label;
        char* mnemonic;
        char* operand;
        char* newline;
        char* semicolon;
        char* colon;
        int error = 0;
        newline = index(line, '\n');
        if(newline)
            *newline = '\0';
        semicolon = index(line, ';');
        if(semicolon)
            *semicolon = '\0';
        colon = index(line, ':');
        if(colon)
        {
            label = strtok(line, "\t :");
            if(grow)
                tree = insert(tree, new(label, address));
        }
        mnemonic = strtok(colon ? NULL : line, "\t ");
        operand = strtok(NULL, "");
        if(mnemonic)
        {
            if(grow)
                address += strcmp(mnemonic, "DB") == 0 ? 0x0001 : 0x0002;
            else
                error = assemble(hexid, mnemonic, operand, tree);
        }
        // Error handling
        char* message = NULL;
        switch(error)
        {
            case 1: message = "error: line %d: operand formatting\n"; break;
            case 2: message = "error: line %d: label not found\n"; break;
            case 3: message = "error: line %d: unsupported chip8 mnemonic\n"; break;
        }
        if(error)
        {
            fprintf(stderr, message, linenumber);
            free(line);
            exit(1);
        }
    }
    free(line);
    return tree;
}
