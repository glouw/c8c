# c8c

c8c aims to be a relatively small c-like programming language for the chip8 platform.
Included is a virtual machine (emu) and assembler (asm).

Running:

    make

will build the compiler, assembler, and virtual machine.
Assembly unit tests (tasm) and source unit tests (tc8c) will then be built with the assembler and compiler.

C8C is typeless. Everything is one byte in size.

You'll be right at home with the c8c syntax:

    add(a, b)
    {
        return a + b;
    }

    sub(a, b)
    {
        return a - b;
    }

    main()
    {
        byte a = 0x0;
        while(1)
        {
            if(1) { a = sub(42, 11); } else { a = add(11, 42); }
        }
    }

If you're looking to build and run your own c8 code then piecewise invoke the toolchain:

    c8c main.c8 main.asm

    asm main.asm main.hex

    bin main.hex main.bin

    emu main.bin
