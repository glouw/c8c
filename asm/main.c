#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* MAXLINE denotes the most chars a line can hold */
#define MAXLINE 500
/* MAXPAGE denotes the number of label pushbacks our address book will take */
#define MAXPAGE 500
/* SZ denotes the size of your independent l, m, and o buffers */
#define SZ 100
/* MAXMS denotes the maxmimum number of opcodes availble */
#define MAXMS 19
/* a list of monics */
static char* ms[MAXMS] =
{
    "ADD","AND","CALL","CLS","DRW","JMP","LD","OR","RET","RND","SE","SHL","SHR","SKP","SKPN","SNE","SUB","SUBN","XOR"
};
/* book will keep track of label addresses; initializes to zero because global */
struct book
{
    int addr;   // addr denotes the line address
    char l[SZ]; // l denotes the name of the label
}
book[MAXPAGE];
/* this linear search will continue until addr is zero; if it finds a matching label it returns addr */
int search(char* l)
{
    for(int i=0; book[i].addr!=0; i++)
        if(strcmp(book[i].l, l) == 0)
            return book[i].addr;
    fprintf(stderr, "Label %s not found... ", l);
    abort(); // A label must alwasy be found; if not, abort
}
/* mneomnic list uses independent functions */
void add (char o[], FILE* fp) {
    if(o[0]=='V')
        if(o[4]=='V') fprintf(fp, "8%C%C4", o[1], o[5]); else fprintf(fp, "7%C%C%C", o[1], o[4], o[5]);
    else
        fprintf(fp, "F%C1E", o[4]);
}
void and (char o[], FILE* fp) { fprintf(fp, "8%C%C2", o[1], o[5]); }
void call(char o[], FILE* fp) { fprintf(fp, "2%X", search(o)); }
void cls (char o[], FILE* fp) { fprintf(fp, "00E0"); }
void drw (char o[], FILE* fp) { fprintf(fp, "D%C%C%C", o[1], o[5], o[8]); }
void jmp (char o[], FILE* fp) { if(o[0]=='V') fprintf(fp, "B%X", search(o+4)); else fprintf(fp, "1%X", search(o)); }
void ld  (char o[], FILE* fp)
{
    switch(o[0])
    {
        case 'V':
            switch(o[4])
            {
                case 'V': fprintf(fp, "8%C%C0",  o[1], o[5]);             break;
                case 'D': fprintf(fp, "F%C07",   o[1]);                   break;
                case 'K': fprintf(fp, "F%C0A",   o[1]);                   break;
                case 'I': fprintf(fp, "F%C65",   o[1]);                   break;
                default : fprintf(fp, "6%C%C%C", o[1], o[4], o[5], o[6]); break;
            }
            break;
        case 'I': if(o[3]=='V') fprintf(fp, "F%C55", o[4]); else fprintf(fp, "A%X", search(o+3)); break;
        case 'D': fprintf(fp, "F%C15", o[5]); break;
        case 'S': fprintf(fp, "F%C18", o[5]); break;
        case 'F': fprintf(fp, "F%C29", o[4]); break;
        case 'B': fprintf(fp, "F%C33", o[4]); break;
    }
}
void or  (char o[], FILE* fp) { fprintf(fp, "8%C%C1", o[1], o[5]); }
void ret (char o[], FILE* fp) { fprintf(fp, "00EE"); }
void rnd (char o[], FILE* fp) { fprintf(fp, "C%C%S", o[1], o[4]); }
void se  (char o[], FILE* fp) { if(o[4]=='V') fprintf(fp, "5%C%C0", o[1], o[5]); else fprintf(fp, "3%C%C%C", o[1], o[4], o[5]); }
void shl (char o[], FILE* fp) { fprintf(fp, "8%C%CE", o[1], o[5]); }
void shr (char o[], FILE* fp) { fprintf(fp, "8%C%C6", o[1], o[5]); }
void skp (char o[], FILE* fp) { fprintf(fp, "E%C9E",  o[1]); }
void skpn(char o[], FILE* fp) { fprintf(fp, "E%CA1",  o[1]); }
void sne (char o[], FILE* fp) { if(o[4]=='V') fprintf(fp, "9%C%C0", o[1], o[5]); else fprintf(fp, "4%C%C%C",o[1], o[4], o[5]); }
void sub (char o[], FILE* fp) { fprintf(fp, "8%C%C5", o[1], o[5]); }
void subn(char o[], FILE* fp) { fprintf(fp, "8%C%C7", o[1], o[5]); }
void xor (char o[], FILE* fp) { fprintf(fp, "8%C%C3", o[1], o[5]); }
/* the opcode list map is an array of function pointers that point to the opcode list above */
void (*list[MAXMS])(char o[], FILE* fp) =
{
    add, and, call, cls, drw, jmp, ld, or, ret, rnd, se, shl, shr, skp, skpn, sne, sub, subn, xor
};
/* we can call a function from the array above by speciyfing a momnic number */
void assemble(char o[], int mnum, FILE* fp)
{
    void (*_assemble)(char o[], FILE* fp) = list[mnum];
    _assemble(o, fp);
    fputc('\n', fp);
}
/* go! */
int main(int argc, char* argv[])
{
    /* for debugging purposes, stdin and stdout will remain as default IO if there are no command line args */
    FILE* in  = stdin;
    FILE* out = stdout;
    /* if one command line argument is specified, then write from stdin to that file */
    if(argc == 2)
        out = fopen(argv[1], "w");
    /* otherwise, if two command line arguments are specified, read from the first and write to the second */
    if(argc == 3)
        in = fopen(argv[1], "r"), out = fopen(argv[2], "w");
    /* the line buffer will hold one line */
    char line[MAXLINE];
    /* the line buffer will be split into tokens; each token will have its own buffer */
    char l[SZ]; // label
    char m[SZ]; // mneomic
    char o[SZ]; // operand
    /* addr and mark will define the current line address and the current page of book, respectively */
    int addr = 0x0200; // starts at 0x0200 but increment first to make room for reset vector
    int mark = 0;
    /* get lines until EOF (ctrl+d or end of file) is found */
    while(fgets(line, SZ, in))
    {
        /* each instruction is two bytes long; increment addr by 2 */
        addr += 0x0002;
        /* if only a new line is read then the line is empty; restart the loop */
        if(line[0] == '\n') continue;
        /* if the first read char is a space then extract the mneomic and operand tokens */
        if(isspace(line[0]))
        {
            char* a = strtok(line, "\t ;");
            char* b = strtok(NULL, ";");
            if(*a == '$')
            {
                fprintf(out, "%s\n", a+1); // skip the dollah sign ($)
                continue;
            }
            if(a) strcpy(m, a);
            if(b) strcpy(o, b);
        }
        /* otherwise if the first read char is an ambersan (&) then this is a variable label:
         * push back the label and address to the address book */
        else
        if(line[0] == '&')
        {
            char* a = strtok(line, ":");
            char* b = strtok(NULL, "\t;");
            if(a) strcpy(book[mark].l, a);
            book[mark++].addr = addr;
            fprintf(out, "%s\n", b+1); // skip the dot (.)
            continue; // no need to assemble data addresses, restart the loop
        }
        /* otherwise there has to be a label: push back the label and address to the address book */
        /* and read the monic and operand tokens */
        else
        {
            char* a = strtok(line, ":");
            char* b = strtok(NULL, "\t ;");
            char* c = strtok(NULL, ";");
            if(a) strcpy(book[mark].l, a);
            if(b) strcpy(m, b);
            if(c) strcpy(o, c);
            book[mark++].addr = addr;
        }
        /* search ms list */
        int cmp(const void*, const void*);
        char** found = bsearch(m, ms, MAXMS, sizeof(char*), cmp);
        /* monic number is difference between found address and base ms address */
        int mnum = found - ms;
        /* assemble */
        assemble(o, mnum, out);
    }
    fclose(in);
    fclose(out);
    /* a reset vector is requried for the final published hex file */
    if(argc == 2 || argc == 3)
    {
        /* open a temp output file and write the reset vector to it */
        char* tmpfname = "tmp";
        FILE* tmpout = fopen(tmpfname, "w");
        fprintf(tmpout, "1%X\n", search("MAIN"));
        /* copy over the remaining lines */
        char* fname = argv[argc-1];
        FILE* tmpin  = fopen(fname, "r");
        char tmpline[MAXLINE];
        while(fgets(tmpline, SZ, tmpin)) fputs(tmpline, tmpout);
        fclose(tmpin);
        fclose(tmpout);
        /* move temp to the user specified output file name */
        char* cmd = "mv";
        int bytes = strlen(cmd) + strlen(fname) + strlen(tmpfname) + 2 + 1; // 2 spaces and a null byte
        char* call = malloc(bytes);
        sprintf(call, "%s %s %s", cmd, tmpfname, fname);
        system(call);
        free(call);
    }
    return 0;
}
/* besearch compare function */
int cmp(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}
