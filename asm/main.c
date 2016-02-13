#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "assemble.h"
#include <assert.h>

int main(int argc, char* argv[])
{
    /* command line requires an assembly (assem) input and a hexidecimal output (hexid) */
    if(argc != 3) return 1;
    FILE* assem = fopen(argv[1], "r");
    if(assem == NULL) return 1;
    FILE* hexid = fopen(argv[2], "w");
    /* FIRST SCAN: gather labels, store in "labels" binary tree (see tree.c) */
    tnode* labels = NULL;
    /* nlabels keeps track of label count, but is only used to check if 0 for tree init */
    unsigned nlabels = 0;
    /* 0x0200 reserved for reset vector */
    unsigned address = 0x0202;
    /* get one line from assem */
    char* line = NULL; unsigned len = 0;
    while(getline(&line, &len, assem) != -1)
    {
        /* labels end with a colon - if a colon is found, put address & label pair in tree */
        char* colon = index(line, ':');
        if(colon)
        {
            *colon = '\0';
            labels = tree.add(nlabels++ ? labels : NULL, tree.new(line, address));
        }
        /* increment address if a mnemonic is found */
        char* mnemonic = strtok(colon ? colon + 1 : line, "\t \n;");
        if(mnemonic) address += 0x0002;
    }
    /* if an error occurs during assembly then fail goes high */
    unsigned error = 0, fail = 0;
    /* write reset vector to hexid if entry name is found */
    char* ename = "MAIN";
    tnode* enode = tree.get(labels, ename);
    if(enode == NULL)
    {
        fail = 1;
        fprintf(stderr, "error: %s undefined reference\n", ename);
    }
    else fprintf(hexid, "1%03X\n", enode->address);
    /* SECOND SCAN: rewind, and generate hex code */
    rewind(assem);
    unsigned linenumber = 0;
    /* get a line from assem */
    while(getline(&line, &len, assem) != -1)
    {
        ++linenumber;
        /* get mnemonic and operand - skip label if colon is present */
        char* colon = index(line, ':');
        char* mnemonic = strtok(colon ? colon + 1 : line, "\t ;\n");
        char* operand = strtok(NULL, ";\n");
        /* if mnemonic was found then assemble (see assemble.c) */
        if(mnemonic) error = assemble(mnemonic, operand, labels, hexid);
        /* otherwise continue onto the next line */
        else continue;
        /* if an error occurs during assemble (assemble() returns 1,2,3 and so forth) */
        if(error)
        {
            /* ...then fail */
            fail = 1;
            const char* message = "unknown error code";
            /* and report the nature of the error */
            switch(error)
            {
                case 1: message = "unrecognizable operands"; break;
                case 2: message = "label not found"; break;
                case 3: message = "unsupported chip8 mnemonic"; break;
                case 4: message = "missing operand"; break;
            }
            fprintf(stderr, "error: line %d: %s\n", linenumber, message);
        }
    }
    /* clean up */
    free(line);
    tree.print(labels);
    tree.delete(labels);
    fclose(assem);
    fclose(hexid);
    /* remove hexid if complilation failed */
    if(fail) remove(argv[2]);
    /* go home */
    return fail;
}
