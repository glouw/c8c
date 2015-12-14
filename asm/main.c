#include <stdio.h>
#include <string.h>

int main(void)
{
    for(;;)
    {
        #define SIZE 100
        char cmd[SIZE] = { '\0' };
        fgets(cmd, SIZE, stdin);
             if(!strncmp(cmd, "CLS" , 3)) printf("00E0\n"                   ); /*00E0*/
        else if(!strncmp(cmd, "RET" , 3)) printf("00EE\n"                   ); /*00EE*/
        else if(!strncmp(cmd, "JP"  , 2)) printf("1%s\n"   , cmd+3          ); /*1NNN*/
        else if(!strncmp(cmd, "CALL", 4)) printf("2%s\n"   , cmd+5          ); /*2NNN*/
        else if(!strncmp(cmd, "ADD" , 3)) printf("7%c%s\n" , cmd[5], cmd+8  ); /*7XKK*/
        else if(!strncmp(cmd, "OR " , 2)) printf("8%c%c1\n", cmd[4], cmd[8] ); /*8XY1*/
        else if(!strncmp(cmd, "AND" , 3)) printf("8%c%c2\n", cmd[5], cmd[9] ); /*8XY2*/
        else if(!strncmp(cmd, "XOR" , 3)) printf("8%c%c3\n", cmd[5], cmd[9] ); /*8XY3*/
        else if(!strncmp(cmd, "ADD" , 3)) printf("8%c%c4\n", cmd[5], cmd[9] ); /*8XY4*/
        else if(!strncmp(cmd, "SHR" , 3)) printf("8%c%c6\n", cmd[5], cmd[9] ); /*8XY6*/
        else if(!strncmp(cmd, "SHL" , 3)) printf("8%c%cE\n", cmd[5], cmd[9] ); /*8XYE*/
             if(!strncmp(cmd, "SUBN", 4)) printf("8%c%c7\n", cmd[6], cmd[10]); /*8XY7*/
        else if(!strncmp(cmd, "SUB" , 3)) printf("8%c%c5\n", cmd[5], cmd[9] ); /*8XY5*/
        /* SNE DISAMBUGATION */
             if(!strncmp(cmd, "SNE" , 3)                                       /*9XY0*/
                && cmd[4] == 'V'
                && cmd[8] == 'V')
                    printf("9%c%c0\n", cmd[5], cmd[9]);
        else if(!strncmp(cmd, "SNE" , 3))                                      /*4XKK*/
                printf("4%c%s\n", cmd[5], cmd+8);
        /* LD DISAMBGUTATION */
             if(!strncmp(cmd, "LD", 2)
                && cmd[3] == 'V'
                && cmd[7] == 'V')
                    printf("8%c%c0\n", cmd[4], cmd[8]);
        else if(!strncmp(cmd, "LD", 2)                                         /*ANNN*/
                && cmd[3] == 'I')
                    printf("A%s\n", cmd+6);
        else if(!strncmp(cmd, "LD", 2))                                        /*6XKK*/
                    printf("6%c%s\n", cmd[4], cmd+7);
        /* SE DISAMBUGATION */
             if(!strncmp(cmd, "SE", 2)                                         /*5XY0*/
                && cmd[3] == 'V'
                && cmd[7] == 'V')
                    printf("5%c%c0\n", cmd[4], cmd[8]);
        else if(!strncmp(cmd, "SE", 2))                                        /*3XKK*/
                    printf("3%c%s\n", cmd[4], cmd+7);
    }
}
