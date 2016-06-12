#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool fail;
char* input;
char* output;
FILE* assembly;
FILE* hexidecimal;
unsigned linenumber;

struct node
{
    char* name;
    unsigned address;
    struct node* left;
    struct node* rite;
};

struct node* new(char* name, unsigned address)
{
    struct node* node = malloc(sizeof(struct node));
    node->name = strdup(name);
    node->address = address;
    node->left = NULL;
    node->rite = NULL;
    return node;
}

struct node* insert(struct node* tree, struct node* node)
{
    int difference; 
    if(tree == NULL)
        return node;
    difference = strcmp(node->name, tree->name);
    if(difference == 0)
    {
        fprintf(stderr, "error: file %s: line %d: label \"%s\" already defined\n",
            input, linenumber, node->name);
        fail = true;
        free(node->name);
        free(node);
    }
    else if(difference < 0)
        tree->left = insert(tree->left, node);
    else
        tree->rite = insert(tree->rite, node);
    return tree;
}

struct node* get(struct node* tree, char* name)
{
    int difference;
    if(tree == NULL)
        return NULL;
    difference = strcmp(name, tree->name);
    if(!difference)
        return tree;
    else if(difference < 0)
        return get(tree->left, name);
    else
        return get(tree->rite, name);
}

void burn(struct node* tree)
{
    if(tree == NULL)
        return;
    burn(tree->left);
    burn(tree->rite);
    free(tree->name);
    free(tree);
}

void print(struct node* tree)
{
    if(tree == NULL)
        return;
    print(tree->left);
    fprintf(stderr, "%04X: %s\n", tree->address, tree->name);
    print(tree->rite);
}

int add(char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, ",");
    char* b = strtok(NULL, "\t ");
    // ADD Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c4\n", a[1], b[1]);
    // ADD I, Vx
    else 
    if(strlen(a) == 1 && a[0] == 'I' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c1E\n", b[1]);
    // ADD Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(hexidecimal, "7%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

int and(char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, ",");
    char* b = strtok(NULL, "\t ");
    // AND Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c2\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

int call(char* operand, struct node* tree)
{
    char* a = strtok(operand, "\t ");
    struct node* found = get(tree, a);
    // CALL address
    if(found)
        fprintf(hexidecimal, "2%03X\n", found->address);
    else
        return 2;
    return 0;
}

int cls(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int db(char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ");
    // DB
    if(strlen(a) == 4 && strncmp(a, "0x", 2) == 0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]))
           fprintf(hexidecimal, "%c%c\n", a[2], a[3]);
    else
        return 1;
    return 0;
}

int drw(char* operand, struct node* tree)
{
    (void)tree;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    char* c = strtok(NULL, "\t ");
    // DRW Vx, Vy, nib
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]) &&
       strlen(c) == 3 && strncmp(c, "0x", 2) == 0 && isxdigit(c[2]))
           fprintf(hexidecimal, "D%c%c%c\n", a[1], b[1], c[2]);
    else
        return 1;
    return 0;
}

int end(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    // End
    fprintf(hexidecimal, "00EF\n");
    return 0;
}

int jp(char* operand, struct node* tree)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // JP V0, address
    if(strlen(a) == 2 && a[0] == 'V' && a[1] == '0' &&
       (found = get(tree, b)))
           fprintf(hexidecimal, "B%03X\n", found->address);
    // JP ad
    else
    if((found = get(tree, a)))
        fprintf(hexidecimal, "1%03X\n", found->address);
    else
        return 2;
    return 0;
}

