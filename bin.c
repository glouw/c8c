#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static int hex(const int c)
{
    switch(c)
    {
    case '0': return 0x0;
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0x4;
    case '5': return 0x5;
    case '6': return 0x6;
    case '7': return 0x7;
    case '8': return 0x8;
    case '9': return 0x9;
    case 'A': return 0xA;
    case 'B': return 0xB;
    case 'C': return 0xC;
    case 'D': return 0xD;
    case 'E': return 0xE;
    case 'F': return 0xF;
    default:
        fprintf(stderr, "unknown char '%c' during conversion\n", c);
        exit(1);
    }
}

static int bin(const int lo, const int hi)
{
    return (hex(lo) << 4) | hex(hi);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "expected input and output arguments");
        exit(1);
    }
    FILE* const fi = fopen(argv[1], "r");
    FILE* const fo = fopen(argv[2], "w");
    for(int c = fgetc(fi); c != EOF; c = fgetc(fi))
    {
        if(isspace(c))
            continue;
        fprintf(fo, "%c", bin(c, fgetc(fi)));
    }
    fclose(fi);
    fclose(fo);
}
