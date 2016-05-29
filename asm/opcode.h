#pragma once

const struct opcode
{
    /* Generates machine code */
    int (*assemble)(char* m, char* o, node* labels, FILE* hexid);
}
opcode;