int ld(char* operand, struct node* tree)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // LD DT, Vx
    if(strlen(a) == 2 && a[0] == 'D' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c15\n", b[1]);
    // LD ST, Vx
    else
    if(strlen(a) == 2 && a[0] == 'S' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c18\n", b[1]);
    // LD F, Vx
    else
    if(strlen(a) == 1 && a[0] == 'F' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c29\n", b[1]);
    // LD B, Vx
    else
    if(strlen(a) == 1 && a[0] == 'B' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c33\n", b[1]);
    // LD Vx, DT
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'D' && b[1] == 'T')
           fprintf(hexidecimal, "F%c07\n", a[1]);
    // LD Vx, [I]
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 3 && b[0] == '[' && b[1] == 'I' && b[2] == ']')
           fprintf(hexidecimal, "F%c65\n", a[1]);
    // LD Vx, K
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 1 && b[0] == 'K')
           fprintf(hexidecimal, "F%c0A\n", a[1]);
    // LD Vx, Vy
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "8%c%c0\n", a[1], b[1]);
    // LD Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
      (strlen(b) == 4 && strncmp(b, "0x", 2)) == 0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(hexidecimal, "6%c%c%c\n", a[1], b[2], b[3]);
    // LD [I], Vx
    else
    if(strlen(a) == 3 && a[0] == '[' && a[1] == 'I' && a[2] == ']' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(hexidecimal, "F%c55\n", b[1]);
    // LD I, ad
    else
    if(strlen(a) == 1 && a[0] == 'I')
    {
       if((found = get(tree, b)))
           fprintf(hexidecimal, "A%03X\n", found->address);
       else
           return 2;
    }
    else
        return 1;
    return 0;
}

int or(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int ret(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int rnd(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int se(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int shl(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int shr(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int sknp(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int skp(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int sne(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int sub(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int subn(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int xor(char* operand, struct node* tree)
{
    (void)operand, (void)tree;
    return 0;
}

int (*functions[])(char* operand, struct node* tree) =
{
    add, and, call, cls, db, drw, end, jp, ld, or, ret, rnd, se,
    shl, shr, sknp, skp, sne, sub, subn, xor
};

char* operations[] =
{
    "ADD","AND","CALL","CLS","DB","DRW","END","JP","LD","OR","RET","RND","SE",
    "SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","XOR"
};

int compare(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

#define size(array) sizeof(array) / sizeof(char*)

int assemble(char* operation, char* operand, struct node* tree)
{
    char** found = (char**)bsearch(operation, operations, size(operations), sizeof(char*), compare);
    int index = found - operations;
    int error = (*functions[index])(operand, tree);
    return error;
}

struct node* scan(struct node* tree)
{
    unsigned size = 0;
    unsigned address = 0x0202;
    char* line = NULL;
    bool grow = tree ? false : true;
    for(linenumber = 1; getline(&line, &size, assembly) != -1; linenumber++)
    {
        char* label;
        char* operation;
        char* operand;
        char* newline;
        char* semicolon;
        char* colon;
        int error = 0;
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
            if(grow)
                tree = insert(tree, new(label, address));
        }
        operation = strtok(colon ? NULL : line, "\t ");
        operand = strtok(NULL, "");
        if(operation)
        {
            if(grow)
                address += strcmp(operation, "DB") == 0 ? 0x0001 : 0x0002;
            else
                error = assemble(operation, operand, tree);
        }
        if(error)
            fail = true;
        if(error == 1)
            fprintf(stderr, "error: %s: line %d: weak operands\n", input, linenumber);
        else
        if(error == 2)
            fprintf(stderr, "error: %s: line %d: missing label\n", input, linenumber);
    }
    free(line);
    return tree;
}

int main(int argc, char* argv[])
{
    struct node* tree;
    struct node* reset;
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
    // First pass
    tree = scan(NULL);
    // Reset vector
    reset = get(tree, "MAIN");
    if(reset)
        fprintf(hexidecimal, "1%03X\n", reset->address);
    else
    {
        fail = true;
        fprintf(stderr, "error: %s label MAIN not found\n", input);
    }
    // Second pass
    rewind(assembly);
    scan(tree);
    // Cleanup
    print(tree);
    burn(tree);
    fclose(assembly);
    fclose(hexidecimal);
    if(fail)
    {
        remove(output);
        return 1;
    }
    return 0;
}
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
