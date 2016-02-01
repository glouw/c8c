#include "public.h"

/* compare function for bsearch within main */
int cmp(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

int main(int argc, char* argv[])
{
    /* compiler error: cerror goes 1 when there is a problem anywhere */
    int cerror = 0;
    /* chip8 wakes up at this address */
    int address = 0x0200, linenumber = 1;
    /* linked list keeps tracks of declared labels and their addresses */
    struct node* list = push(NULL, 0x0000, "N/A");
    /* for getline() */
    char* line = NULL;
    unsigned len = 0;
    /* label, mnem, operand pointers for line */
    char *l, *m, *o;
    /* default to standard in, out */
    FILE* in  = stdin;
    FILE* out = stdout;
    /* oname is output filepoint, iname is input file name */
    char* oname = NULL;
    char* iname = NULL;
    /* supported chip8 mnems */
    char* mnem[] =
    {
        "ADD","AND","CALL","CLS","DRW","JUMP","LD","OR","RET","RND","SE","SHL",
        "SHR","SKP","SKPN","SNE","SUB","SUBN","XOR"
    };
    /* if one file is specified on commmand line then it is to be written to (oname) */
    if(argc == 2)
    {
        oname = argv[argc-1];
        out = fopen(oname, "w");
    }
    /* if two, read from the first (iname), write to the last (oname) */
    if(argc == 3)
    {
        iname = argv[argc-2];
        oname = argv[argc-1];
        if((in = fopen(iname, "r")) == NULL)
            fprintf(stderr, "file %s does not exist\n", iname);
        out = fopen(oname, "w");
    }
    /* let's get to work! read a line from in */
    while(getline(&line, &len, in) != -1)
    {
        /* the address is first and foremost incremented to make room for the zero vector */
        address += 0x0002;
        char* semicolon =  index(line, ';' );
        char* colon     =  index(line, ':' );
        char* tabbed    =  index(line, '\t');
        char* spaced    =  index(line, ' ' );
        char* newline   = rindex(line, '\n');
        /* if empty line, ignore and continue */
        if(line == newline)
        {
            address -= 0x0002;
            continue;
        }
        /* if there is a semicolon then end the line at the semicolon */
        if(semicolon)
            *semicolon = '\0';
        /* otherwise the line ends at the newline */
        else
            *newline = '\0';
        /* if there is a colon then we have a label*/
        if(colon && (line!=tabbed || line!=spaced))
        {
            l = strtok(line, ":");   /* labl */
            m = strtok(NULL, " \t"); /* mnem */
            o = strtok(NULL, "");    /* oper */
            if(find(list, l) != -1)
            {
                cerror = 1;
                linenumber = (address-0x0200)/2;
                fprintf(stderr, "%s: error: line %d: label %s already exists\n", iname, linenumber, l);
                continue;
            }
            list = push(list, address, l);
        }
        /* else if the line is tabbed (stdin) or spaced (file) then we do not have a label */
        else if(line==tabbed || line==spaced)
        {
            m = strtok(line, " \t"); /* mnem */
            o = strtok(NULL, "");    /* oper */
        }
        /* otherwise report that the line needs to be tabbed */
        else
        {
            cerror = 1;
            linenumber = (address-0x0200)/2;
            fprintf(stderr, "%s: error: line %d: missing tab\n", iname, linenumber);
            continue;
        }
        /* if the first two chars of _m_ are "0x" then this is not a mnemomnic, it is data */
        if(strncmp(m, "0x", 2) == 0)
        {
            fprintf(out, "%s\n", m+2);
            continue;
        }
        /* bsearch mnem list for mnemonic m */
        char** found = bsearch(m, mnem, sizeof(mnem)/sizeof(char*), sizeof(char*), cmp);
        /* if the mnem is not found in list throw an error */
        if(!found)
        {
            cerror = 1;
            linenumber = (address-0x0200)/2;
            fprintf(stderr, "%s: error: line %d: %s not a valid CHIP8 mnem\n", iname, linenumber, m);
            continue;
        }
        /* mnum can be found by subtracting 'mnem' from 'found' */
        int mnum = found - mnem;
        /* pack and go */
        struct pack p = { out, o, list };
        /* assemble, return assemble error */
        int aerror = assemble(mnum, p);
        /* handle assemble error if not zero */
        if(aerror)
        {
            cerror = 1;
            linenumber = (address-0x0200)/2;
            char* message = "unhandled case in aerror switch statement";
            switch(aerror)
            {
                case 1: message = "assembly failure"; break;
                case 2: message = "unknown label";    break;
            }
            fprintf(stderr, "%s: error: line %d: %s %s\n", iname, linenumber, message, o);
        }
    }
    /* the final assembly requires a zero address but only if oname was specified on the command line */
    if(oname)
    {
        /* entry point */
        char* epoint = "MAIN";
        /* the entry address MAIN must be searched for and found first */
        int eaddress = find(list, epoint);
        /* if the eaddress was found then insert a zero vector */
        if(eaddress != -1)
        {
            /* close buffer and reopen to save */
            out = freopen(oname, "r", out);
            char* tname = "tmp";
            FILE* tout = fopen(tname, "w");
            fprintf(tout, "1%X\n", eaddress);
            /* transfer every line from the original file to tmp */
            while(getline(&line, &len, out) != -1)
                fputs(line, tout);
            fclose(tout);
            /* rename temp file to real */
            rename(tname, oname);
        }
        /* otherwise complain that the entry point is missing */
        else
            fprintf(stderr, "%s: error: missing %s entry point\n", iname, epoint);
        /* if the compilation error flag is high then remove the assembled file - it's of no use */
        if(cerror)
            remove(oname);
    }
    /* free heaped memory, go home */
    fclose(in);
    fclose(out);
    free(line);
    delt(list);
    return 0;
}
