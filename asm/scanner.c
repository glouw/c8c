#include "scanner.h"

#include "opcodes.h"
#include "tree.h"
#include "errors.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static struct node* scan(struct node* labels)
{
    unsigned size = 0;
    unsigned address = 0x0202;
    char* line = NULL;
    bool grow = labels ? false : true;
    for(unsigned linenumber = 1; getline(&line, &size, file.input) != -1; linenumber++)
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
            {
                labels = tree.insert(labels, tree.build(label, address));
                if(tree.poisoned)
                {
                    fprintf(stderr, "error: line %d: %s already defined\n", linenumber, label);
                    free(line);
                    tree.burn(labels);
                    exit(1);
                }
            }
        }
        mnemonic = strtok(colon ? NULL : line, "\t ");
        operand = strtok(NULL, "");
        if(mnemonic)
        {
            if(grow)
                address += strcmp(mnemonic, "DB") == 0 ? 0x0001 : 0x0002;
            else
                error = opcodes.assemble(mnemonic, operand, labels);
        }
        if(error)
        {
            errors.handle(error, linenumber);
            free(line);
            tree.burn(labels);
            exit(1);
        }
    }
    free(line);
    return labels;
}

struct scanner scanner = {
    .scan = scan
};
