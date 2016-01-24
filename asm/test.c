#include <stdio.h>
#include <string.h>

int main(void)
{
    char* str = "0";
    printf("%d\n", strncmp(str, "0x", 2));
}
