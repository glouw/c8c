CC = gcc

# Warnings flags.
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra -Wimplicit-fallthrough=5

# Optimization flags.
CFLAGS+= -Ofast -march=native

# Linker flags.
LDFLAGS = -lSDL2

emu: emu.o
	$(CC) $(CFLAGS) $(LDFLAGS) emu.c

asm: asm.o
	$(CC) $(CFLAGS) asm.c

c8c: c8c.o
	$(CC) $(CFLAGS) c8c.c

all:
	emu
	asm
	c8c
	make -C test/asm
	make -C test/c8c

clean:
	rm -f asm
	rm -f emu
	rm -f c8c
	rm -f asm.o
	rm -f emu.o
	rm -f c8c.o
	make clean -C test/asm
	make clean -C test/c8c
