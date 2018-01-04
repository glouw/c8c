# c8c

c8c aims to be a relatively small c-like programming language for the CHIP-8 platform:

    multiply(a, b)
    {
        if(b == 0)
        {
            return 0;
        }
        return a + multiply(a, b - 1);
    }

    main()
    {
        putnum(24, 13, multiply(9, 9));
        while(1)
        {
            // Wait here forever.
        }
    }

First build the tool chain:

    make

To build your own c8 code, piecewise invoke the toolchain:

    c8c main.c8 main.asm

    asm main.asm main.hex

    bin main.hex main.bin

    emu main.bin
