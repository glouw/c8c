#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    const char* argv[1] = { "Gustav J. Louw" };
    /* const char* assures the data pointed to by the argv array
     * is not modifiable  */
    argv[0][4] = 'Z';
}
