#pragma once

#include "tree.h"

const struct opcodes
{
    int (*assemble)(char* mnemonic, char* operand, struct node* tree);
}
opcodes;
