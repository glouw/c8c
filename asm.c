#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Input file.
static FILE* fi;

// Output file.
static FILE* fo;

// Name of assembly file.
static char* assem;

// Name of hex file.
static char* hexid;

// Failure flag goes high if anything went wrong during assembly.
static bool failure;

// Cleans up files. Uses failure flag to remove output hex file.
static void fshutdown()
{
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(failure)
        remove(hexid);
}

// Inits files.
static void finit(char* argv[])
{
    assem = argv[1];
    hexid = argv[2];
    // Input
    fi = fopen(argv[1], "r");
    if(fi == NULL)
    {
        fprintf(stderr, "error: %s does not exist\n", assem);
        exit(1);
    }
    // Output
    fo = fopen(argv[2], "w");
    if(fo == NULL)
    {
        fprintf(stderr, "error: %s cannot be made\n", hexid);
        exit(1);
    }
    atexit(fshutdown);
}

// Duplicates a string.
static char* dup(char* s)
{
    int len = strlen(s) + 1;
    char* p = (char*) malloc(len);
    return p ? (char*) memcpy(p, s, len) : NULL;
}

// Rewinds both the input and output files.
static void frewind()
{
    rewind(fi);
    rewind(fo);
}

// Binary tree node.
// Keeps track of label names and their addresses.
struct node
{
    char* name;
    unsigned address;
    struct node* l;
    struct node* r;
};

// New binary tree node.
static struct node* build(char* name, unsigned address)
{
    struct node* node = (struct node*) malloc(sizeof(*node));
    node->name = dup(name);
    node->address = address;
    node->l = NULL;
    node->r = NULL;
    return node;
}

// Inserts a new node into a binary tree.
static struct node* insert(struct node* nodes, struct node* node)
{
    if(nodes == NULL)
        return node;
    int difference = strcmp(node->name, nodes->name);
    if(difference == 0)
    {
        free(node->name);
        free(node);
        failure = true;
    }
    else if(difference < 0)
        nodes->l = insert(nodes->l, node);
    else
        nodes->r = insert(nodes->r, node);
    return nodes;
}

// Gets a node from a binary tree. Returns NULL if node was not found.
static struct node* get(struct node* nodes, const char* name)
{
    if(nodes == NULL)
        return NULL;
    int difference = strcmp(name, nodes->name);
    if(difference == 0)
        return nodes;
    else if(difference < 0)
        return get(nodes->l, name);
    else
        return get(nodes->r, name);
}

// Cleans up the binary tree.
static void burn(struct node* nodes)
{
    if(nodes == NULL)
        return;
    burn(nodes->l);
    burn(nodes->r);
    free(nodes->name);
    free(nodes);
}

static int add(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // ADD Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c4\n", a[1], b[1]);
    // ADD I, Vx.
    else
    if(strlen(a) == 1 && a[0] == 'I' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c1E\n", b[1]);
    // ADD Vx, byte.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(fo, "7%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int _and(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // AND Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c2\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int call(char* operand, struct node* labels)
{
    char* a = strtok(operand, "\t ");
    struct node* found = get(labels, a);
    // CALL address.
    if(found)
        fprintf(fo, "2%03X\n", found->address);
    else
        return 2;
    return 0;
}

static int cls(char* operand, struct node* labels)
{
    (void )operand, (void) labels;
    // CLS.
    fprintf(fo, "00E0\n");
    return 0;
}

static int db(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ");
    // DB (Define Byte).
    if(strlen(a) == 4 && strncmp(a, "0x", 2) == 0 &&
       isxdigit(a[2]) &&
       isxdigit(a[3]))
           fprintf(fo, "%c%c\n", a[2], a[3]);
    else
        return 1;
    return 0;
}

static int drw(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ,");
    char* c = strtok(NULL, "\t ");
    // DRW Vx, Vy, nibble.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]) &&
       strlen(c) == 3 && strncmp(c, "0x", 2) == 0 && isxdigit(c[2]))
           fprintf(fo, "D%c%c%c\n", a[1], b[1], c[2]);
    else
        return 1;
    return 0;
}

static int jp(char* operand, struct node* labels)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // JP V0, address.
    if(strlen(a) == 2 && a[0] == 'V' && a[1] == '0' &&
       (found = get(labels, b)))
           fprintf(fo, "B%03X\n", found->address);
    // JP address.
    else
    if((found = get(labels, a)))
        fprintf(fo, "1%03X\n", found->address);
    else
        return 2;
    return 0;
}

