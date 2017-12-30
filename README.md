# c8c

c8c aims to be a relatively small c-like programming language for the chip8 platform.
A virtual machine (emu) and assembler (asm) is also included. Running:

    make;

Will build the compiler, assembler, and virtual machine, and then build all the
unit tests for the virtual machine using the compiler and assembler.

Outside of that, if you're looking to build and run your own c8 code:

Compile your c8 program:

    c8c main.c8 main.asm

Assemble it:

    asm main.asm main.hex

Bin it:

    bin main.hex main.bin

And run it:

    emu main.bin
