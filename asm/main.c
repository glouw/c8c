#include "public.h"

/* let's rock ! */
int main(int argc, char* argv[])
{
    /* error goes true if a single error is found during assembly */
    int error = 0;
    /* chip8 wakes up at this address */
    int address = 0x0200;
    /* linked list keeps tracks of declared labels and their addresses */
    struct node* list = push(NULL, 0x0000, "N/A");
    /* for getline() */
    char* line = NULL; unsigned len = 0;
    /* label, mnem, operand pointers for line */
    char *l, *m, *o;
    /* label, mnem, operand delimeters for token extraction */
    char* deliml= ":";
    char* delimm = " \t;";
    char* delimo = ";";
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
        /* the address is first and foremost incremented to make room for the zero vector */
        address += 0x0002;
        char* semicolon = index(line, ';');
        char* colon     = index(line, ':');
        char* tabbed    = index(line, '\t');
        char* spaced    = index(line, ' ');  // when reading from a file a tab is seen as several spaces
        char* newline   = index(line, '\n'); // when readng from standard input a tab is a tab character
        /* if the line read is empty (just a newline) then ignore it */
        if(line == newline)
        {
            address -= 0x0002;
            continue;
        }
        /* a line must always finish with a semicolon - Everything after a semicolon is a comment and ignored */
        if(!semicolon)
        {
            error = errorc(oname, address);
            continue;
        }
        /* if the line is commented out, ignore it and continue onto the next line */
        if(line == semicolon)
        {
            continue;
        }
        /* a line must either start with a tab character or a label */
        /* a label is defined by the colon (':') character if placed before the semicolon */
        /* Thus, if a line has tab and a label then it is invalid */
        if((line==tabbed && line!= spaced) && colon && colon<semicolon)
        {
            error = errord(oname, address);
            continue;
        }
        /* likewise, if a line does not have a tab nor a label then it is invalid */
        else if((line!=tabbed && line!=spaced) && !colon)
        {
            error = errore(oname, address);
            continue;
        }
        /* begin token collection: If the line is tabbed, collect tokens mnem and oper */
        if(line == tabbed || line == spaced)
        {
            m = strtok(line, delimm); /* mnem */
            o = strtok(NULL, delimo);    /* oper */
        }
        /* else, collect label, mnem, and oper tokens */
        else
        {
            l = strtok(line, deliml);    /* labl */
            m = strtok(NULL, delimm); /* mnem */
            o = strtok(NULL, delimo);    /* oper */
            /* If the user attempts to declare a label that already exists throw an error */
            int paddress = find(list, l); // past address
            if(paddress != -1)
            {
                error = errorf(oname, address, paddress, l);
                continue;
            }
            /* else, push back the address to list */
            else
            {
                list = push(list, address, l);
            }
        }
        /* if the first two chars of the mnem are "0x" then regard the following as data */
        if(strncmp(m, "0x", 2) == 0)
        {
            int nibs = 4;
            char* nib = m + strlen(m) - nibs; // skip 0x
            if(index(nib, 'x'))
            {
                error = errori(oname, address, nibs);
            }
            else
            {
                fprintf(out, "%s\n", nib);
            }
            continue;
        }
        /* bsearch mnem list for mnemonic m */
        int cmp(const void* a, const void* b);
        char** found = bsearch(m, mnem, sizeof(mnem)/sizeof(char*), sizeof(char*), cmp);
        /* if not found then that mnem is not supported by chip8; throw error */
        if(!found)
        {
            error = errorg(oname, address, m);
            continue;
        }
        /* otherwise, the mnem number, mnum (possible 0-18), is simply the array index
         * of mnem which can be found by subtracting mnem from found */
        int mnum = found - mnem;
        /* assemble */
        switch(mnum)
        {
            case  0: error |=  add(iname, out, o, address      ); break;
            case  1: error |=  and(iname, out, o, address      ); break;
            case  2: error |= call(iname, out, o, address, list); break;
            case  3:           cls(       out                  ); break;
            case  4: error |=  drw(iname, out, o, address      ); break;
            case  5: error |= jump(iname, out, o, address, list); break;
            case  6: error |=   ld(iname, out, o, address, list); break;
            case  7:            or(       out, o               ); break;
            case  8:           ret(       out                  ); break;
            case  9: error |=  rnd(iname, out, o, address      ); break;
            case 10: error |=   se(iname, out, o, address      ); break;
            case 11:           shl(       out, o               ); break;
            case 12:           shr(       out, o               ); break;
            case 13:           skp(       out, o               ); break;
            case 14:          skpn(       out, o               ); break;
            case 15: error |=  sne(iname, out, o, address      ); break;
            case 16:           sub(       out, o               ); break;
            case 17:          subn(       out, o               ); break;
            case 18:           xor(       out, o               ); break;
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
            error = errorh(oname, ename);
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
        /* if there were any errors during assembly then incinerate the assembled file */
        if(error)
        {
            remove(oname);
            fprintf(stderr, RED"FAILURE: "NRM"%s\n", oname);
        }
        /* otherwise, we're good to go! */
        else
        {
            fprintf(stderr, GRN"SUCCESS: "NRM"%s\n", oname);
        }
    }
    /* free heaped memory, go home */
    fclose(in);
    fclose(out);
    free(line);
    delt(list);
    return 0;
}
/* compare function for bsearch within main */
int cmp(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}
