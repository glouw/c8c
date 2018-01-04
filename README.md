# c8c

c8c aims to be a relatively small typeless c-like programming language for the CHIP-8 platform:

    mul(a, b)
    {
        if(b == 0)
        {
            return 0;
        }
        return a + mul(a, b - 1);
    }

    main()
    {
        putnum(24, 13, mul(9, 9));
        while(1)
        {
            // Wait here forever.
        }
    }

![Screenshot](scrots/2018-01-04-003042_512x256_scrot.png)

First build the tool chain:

    make

Note that SDL2 is required.

To build your own c8 code, piecewise invoke the toolchain:

    c8c main.c8 main.asm

    asm main.asm main.hex

    bin main.hex main.bin

    emu main.bin
