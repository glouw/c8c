![Screenshot](scrots/c8c.png)

c8c aims to be a small typeless programming language for the CHIP-8 virtual machine:

    r = { 0x80, 0x40, 0x20, 0x10 };
    l = { 0x20, 0x40, 0x80, 0x10 };

    main()
    {
        auto x = 0, xmax = 64, dx = 4;
        auto y = 0, ymax = 32, dy = 4;
        while(y < ymax)
        {
            if(rand() & 0x1)
            {
                draw(x, y, r);
            }
            else
            {
                draw(x, y, l);
            }
            if((x += dx) == xmax)
            {
                x = 0;
                y += dy;
            }
        }
        while(1)
        {
            // Never leave main.
        }
    }

![Screenshot](scrots/2018-01-04-140651_512x256_scrot.png)

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
        putchar(24, 13, mul(9, 9));
        while(1)
        {
            // Never leave main.
        }
    }

![Screenshot](scrots/2018-01-31-130004_512x256_scrot.png)

Just run:

    make

This will build the CHIP-8 virtual machine, binner, assembler, and compiler.
The compiler will then build all the unit tests (tc8c) and example code pieces (examples).

To run a compiled binary, invoke the virtual machine:

    ./emu examples/maze.bin

Hit the END key to exit.

To build your own c8 code, piecewise invoke the toolchain:

    ./c8c main.c8 main.asm

    ./asm main.asm main.hex

    ./bin main.hex main.bin

    ./emu main.bin
