#pragma once

/* assembles given mnemonic m, an operand o, a label tree, and an output file; returns error code */
int assemble(char* m, char* o, tnode* labels, FILE* hexid);
