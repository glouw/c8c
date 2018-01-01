#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

union
{
    struct
    {
        unsigned tree : 1, generator : 1;
    }
    bits;
    unsigned any;
}
flags;

static FILE* fi;

static FILE* fo;

static char* assem;

static char* hexid;

static void fshutdown()
{
    if(fi) fclose(fi);
    if(fo) fclose(fo);
    if(flags.any)
        remove(hexid);
}

static void finit()
{
    atexit(fshutdown);
}

static void use(char* argv[])
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
}

static char* dup(char* s)
{
    int len = strlen(s) + 1;
    char* p = (char*) malloc(len);
    return p ? (char*) memcpy(p, s, len) : NULL;
}

static void reset()
{
    rewind(fi);
    rewind(fo);
}

struct node
{
    char* name;
    unsigned address;
    struct node* l;
    struct node* r;
};

static struct node* build(char* name, unsigned address)
{
    struct node* node = (struct node*) malloc(sizeof(*node));
    node->name = dup(name);
    node->address = address;
    node->l = NULL;
    node->r = NULL;
    return node;
}

static struct node* insert(struct node* nodes, struct node* node)
{
    if(nodes == NULL)
        return node;
    int difference = strcmp(node->name, nodes->name);
    if(difference == 0)
    {
        free(node->name);
        free(node);
        flags.bits.tree = true;
    }
    else if(difference < 0)
        nodes->l = insert(nodes->l, node);
    else
        nodes->r = insert(nodes->r, node);
    return nodes;
}

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
    // ADD Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c4\n", a[1], b[1]);
    // ADD I, Vx
    else
    if(strlen(a) == 1 && a[0] == 'I' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c1E\n", b[1]);
    // ADD Vx, byte
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
    // AND Vx, Vy
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
    // CALL address
    if(found)
        fprintf(fo, "2%03X\n", found->address);
    else
        return 2;
    return 0;
}

static int cls(char* operand, struct node* labels)
{
    (void )operand, (void) labels;
    fprintf(fo, "00E0\n");
    return 0;
}

static int db(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ");
    // DB
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
    // DRW Vx, Vy, nib
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
    // JP V0, address
    if(strlen(a) == 2 && a[0] == 'V' && a[1] == '0' &&
       (found = get(labels, b)))
           fprintf(fo, "B%03X\n", found->address);
    // JP ad
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
    // LD DT, Vx
    if(strlen(a) == 2 && a[0] == 'D' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c15\n", b[1]);
    // LD ST, Vx
    else
    if(strlen(a) == 2 && a[0] == 'S' && a[1] == 'T' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c18\n", b[1]);
    // LD F, Vx
    else
    if(strlen(a) == 1 && a[0] == 'F' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c29\n", b[1]);
    // LD B, Vx
    else
    if(strlen(a) == 1 && a[0] == 'B' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c33\n", b[1]);
    // LD Vx, DT
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'D' && b[1] == 'T')
           fprintf(fo, "F%c07\n", a[1]);
    // LD Vx, [I]
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 3 && b[0] == '[' && b[1] == 'I' && b[2] == ']')
           fprintf(fo, "F%c65\n", a[1]);
    // LD Vx, K
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 1 && b[0] == 'K')
           fprintf(fo, "F%c0A\n", a[1]);
    // LD Vx, Vy
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "8%c%c0\n", a[1], b[1]);
    // LD Vx, byte
    else
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
      (strlen(b) == 4 && strncmp(b, "0x", 2)) == 0 &&
      isxdigit(b[2]) &&
      isxdigit(b[3]))
           fprintf(fo, "6%c%c%c\n", a[1], b[2], b[3]);
    // LD [I], Vx
    else
    if(strlen(a) == 3 && a[0] == '[' && a[1] == 'I' && a[2] == ']' &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "F%c55\n", b[1]);
    // LD I, ad
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
    // OR Vx, Vy
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
    // RET
    fprintf(fo, "00EE\n");
    return 0;
}

static int rnd(char* operand, struct node* labels)
{
    (void) labels;
    char* a = strtok(operand, "\t ,");
    char* b = strtok(NULL, "\t ");
    // RND Vx, byte
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
    // SE Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "5%c%c0\n", a[1], b[1]);
    // SE Vx, byte
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
    // SHL Vx, Vy
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
    // SHR Vx, Vy
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
    // SKNP Vx
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
    // SKP Vx
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
    // SNE Vx, Vy
    if(strlen(a) == 2 && a[0] == 'V' && isxdigit(a[1]) &&
       strlen(b) == 2 && b[0] == 'V' && isxdigit(b[1]))
           fprintf(fo, "9%c%c0\n", a[1], b[1]);
    // SNE Vx, byte
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
    // SUB Vx, Vy
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
    // SUBN Vx, Vy
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
    // XOR Vx, Vy
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
#define size(array) sizeof(array) / sizeof(char*)
    const char** found = (const char**) bsearch(mnemonic, mnemonics, size(mnemonics), sizeof(char*), compare);
    if(!found)
        return 3;
    int index = found - mnemonics;
    return (*functions[index])(operand, labels);
}

static struct node* labels;

static void lshutdown()
{
    burn(labels);
}

static void linit()
{
    atexit(lshutdown);
}

static void scan(bool growing)
{
    unsigned address = 0x0202;
#define MAX 320
    char line[MAX];
    for(unsigned linenumber = 1; fgets(line, MAX, fi); linenumber++)
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
                if(flags.bits.tree)
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
            flags.bits.generator = true;
            fprintf(stderr, "error: line %d: %s\n", linenumber, types[error]);
            exit(1);
        }
    }
}

static void enter(const char* entry)
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
    finit();
    if(argc != 3)
        exit(1);
    use(argv);
    // First pass
    linit();
    scan(true);
    reset();
    // Reset vector
    enter("main");
    // Second pass
    scan(false);
    // Done
    exit(0);
}
