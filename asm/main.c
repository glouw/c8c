#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "opcode.h"

/* Error messages */
#define RED "\x1b[31m"
#define NRM "\x1b[0m"

int main(int argc, char* argv[])
{
    if(argc != 3)
        return 1;
    char* fr = argv[1];
    char* fw = argv[2];
    FILE* assem = fopen(fr, "r");
    if(assem == NULL)
    {
        fprintf(stderr, RED"error: "NRM"file %s not found\n", fr);
        return 1;
    }
    FILE* hexid = fopen(fw, "w");
    /* First scan: Gather labels */
    node* labels = NULL;
    char* line = NULL;
    unsigned address = 0x0202, len = 0, linenumber = 0, fail = 0;
    while(getline(&line, &len, assem) != -1)
    {
        linenumber += 1;
        /* Remove comment by terminating line at semicolon */
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        if(semic) *semic = '\0'; else *newln = '\0';
        /* Labels end with a colon - if a colon is found, put address & label pair in tree */
        char* colon = index(line, ':');
        if(colon)
        {
            char* token = strtok(line, ":");
            node* exists = tree.get(labels, token);
            if(exists)
            {
                fail = 1;
                fprintf(stderr, RED"error:"NRM" file %s: line %d: label %s already defined\n",
                    fr, linenumber, token);
            }
            labels = tree.add(labels, tree.new(token, address));
        }
        /* Only incrememnt address if there is a mnemonic; mnemonics end with a space or a tab */
        char* mnemonic = strtok(colon ? NULL : line, "\t ");
        if(mnemonic) address += 0x0002;
    }
    unsigned error = 0;
    /* Write reset vector to hexid if entry name is found */
    char* token = "MAIN";
    node* label = tree.get(labels, token);
    if(label == NULL)
    {
        fail = 1;
        fprintf(stderr, RED"error:"NRM" file %s: line %d: %s undefined reference\n",
            fr, linenumber, token);
    }
    else fprintf(hexid, "1%03X\n", label->address);
    /* Second scan: Generate hex code */
    rewind(assem);
    linenumber = 0;
    while(getline(&line, &len, assem) != -1)
    {
        linenumber += 1;
        /* Remove comment by terminating line at semicolon */
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        if(semic) *semic = '\0'; else *newln = '\0';
        /* Get mnemonic and operand - skip label if colon is present */
        char* colon = index(line, ':');
        char* mnemonic = strtok(colon ? colon+1 : line, "\t ");
        char* operand = strtok(NULL, "");
        /* If mnemonic was found then assemble */
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
            fprintf(stderr, RED"error:"NRM" file %s: line %d: %s\n",
                fr, linenumber, message);
        }
    }
    /* Clean up */
    free(line);
    tree.burn(labels);
    fclose(assem);
    fclose(hexid);
    if(fail)
        remove(fw);
    return fail;
}
