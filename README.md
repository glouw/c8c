# c8c

c8c aims to be a relatively small c-like programming language for the chip8 platform.
A virtual machine (emu) and assembler (asm) is also included. Running:

    make;

will build the compiler, assembler, and virtual machine, and then build all the
unit tests (tasm, tc8c) for the virtual machine using the assembler and compiler.

If you're looking to build and run your own c8 code then piecewise invoke the toolchain:

    c8c main.c8 main.asm

    asm main.asm main.hex

    bin main.hex main.bin

    emu main.bin
