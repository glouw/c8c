#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int mem[4096], sp=0x0FFF;

static char* scan(char* line, int* i)
{
    while(--(*i) >= 0)
        if(!isalnum(*(line[*i])))
            return line[*i];
    return NULL;
}

int calc(char* line)
{
    char* operator, *expr = strdup(line);
    int len = strlen(expr);
    while(operator = scan(expr, &len))
    {
        char* operand = operator + 1;
        if(*operand)
            mem[sp--] = atoi(operand);
        switch(*operator)
        {
            case '+': ++sp; mem[sp+1] += mem[sp]; break;
            case '-': ++sp; mem[sp+1] -= mem[sp]; break;
            case '*': ++sp; mem[sp+1] *= mem[sp]; break;
            case '/': ++sp; mem[sp+1] /= mem[sp]; break;
            default:
                break;
        }
        *operator = '\0';
    }
    free(expr);
    return sp;
}

int main(void)
{
    return 0;
}
