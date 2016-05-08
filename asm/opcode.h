#pragma once

const struct opcode
{
    int (*assemble)(char* m, char* o, node* labels, FILE* hexid);
}
opcode;
