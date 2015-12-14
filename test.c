#include <stdio.h>

void speak(void)
{
    puts("HI THERE!");
}

void (*list[1])(void) = { speak };

int main(void)
{
    void (*run)(void) = list[0];
    run();
    return 0;
}
