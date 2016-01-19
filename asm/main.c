#include "public.h"

/* compare function for bsearch within main */
static int cmp(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

/* get psyched! */
int main(int argc, char* argv[])
{
    /* error goes true if a single error is found during assembly */
    bool error = false;
    /* chip8 wakes up at this address */
    int address = 0x0200;
    /* linked list keeps tracks of declared labels and their addresses */
    struct node* list = push(NULL, 0x0000, "N/A");
    /* for getline() */
    char* line = NULL; unsigned len = 0;
    /* label, mnem, operand pointers for line */
    char *l, *m, *o;
    /* error lines: linenum is for current, plinenum for past */
    int linenum, plinenum;
    /* default to standard in, out */
    FILE* in  = stdin;
    FILE* out = stdout;
    /* oname is output filename, iname is input file name */
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
        in  = fopen(iname, "r");
        out = fopen(oname, "w");
    }
    /* let's get to work! read a line from in */
    while(getline(&line, &len, in) != -1)
    {
        /* The address is first and foremost incremented to make room for the zero vector */
        address += 0x0002;
        char* semicolon = index(line, ';');
        char* colon     = index(line, ':');
        char* tabbed    = index(line, '\t');
        char* newline   = index(line, '\n');
        /* If the line read is empty (just a newline) then ignore it */
        if(line == newline)
        {
            address -= 0x0002;
            continue;
        }
        /* A line must always finish with a semicolon - Everything after a semicolon is a comment and ignored */
        if(!semicolon)
        {
            linenum = (address-0x0200)/2;
            fprintf(stderr, "line %d: error: missing semicolon\n", linenum);
            error = true;
            continue;
        }
        /* A line must either start with a tab character or a label */
        /* A label is defined by the colon (':') character if placed before the semicolon */
        /* Thus, if a line has tab and a label then it is invalid */
        if(line==tabbed && colon && colon<semicolon)
        {
            linenum = (address-0x0200)/2;
            fprintf(stderr, "line %d: error: label with tab\n", linenum);
            error = true;
            continue;
        }
        /* Likewise, if a line does not have a tab nor a label then it is invalid */
        else if(line!=tabbed && !colon)
        {
            linenum = (address-0x0200)/2;
            fprintf(stderr, "line %d: error: missing tab\n", linenum);
            error = true;
            continue;
        }
        /* Begin token collection: If the line is tabbed, collect tokens mnem and oper */
        if(line == tabbed)
        {
            m = strtok(line, " \t;"); /* mnem */
            o = strtok(NULL, ";");    /* oper */
        }
        /* Else, collect label, mnem, and oper tokens */
        else
        {
            l = strtok(line, ":");    /* labl */
            m = strtok(NULL, " \t;"); /* mnem */
            o = strtok(NULL, ";");    /* oper */
            /* If the user attempts to declare a label that already exists throw an error */
            int paddress = find(list, l); // past address
            if(paddress != -1)
            {
                plinenum = (paddress-0x0200)/2; // past line number
                linenum = (address-0x0200)/2;   // line number
                fprintf(stderr, "line %d: error: label %s already exists at line %d\n", linenum, l, plinenum);
                error = true;
                continue;
            }
            /* else, push back the address to list */
            else list = push(list, address, l);
        }
        /* bsearch mnem list for mnemonic m */
        char** found = bsearch(m, mnem, sizeof(mnem)/sizeof(char*), sizeof(char*), cmp);
        /* if not found then that mnem is not supported by chip8; throw error */
        if(!found)
        {
            linenum = (address-0x0200)/2;
            fprintf(stderr, "line %d: error: %s is not valid mnem\n", linenum, m);
            error = true;
            continue;
        }
        /* otherwise, the mnem number, mnum (possible 0-18), is simply the array index
         * of mnem which can be found by subtracting mnem from found */
        int mnum = found - mnem;
        /* assemble */
        switch(mnum)
        {
            case  0:           add(out, o               ); break;
            case  1:           and(out, o               ); break;
            case  2: error |= call(out, o, list, address); break;
            case  3:           cls(out                  ); break;
            case  4:           drw(out, o               ); break;
            case  5: error |= jump(out, o, list, address); break;
            case  6: error |=   ld(out, o, list, address); break;
            case  7:            or(out, o               ); break;
            case  8:           ret(out                  ); break;
            case  9:           rnd(out, o               ); break;
            case 10:            se(out, o               ); break;
            case 11:           shl(out, o               ); break;
            case 12:           shr(out, o               ); break;
            case 13:           skp(out, o               ); break;
            case 14:          skpn(out, o               ); break;
            case 15:           sne(out, o               ); break;
            case 16:           sub(out, o               ); break;
            case 17:          subn(out, o               ); break;
            case 18:           xor(out, o               ); break;
        }
        fputc('\n', out);
    }
    /* the final assembly requires a zero address but only if oname was specified */
    if(oname)
    {
        /* if user attempts to assemble a program without a MAIN entry label then throw error */
        char* ename = "MAIN";
        int eaddress = find(list, ename);
        if(eaddress == -1)
        {
            fprintf(stderr, "fatal error: cannot find entry point label %s\n", ename);
            error = true;
            remove(oname);
        }
        /* else create tmp file and make the first line a zero vector for MAIN */
        else
        {
            out = freopen(oname, "r", out); // freopen saves buffer and reopens
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
        /* if there were any errors during assembly then remove the assembled file */
        if(error)
        {
            remove(oname);
            fprintf(stderr, "%s failed to assemble\n", oname);
        }
    }
    /* free heaped memory, go home */
    fclose(in);
    fclose(out);
    free(line);
    delt(list);
    return 0;
}
