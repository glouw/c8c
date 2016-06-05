#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

static bool fail = false;
static char* input;
static char* output;
static FILE* assembly;
static FILE* hexidecimal;
static unsigned linenumber;

struct node
{
    char* name;
    unsigned address;
    struct node* left;
    struct node* rite;
};

static struct node* new(char* name, unsigned address)
{
    struct node* node = malloc(sizeof(struct node));
    node->name = strdup(name);
    node->address = address;
    node->left = NULL;
    node->rite = NULL;
    return node;
}

static struct node* add(struct node* tree, struct node* node)
{
    if(tree == NULL)
        return node;
    int difference = strcmp(node->name, tree->name);
    if(difference == 0)
    {
        fprintf(stderr, "error: file %s: line %d: label \"%s\" already defined\n",
            input, linenumber, node->name);
        fail = true;
        free(node->name);
        free(node);
    }
    else if(difference < 0)
        tree->left = add(tree->left, node);
    else
        tree->rite = add(tree->rite, node);
    return tree;
}

static struct node* get(struct node* tree, char* name)
{
    if(tree == NULL)
        return NULL;
    int difference = strcmp(name, tree->name);
    if(!difference)
        return tree;
    else if(difference < 0)
        return get(tree->left, name);
    else
        return get(tree->rite, name);
}

static void burn(struct node* tree)
{
    if(tree == NULL)
        return;
    burn(tree->left);
    burn(tree->rite);
    free(tree->name);
    free(tree);
}

static void print(struct node* tree)
{
    if(tree == NULL)
        return;
    print(tree->left);
    printf("%04X: %s\n", tree->address, tree->name);
    print(tree->rite);
}

static struct node* grow(void)
{
    struct node* tree = NULL;
    unsigned size = 0;
    unsigned address = 0x0202;
    linenumber = 1;
    char* line = NULL;
    while(getline(&line, &size, assembly) != -1)
    {
        char* label;
        char* operation;
        char* operand;
        char* newline;
        char* semicolon;
        char* colon;
        newline = index(line, '\n');
        if(newline)
            *newline = '\0';
        semicolon = index(line, ';');
        if(semicolon)
            *semicolon = '\0';
        colon = index(line, ':');
        if(colon)
        {
            label = strtok(line, ":");
            tree = add(tree, new(label, address));
        }
        operation = strtok(colon ? NULL : line, "\t ");
        operand = strtok(NULL, "\n");
        if(operation && operand && strcmp(operation, "DB") == 0)
            address += 0x0001;
        else if(operation)
            address += 0x0002;
        linenumber += 1;
    }
    free(line);
    return tree;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "error: %s: missing arguments\n", input);
        return 1;
    }
    input = argv[1];
    assembly = fopen(input, "r");
    if(!assembly)
    {
        fprintf(stderr, "error: %s not found\n", input);
        return 1;
    }
    output = argv[2];
    hexidecimal = fopen(output, "w");
    /* First pass */
    struct node* tree = grow();
    print(tree);
    /* Second pass */
    /* Cleanup */
    burn(tree);
    fclose(assembly);
    fclose(hexidecimal);
    if(fail)
    {
        fprintf(stderr, "Assembly failed...\n");
        remove(output);
        return 1;
    }
    fprintf(stderr, "Assembly success!\n");
    return 0;
}

