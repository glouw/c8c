#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

static FILE* fo = NULL;
static FILE* fi = NULL;
static int linenumber = 1;
static int sp = 1;
static bool defining = false;
static struct frame { int sp; } frames[12];
static int fp = 0;
static int rp = 4096;

static bool hex(const char* word)
{
    return word[0] == '0' && word[1] == 'x';
}

static void _ld(const char* word)
{
    const int integer = hex(word) ? strtol(word, NULL, 0) : atoi(word);
    fprintf(fo, "LD V%1X, 0x%02X\n", sp, integer);
    sp++;
}

static void _and(void) { fprintf(fo, "AND V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _add(void) { fprintf(fo, "SUM V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _sub(void) { fprintf(fo, "SUB V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _or (void) { fprintf(fo, "OR  V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _xor(void) { fprintf(fo, "XOR V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _shl(void) { fprintf(fo, "SHL V%1X, V%1X\n", sp - 1, sp - 1);       }
static void _shr(void) { fprintf(fo, "SHR V%1X, V%1X\n", sp - 1, sp - 1);       }
static void _pop(void) { sp--;                                                  }
static void _let(void) { defining = true; }
static void _eql(void) { defining = false; }
static void _ret(void) { fprintf(fo, "RET\n"); }

static void enter(const int arguments)
{
    frames[fp].sp = sp;
    // Save the stack to RAM
    fprintf(fo, "LD I, 0x%03X\n", rp -= frames[fp].sp);
    fprintf(fo, "LD [I], V%1X\n", frames[fp].sp - 1);
    // The last element of this stack is copied over to the new stack
    fprintf(fo, "LD V0, V%1X\n", frames[fp].sp - 1);
    fp++;
    sp = 1;
}

static void done(void)
{
}

enum type
{
    function, operator
};

struct token
{
    char* name;
    void (*generate)(void);
    enum type type;
    struct token* l;
    struct token* r;
}
*tokens = NULL;

struct token* build(char* name, enum type type, void (*generate)(void))
{
    struct token* token = malloc(sizeof(*token));
    token->name = strdup(name);
    token->type = type;
    token->generate = generate;
    token->l = NULL;
    token->r = NULL;
    return token;
}

void kill(struct token* token)
{
    free(token->name);
    free(token);
}

struct token* insert(struct token* head, struct token* token)
{
    if(head == NULL)
        return token;
    int cmp = strcmp(token->name, head->name);
    if(cmp == 0)
    {
        fprintf(stderr, "error: line %d: token '%s' already defined\n", linenumber, token->name);
        kill(token);
        exit(1);
    }
    else if(cmp < 0)
        head->l = insert(head->l, token);
    else
        head->r = insert(head->r, token);
    return head;
}

struct token* search(struct token* head, char* name)
{
    if(head == NULL)
        return NULL;
    int cmp = strcmp(name, head->name);
    if(cmp == 0)
        return head;
    else if(cmp < 0)
        return search(head->l, name);
    else
        return search(head->r, name);
}

void destroy(struct token* head)
{
    if(head == NULL)
        return;
    destroy(head->l);
    destroy(head->r);
    kill(head);
}

static void print(struct token* head)
{
    if(head == NULL)
        return;
    print(head->l);
    print(head->r);
    fprintf(stderr, "%s\n", head->name);
}

static void cleanup(void)
{
    if(fo) fclose(fo);
    if(fi) fclose(fi);
    destroy(tokens);
}

static bool isxnum(const char* word)
{
    for(char* temp = (char*)(word + 2); *temp; temp++)
        if(!isxdigit(*temp))
            return false;
    return true;
}

static bool isnum(const char* word)
{
    for(char* temp = (char*)(word + 0); *temp; temp++)
        if(!isdigit(*temp))
            return false;
    return true;
}

static bool loadable(const char* word)
{
    return hex(word) ? isxnum(word) : isnum(word);
}

static int extract(const char* word)
{
    char* a = strchr(word, '(');
    if(a == NULL)
        return 0;
    const char* arguments = strtok(a, ")");
    return atoi(arguments);
}

static void compile(char* line)
{
    char* word = NULL;
    const char* const delims = "\n #";
    while((word = strtok(line, delims)))
    {
        line = NULL;
        // Load
        if(loadable(word))
        {
            _ld(word);
            continue;
        }
        // Operator or function
        const struct token* token = search(tokens, word);
        if(token && token->type == operator)
        {
            token->generate();
            continue;
        }
        if(token && token->type == function)
        {
            const int arguments = extract(word);
            printf("args: %d\n", arguments);
            enter(arguments);
            fprintf(fo, "CALL %s\n", word);
            done();
            continue;
        }
        // Token function insert
        if(defining)
        {
            fprintf(fo, "%s:\n", word);
            tokens = insert(tokens, build(word, function, NULL));
            continue;
        }
        if(token == NULL)
        {
            fprintf(stderr, "error: line %d: %s not defined\n", linenumber, word);
            exit(1);
        }
    }
}

static void init(void)
{
    tokens = insert(tokens, build("+",  operator, _add)); // Addition
    tokens = insert(tokens, build("-",  operator, _sub)); // Subtraction
    tokens = insert(tokens, build("&",  operator, _and)); // Bitwise AND
    tokens = insert(tokens, build("|",  operator, _or )); // Bitwise OR
    tokens = insert(tokens, build("^",  operator, _xor)); // Bitwise XOR
    tokens = insert(tokens, build("<<", operator, _shl)); // Bitshift left
    tokens = insert(tokens, build(">>", operator, _shr)); // Bitshift right
    tokens = insert(tokens, build("let",operator, _let)); // For defining
    tokens = insert(tokens, build("=",  operator, _eql)); // For ending a definition
    tokens = insert(tokens, build(";",  operator, _ret)); // End a routine
    tokens = insert(tokens, build(".",  operator, _pop)); // Pop the stack
}

int main(int argc, char* argv[])
{
    if(argc != 1)
        return 1;
    atexit(cleanup);
    fi = stdin;
    fo = stdout;
    if(!fi)
    {
        fprintf(stderr, "%s not found\n", argv[1]);
        exit(1);
    }
    init();
    print(tokens);
    #define MAX 180
    char line[MAX];
    while(fgets(line, MAX, fi))
    {
        compile(line);
        linenumber++;
    }
}
