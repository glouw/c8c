#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "assemble.h"

/* if a line does not have alpha numeric characters (but has spaces/tabs) then it is a comment line */
static int iscomment(char* line)
{
    for(; *line; ++line) if(isalnum(*line)) return 0;
    return 1;
}

/* let's rock */
int main(int argc, char* argv[])
{
    /* exit if assem and hexid files unspecified */
    if(argc != 3) return 1;
    /* assem for input */
    FILE* assem = fopen(argv[1], "r");
    /* if either assem failed to open then return 1 */
    if(assem == NULL) return 1;
    /* hexid for output */
    FILE* hexid = fopen(argv[2], "w");
    /* prepare for first scan; label&address pairs stored in tree "labels" */
    tnode* labels = NULL;
    /* 0x0200 is reserved for reset vector; label count stored in nlabels */
    unsigned address = 0x0202, nlabels = 0;
    /* FIRST PASS: gather labels */
    char* line = NULL;
    unsigned len = 0;
    while(getline(&line, &len, assem) != -1)
    {
        /* find colon, semicolon, and newline characters within line */
        char* colon = index(line, ':');
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        /* if the line is only a newline then continue onto the next line */
        if(line == newln) continue;
        /* if there is a semicolon then end the line there, else end it at the newline */
        if(semic) *semic = '\0'; else *newln = '\0';
        /* if the line does not contain any alpha numeric characters then continue onto the next line */
        if(iscomment(line)) continue;
        /* if a colon exists then a label exists */
        if(colon)
        {
            /* extract label */
            char* label = strtok(line, ":");
            /* add label&address pair to label tree */
            labels = tree.add(nlabels++ ? labels : NULL, tree.new(label, address));
        }
        address += 0x0002;
    }
    /* publish reset vector (JP, addr) */
    tnode* epoint = tree.get(labels, "MAIN");
    fprintf(hexid, "1%03X\n", epoint->address);
    /* rewind, prepare for second scan */
    rewind(assem);
    /* fail becomes 1 with assembly errors; .hex output is deleted at program exit if fail is 1 */
    int fail = 0;
    /* increment linenumber for every line read */
    int linenumber = 0;
    /* SECOND PASS: skip labels, tokenize mnemonics and operands, and assemble */
    while(getline(&line, &len, assem) != -1)
    {
        ++linenumber;
        /* label, mnemonic, operand */
        char* l, *m, *o;
        /* find colon, semicolon, and newline characters */
        char* colon = index(line, ':');
        char* semic = index(line, ';');
        char* newln = index(line, '\n');
        /* if line only contains newline, continue onto next line */
        if(line == newln) continue;
        /* if there is a semicolon then end line there, else end at newline */
        if(semic) *semic = '\0'; else *newln = '\0';
        /* if the line does not contain any alpha numeric characters then it is a comment, continue to next line */
        if(iscomment(line)) continue;
        /* if there is a colon then there is a label: point to label, mnenomic, and operand */
        if(colon) l = strtok(line, ":"), m = strtok(NULL, "\t "), o = strtok(NULL, "");
        /* else point to mnemonic and operand */
        else m = strtok(line, "\t "), o = strtok(NULL, "");
        /* the label, l, is of no use; void it to stop the compiler's whining */
        (void)l;
        /* assemble the mnemonic and operand; catch any errors */
        int error = assemble(m, o, labels, hexid);
        /* if an error occured during assembly (error codes 1,2,3, etc) */
        if(error)
        {
            /* then assembly failed; */
            fail = 1;
            /* and an error message indicating the specified error is thrown */
            char* message = "unknown error code";
            switch(error)
            {
                case 1: message = "unrecognizable operands"; break;
                case 2: message = "label not found"; break;
                case 3: message = "unsupported chip8 mnemonic"; break;
            }
            fprintf(stderr, "error: line %d: %s\n", linenumber, message);
        }
    }
    /* clean up */
    free(line);
    tree.delete(labels);
    fclose(assem);
    fclose(hexid);
    /* delete the output .hex file if complilation failed */
    if(fail) { remove(argv[2]); return 1; }
    /* go home */
    return 0;
}