///* Byte (data type) */
//int byte(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    /* BYTE */
//    if(strlen(a)==4 && strncmp(a,"0x",2)==0 &&
//       isxdigit(a[2]) &&
//       isxdigit(a[3]))
//           fprintf(hexidecimal, "%s", a+2);
//    else return 1;
//    return 0;
//}
//
///* Addition (+) */
//int add(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* ADD Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c4", a[1], b[1]);
//    /* ADD I, Vx */
//    else
//    if(strlen(a)==1 && a[0]=='I' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c1E", b[1]);
//    /* ADD Vx, byte */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==4 && strncmp(b,"0x",2)==0 &&
//       isxdigit(b[2]) &&
//       isxdigit(b[3]))
//           fprintf(hexidecimal, "7%c%s", a[1], b+2);
//    else return 1;
//    return 0;
//}
//
///* Bitwise and (&) */
//int and(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* AND Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c2", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Call subroutine */
//int call(char* operand, node* tree, FILE* hexidecimal)
//{
//    char* a = strtok(operand, "\t ,");
//    node* found = get(tree, a);
//    /* CALL ad */
//    if(found)
//        fprintf(hexidecimal, "2%03X", found->address);
//    else return 2;
//    return 0;
//}
//
///* Clear display */
//int cls(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)o, (void)hexidecimal, (void)tree;
//    /* CLS */
//    fprintf(hexidecimal, "00E0");
//    return 0;
//}
//
///* Draw sprite */
//int drw(char* operand, node* tree, FILE* hexidecimal)
//{
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    char* c = strtok(NULL, "\t ,");
//    /* DRW Vx, Vy, nib */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]) &&
//       strlen(c)==3 && strncmp(c, "0x", 2)==0 && isxdigit(c[2]))
//           fprintf(hexidecimal, "D%c%c%c", a[1], b[1], c[2]);
//    else return 1;
//    return 0;
//}
//
///* Jump */
//int jp(char* operand, node* tree, FILE* hexidecimal)
//{
//    node* found;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* JP V0, ad */
//    if(strlen(a)==2 && a[0]=='V' && a[1]=='0' &&
//       (found = get(tree, b)))
//           fprintf(hexidecimal, "B%03X", found->address);
//    /* JP ad */
//    else
//    if((found = get(tree, a)))
//        fprintf(hexidecimal, "1%03X", found->address);
//    else return 2;
//    return 0;
//}
//
///* Load */
//int ld(char* operand, node* tree, FILE* hexidecimal)
//{
//    node* found;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* LD DT, Vx */
//    if(strlen(a)==2 && a[0]=='D' && a[1]=='T' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c15", b[1]);
//    /* LD ST, Vx */
//    else
//    if(strlen(a)==2 && a[0]=='S' && a[1]=='T' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c18", b[1]);
//    /* LD F, Vx */
//    else
//    if(strlen(a)==1 && a[0]=='F' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c29", b[1]);
//    /* LD B, Vx */
//    else
//    if(strlen(a)==1 && a[0]=='B' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c33", b[1]);
//    /* LD Vx, DT */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='D' && b[1]=='T')
//           fprintf(hexidecimal, "F%c07", a[1]);
//    /* LD Vx, [I] */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==3 &&
//       b[0]=='[' &&
//       b[1]=='I' &&
//       b[2]==']')
//           fprintf(hexidecimal, "F%c65", a[1]);
//    /* LD Vx, K */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==1 && b[0]=='K')
//           fprintf(hexidecimal, "F%c0A", a[1]);
//    /* LD Vx, Vy */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c0", a[1], b[1]);
//    /* LD Vx, byte */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//      (strlen(b)==4 && strncmp(b, "0x", 2))==0 &&
//      isxdigit(b[2]) &&
//      isxdigit(b[3]))
//           fprintf(hexidecimal, "6%c%s", a[1], b+2);
//    /* LD [I], Vx */
//    else
//    if(strlen(a)==3 &&
//       a[0]=='[' &&
//       a[1]=='I' &&
//       a[2]==']' &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "F%c55", b[1]);
//    /* LD I, ad */
//    else
//    if(strlen(a)==1 && a[0]=='I')
//    {
//       if((found = get(tree, b)))
//           fprintf(hexidecimal, "A%03X", found->address);
//       else return 2;
//    }
//    else return 1;
//    return 0;
//}
//
///* Bitwise or (|) */
//int or(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* OR Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c1", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Return from subroutine */
//int ret(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)o, (void)hexidecimal, (void)tree;
//    /* RET */
//    fprintf(hexidecimal, "00EE");
//    return 0;
//}
//
///* Random number with mask */
//int rnd(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* RND Vx, byte */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
//       isxdigit(b[2]) &&
//       isxdigit(b[3]))
//           fprintf(hexidecimal, "C%c%s", a[1], b+2);
//    else return 1;
//    return 0;
//}
//
///* Skip instruction if equal */
//int se(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SE Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "5%c%c0", a[1], b[1]);
//    /* SE Vx, byte */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
//       isxdigit(b[2]) &&
//       isxdigit(b[3]))
//           fprintf(hexidecimal, "3%c%s", a[1], b+2);
//    else return 1;
//    return 0;
//}
//
///* Shift left */
//int shl(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SHL Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%cE", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Shift right */
//int shr(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SHR Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c6", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Skip instruction if keypress equal */
//int skp(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    /* SKP Vx */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
//        fprintf(hexidecimal, "E%c9E", a[1]);
//    else return 1;
//    return 0;
//}
//
///* Skip instruction if keypress not equal */
//int sknp(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    /* SKNP Vx */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]))
//        fprintf(hexidecimal, "E%cA1", a[1]);
//    else return 1;
//    return 0;
//}
//
///* Skip instruction if not equal */
//int sne(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SNE Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "9%c%c0", a[1], b[1]);
//    /* SNE Vx, byte */
//    else
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==4 && strncmp(b, "0x", 2)==0 &&
//       isxdigit(b[2]) &&
//       isxdigit(b[3]))
//           fprintf(hexidecimal, "4%c%s", a[1], b+2);
//    else return 1;
//    return 0;
//}
//
///* Subtract (-) */
//int sub(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SUB Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c5", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Inverse subtract */
//int subn(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* SUBN Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c7", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
///* Exit program */
//int end(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)o, (void)tree, (void)hexidecimal;
//    /* exit */
//    fprintf(hexidecimal, "00EF");
//    return 0;
//}
//
///* Exslusive or (^) */
//int xor(char* operand, node* tree, FILE* hexidecimal)
//{
//    (void)tree;
//    char* a = strtok(operand, "\t ,");
//    char* b = strtok(NULL, "\t ,");
//    /* XOR Vx, Vy */
//    if(strlen(a)==2 && a[0]=='V' && isxdigit(a[1]) &&
//       strlen(b)==2 && b[0]=='V' && isxdigit(b[1]))
//           fprintf(hexidecimal, "8%c%c3", a[1], b[1]);
//    else return 1;
//    return 0;
//}
//
//int (*function[])(char* operand, node* tree, FILE* hexidecimal) =
//{ /*   0     1      2     3     4     5    6    7    8     9    10   11    12    13     14    15    16    17     18      19      20    21 */
//     add,  and,  call,  cls,  drw,  end,  jp,  ld,  or,  ret,  rnd,  se,  shl,  shr,  sknp,  skp,  sne,  sub,  subn, _ubyte, _uword,  xor
//};
//static char* mnemonic[] =
//{ /*   0     1      2     3     4     5    6    7    8     9    10   11    12    13     14    15    16    17     18      19      20    21 */
//    "ADD","AND","CALL","CLS","DRW","END","JP","LD","OR","RET","RND","SE","SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","UBYTE","UWORD","XOR"
//};
//
//int execute(int (*_function)(char*,node*,FILE*), char* operand, node* tree, FILE* hexidecimal)
//{
//    return _function(o, tree, hexidecimal);
//}
//
///* Bsearch callback */
//int compare(const void* a, const void* b)
//{
//    return strcmp((char*)a, *(char**)b);
//}
//
///* Assembles given a mnemonic m, an operand o, a name tree, and an output file; returns error code */
//int assemble(char* m, char* operand, node* tree, FILE* hexidecimal)
//{
//    /* Check if 'm' is a supported mnemonic */
//    #define len(mnemonic) sizeof(mnemonic)/sizeof(*mnemonic)
//    char** supported = bsearch(m, mnemonic, len(mnemonic), sizeof(char*), compare);
//    /* If 'm' is not supported return an error */
//    if(!supported)
//        return 3;
//    /* If the operand is missing and the operand is not CLS, END, or RET then return "a missing operand" error */
//    int index = supported - mnemonic;
//    if(o == NULL && (index != 3 && index != 5 && index != 9))
//        return 4;
//    /* Execute */
//    int error = execute(function[index], o, tree, hexidecimal);
//    /* Report any other errors */
//    if(!error)
//        fputc('\n', hexidecimal);
//    return error;
//}
//int main(int argc, char* argv[])
//{
//    if(argc != 3)
//        return 1;
//    /* File name for file */
//    char* file = argv[1];
//    /* File name for output */
//    char* output = argv[2];
//    /* file: assemblybly file */
//    FILE* assembly = fopen(file, "r");
//    if(!assembly)
//    {
//        fprintf(stderr, RED"error: "NRM"file %s not found\n", file);
//        return 1;
//    }
//    /* Output: hexidecimalecimal file */
//    FILE* hexidecimal = fopen(output, "w");
//    /* First scan: Gather tree */
//    node* tree = NULL;
//    char* line = NULL;
//    unsigned address = 0x0202, length = 0, linenumber = 0;
//    bool fail = false;
//    while(getline(&line, &length, assembly) != -1)
//    {
//        linenumber += 1;
//        /* Remove comment by terminating line at semicolonolon */
//        char* semicolon = index(line, ';');
//        char* newline = index(line, '\n');
//        if(semicolon)
//            *semicolon = '\0';
//        else
//            *newline = '\0';
//        /* tree end with a colon - if a colon is found, put address & name pair */
//        char* colon = index(line, ':');
//        if(colon)
//        {
//            char* name = strtok(line, ":");
//            node* exists = get(tree, name);
//            if(exists)
//            {
//                fail = true;
//                fprintf(stderr, RED"error:"NRM" file %s: line %d: name %s already defined\n",
//                    file, linenumber, name);
//            }
//            else
//                tree = add(tree, new(name, address));
//        }
//        /* Only incrememnt address if there is a mnemonic; mnemonics end with a space or a tab */
//        char* mnemonic = strtok(colon ? NULL : line, "\t ");
//        if(mnemonic)
//        {
//            /* If the found mnemonic indicates a data byte, increment the address by 1 */
//            if(strcmp(mnemonic, "UBYTE") == 0)
//                address += 0x0001;
//            /* Otherwise, for data words and opcodes, increment the address by 2 */
//            else
//                address += 0x0002;
//        }
//    }
//    /* Write reset vector to hexidecimalecimal file if entry name is found */
//    char* name = "MAIN";
//    if(!get(tree, name))
//    {
//        fail = true;
//        fprintf(stderr, RED"error:"NRM" file %s: line %d: %s undefined reference\n",
//            file, linenumber, name);
//    }
//    else
//        fprintf(hexidecimal, "1%03X\n", name->address);
//    /* Second scan: Generate hexidecimalecimal code */
//    rewind(assembly);
//    linenumber = 0;
//    unsigned error = 0;
//    while(getline(&line, &length, assembly) != -1)
//    {
//        linenumber += 1;
//        /* Remove comment by terminating line at semicolonolon */
//        char* semicolon = index(line, ';');
//        char* newline = index(line, '\n');
//        if(semicolon)
//            *semicolon = '\0';
//        else
//            *newline = '\0';
//        /* Get mnemonic and operand - skip name if colon is present */
//        char* colon = index(line, ':');
//        char* mnemonic = strtok(colon ? colon + 1 : line, "\t ");
//        char* operand = strtok(NULL, "");
//        /* If mnemonic was found then assemblyble and return error, if any */
//        if(mnemonic)
//            error = assemble(mnemonic, operand, tree, hexidecimal);
//        /* Handle error */
//        if(error)
//        {
//            fail = true;
//            char* message = "unknown error code";
//            switch(error)
//            {
//                case 1: message = "unrecognizable operands"; break;
//                case 2: message = "name not found"; break;
//                case 3: message = "unsupported chip8 mnemonic"; break;
//                case 4: message = "missing operand"; break;
//            }
//            fprintf(stderr, RED"error:"NRM" file %s: line %d: %s\n",
//                file, linenumber, message);
//        }
//    }
//    /* Clean up */
//    fileee(line);
//    delete(tree);
//    fclose(assembly);
//    fclose(hexidecimal);
//    /* If the assemblybly failed, remove the output file */
//    if(fail)
//    {
//        remove(output);
//        return 1;
//    }
//    else
//        return 0;
//}
