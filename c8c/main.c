#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

static FILE* fo = NULL;
static FILE* fi = NULL;
static unsigned linenumber = 1;
static unsigned sp = 0;

static union
{
    struct
    {
        unsigned defining : 1;
    };
    unsigned any;
}
flags;

static void _and(void) { fprintf(fo, "AND V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _add(void) { fprintf(fo, "SUM V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _sub(void) { fprintf(fo, "SUB V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _or (void) { fprintf(fo, "OR  V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _xor(void) { fprintf(fo, "XOR V%1X, V%1X\n", sp - 2, sp - 1); sp--; }
static void _shl(void) { fprintf(fo, "SHL V%1X, V%1X\n", sp - 1, sp - 1);       }
static void _shr(void) { fprintf(fo, "SHR V%1X, V%1X\n", sp - 1, sp - 1);       }
static void _let(void) { flags.defining = true; sp = 0;                         }
static void _eql(void) { flags.defining = false;                                }
static void _ret(void) { fprintf(fo, "RET\n");                                  }

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
};

static struct token* build(char* name, enum type type, void (*generate)(void))
{
    struct token* token = malloc(sizeof(*token));
    token->name = strdup(name);
    token->type = type;
    token->generate = generate;
    token->l = NULL;
    token->r = NULL;
    return token;
}

static void kill(struct token* token)
{
    free(token->name);
    free(token);
}

static struct token* insert(struct token* head, struct token* token)
{
    if(head == NULL)
        return token;
    int compare = strcmp(token->name, head->name);
    if(compare == 0)
    {
        fprintf(stderr, "error: line %d: token '%s' already defined\n", linenumber, token->name);
        kill(token);
        exit(1);
    }
    else if(compare < 0)
        head->l = insert(head->l, token);
    else
        head->r = insert(head->r, token);
    return head;
}

static struct token* search(struct token* head, char* name)
{
    if(head == NULL)
        return NULL;
    int compare = strcmp(name, head->name);
    if(compare == 0)
        return head;
    else if(compare < 0)
        return search(head->l, name);
    else
        return search(head->r, name);
}

static void destroy(struct token* head)
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

static struct token* tokens;

static void cleanup(void)
{
    if(fo) fclose(fo);
    if(fi) fclose(fi);
    destroy(tokens);
}

static bool hex(char* word)
{
    return word[0] == '0' && word[1] == 'x';
}

static void load(char* word)
{
    int integer = hex(word) ? strtol(word, NULL, 0) : atoi(word);
    fprintf(fo, "LD V%1X, 0x%02X\n", sp, integer);
    sp++;
}

static bool isxnum(char* word)
{
    for(char* temp = word + 2; *temp; temp++)
        if(!isxdigit(*temp))
            return false;
    return true;
}

static bool isnum(char* word)
{
    for(char* temp = word + 0; *temp; temp++)
        if(!isdigit(*temp))
            return false;
    return true;
}

static bool loadable(char* word)
{
    return hex(word) ? isxnum(word) : isnum(word);
}

static void compile(char* line)
{
    char* word = NULL;
    char* delims = "\n #";
    while((word = strtok(line, delims)))
    {
        line = NULL;
        // Load
        if(loadable(word))
        {
            load(word);
            continue;
        }
        // Operator or function
        struct token* token = search(tokens, word);
        if(token && token->type == operator)
        {
            token->generate();
            continue;
        }
        // Token function insert
        if(flags.defining)
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
    fi = stdin;
    fo = stdout;
    #define X(name, generate) tokens = insert(tokens, build(name, operator, generate));
    #define VARIABLES  \
        X("+",   _add) \
        X("-",   _sub) \
        X("&",   _and) \
        X("|",   _or)  \
        X("^",   _xor) \
        X("<<",  _shl) \
        X(">>",  _shr) \
        X("let", _let) \
        X("=",   _eql) \
        X(".",   _ret)
    VARIABLES
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    atexit(cleanup);
    init();
    print(tokens);
    #define MAX 360
    char line[MAX];
    while(fgets(line, MAX, fi))
    {
        compile(line);
        linenumber++;
    }
}