static int ld(char* operand, struct node* labels)
{
    struct node* found;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // LD DT, Vx.
    if(strlen(a) == 2 && a[0] == 'D' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c15\n", b[1]);
    // LD ST, Vx.
    else
    if(strlen(a) == 2 && a[0] == 'S' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c18\n", b[1]);
    // LD F, Vx.
    else
    if(strlen(a) == 1 && a[0] == 'F' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c29\n", b[1]);
    // LD B, Vx.
    else
    if(strlen(a) == 1 && a[0] == 'B' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c33\n", b[1]);
    // LD Vx, DT.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'D' && b[1] == 'T')
           fprintf(fo, "F%c07\n", a[1]);
    // LD Vx, [I].
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 3 && b[0] == '[' && b[1] == 'I' && b[2] == ']')
           fprintf(fo, "F%c65\n", a[1]);
    // LD Vx, K.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 1 && b[0] == 'K')
           fprintf(fo, "F%c0A\n", a[1]);
    // LD Vx, Vy.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c0\n", a[1], b[1]);
    // LD Vx, byte.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
      (strlen(b) == 4 && strncmp(b, "0x", 2)) == 0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(fo, "6%c%c%c\n", a[1], b[2], b[3]);
    // LD [I], Vx.
    else
    if(strlen(a) == 3 && a[0] == '[' && a[1] == 'I' && a[2] == ']' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c55\n", b[1]);
    // LD I, address.
    else
    if(strlen(a) == 1 && a[0] == 'I')
    {
       if((found = get(labels, b)))
           fprintf(fo, "A%03X\n", found->address);
       else
           return 2;
    }
    else
        return 1;
    return 0;
}

static int _or(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // OR Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c1\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int ret(char* operand, struct node* labels)
{
    (void) operand, (void) labels;
    // RET.
    fprintf(fo, "00EE\n");
    return 0;
}

static int rnd(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // RND Vx, byte.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(fo, "C%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int se(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SE Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "5%c%c0\n", a[1], b[1]);
    // SE Vx, byte.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(fo, "3%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int shl(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SHL Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%cE\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int shr(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SHR Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c6\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int sknp(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ");
    // SKNP Vx.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]))
        fprintf(fo, "E%cA1\n", a[1]);
    else
        return 1;
    return 0;
}

static int skp(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ");
    // SKP Vx.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]))
        fprintf(fo, "E%c9E\n", a[1]);
    else
        return 1;
    return 0;
}

static int sne(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SNE Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "9%c%c0\n", a[1], b[1]);
    // SNE Vx, byte.
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 4 && strncmp(b, "0x", 2) == 0 &&
       isxdigit(b[2]) &&
       isxdigit(b[3]))
           fprintf(fo, "4%c%c%c\n", a[1], b[2], b[3]);
    else
        return 1;
    return 0;
}

static int sub(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SUB Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c5\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int subn(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // SUBN Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c7\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int _xor(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // XOR Vx, Vy.
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c3\n", a[1], b[1]);
    else
        return 1;
    return 0;
}

static int (*functions[])(char* operand, struct node* labels) = {
    add, _and, call, cls, db, drw, jp, ld, _or, ret, rnd, se,
    shl, shr, sknp, skp, sne, sub, subn, _xor
};

static const char* mnemonics[] = {
    "ADD","AND","CALL","CLS","DB","DRW","JP","LD","OR","RET","RND","SE",
    "SHL","SHR","SKNP","SKP","SNE","SUB","SUBN","XOR"
};

static int compare(const void* a, const void* b)
{
    return strcmp((char*)a, *(char**)b);
}

static int assemble(char* mnemonic, char* operand, struct node* labels)
{
    const char** found = (const char**) bsearch(
        mnemonic,
        mnemonics,
        sizeof(mnemonics) / sizeof(*mnemonics),
        sizeof(*mnemonics),
        compare);
    if(!found)
        return 3;
    return (*functions[found - mnemonics])(operand, labels);
}

static struct node* scan(struct node* labels)
{
    unsigned address = 0x0202;
    const bool growing = labels == NULL;
    char line[320];
    for(unsigned linenumber = 1; fgets(line, sizeof(line), fi); linenumber++)
    {
        char* label;
        char* mnemonic;
        char* operand;
        char* newline;
        char* semicolon;
        char* colon;
        int error = 0;
        newline = strchr(line, '\n');
        if(newline)
            *newline = '\0';
        semicolon = strchr(line, ';');
        if(semicolon)
            *semicolon = '\0';
        colon = strchr(line, ':');
        if(colon)
        {
            label = strtok(line, "\t :");
            if(growing)
            {
                labels = insert(labels, build(label, address));
                if(failure)
                {
                    fprintf(stderr, "error: line %d: %s already defined\n", linenumber, label);
                    exit(1);
                }
            }
        }
        mnemonic = strtok(colon ? NULL : line, "\t ");
        operand = strtok(NULL, "");
        if(mnemonic)
        {
            if(growing)
                address += strcmp(mnemonic, "DB") == 0 ? 0x0001 : 0x0002;
            else
                error = assemble(mnemonic, operand, labels);
        }
        if(error)
        {
            const char* types[] = {
                /* 0 */ "no error",
                /* 1 */ "operand formatting",
                /* 2 */ "label not found",
                /* 3 */ "unsupported chip8 mnemonic"
            };
            failure = true;
            fprintf(stderr, "error: line %d: %s\n", linenumber, types[error]);
            exit(1);
        }
    }
    return labels;
}

// Generates the reset vector for the entry label.
static void rvec(const char* entry, struct node* labels)
{
    struct node* reset = get(labels, entry);
    if(!reset)
    {
        fprintf(stderr, "error: entry point %s not found\n", entry);
        exit(1);
    }
    fprintf(fo, "1%03X\n", reset->address);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "expected input and output arguments");
        exit(1);
    }
    finit(argv);
    // First pass.
    struct node* labels = NULL;
    labels = scan(labels);
    frewind();
    // Second pass.
    rvec("main", labels);
    scan(labels);
    burn(labels);
    exit(0);
}
