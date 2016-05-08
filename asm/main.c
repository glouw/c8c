#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "opcode.h"

int main(int argc, char* argv[])
{
    if(argc != 3) return 1;
    char* fr = argv[1];
    char* fw = argv[2];
    FILE* assem = fopen(fr, "r");
    if(assem == NULL) return 1;
    FILE* hexid = fopen(fw, "w");
    /* First scan: Gather labels */
    node* labels = NULL;
    unsigned address = 0x0202;
    char* line = NULL; unsigned len = 0;
    while(getline(&line, &len, assem) != -1)
    {
        /* remove comment by terminating line at semicolon */
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        if(semic) *semic = '\0'; else *newln = '\0';
        /* labels end with a colon - if a colon is found, put address & label pair in tree */
        char* colon = index(line, ':');
        if(colon)
        {
            char* token = strtok(line, ":");
            labels = tree.add(labels, tree.new(token, address));
        }
        /* only incrememnt address if there is a mnemonic; mnemonics end with a space or a tab */
        char* mnemonic = strtok(colon ? NULL : line, "\t ");
        if(mnemonic) address += 0x0002;
    }
    unsigned error = 0, fail = 0;
    /* write reset vector to hexid if entry name is found */
    char* token = "MAIN";
    node* label = tree.get(labels, token);
    if(label == NULL)
    {
        fail = 1;
        fprintf(stderr, "error: %s undefined reference\n", token);
    }
    else fprintf(hexid, "1%03X\n", label->address);
    /* Second scan: generate hex code */
    rewind(assem);
    unsigned linenumber = 0;
    while(getline(&line, &len, assem) != -1)
    {
        ++linenumber;
        /* remove comment by terminating line at semicolon */
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        if(semic) *semic = '\0'; else *newln = '\0';
        /* get mnemonic and operand - skip label if colon is present */
        char* colon = index(line, ':');
        char* mnemonic = strtok(colon ? colon+1 : line, "\t ");
        char* operand = strtok(NULL, "");
        /* if mnemonic was found then assemble */
        if(mnemonic) error = opcode.assemble(mnemonic, operand, labels, hexid);
        else continue;
        if(error)
        {
            fail = 1;
            char* message = "unknown error code";
            switch(error)
            {
                case 1: message = "unrecognizable operands"; break;
                case 2: message = "label not found"; break;
                case 3: message = "unsupported chip8 mnemonic"; break;
                case 4: message = "missing operand"; break;
            }
            char* red = "\x1b[31m";
            char* nrm = "\x1b[0m";
            fprintf(stderr, "%serror: %sfile %s: line %d: %s\n", red, nrm, fr, linenumber, message);
        }
    }
    /* clean up */
    free(line);
    tree.delete(labels);
    fclose(assem);
    fclose(hexid);
    if(fail) remove(fw);
    return fail;
}
