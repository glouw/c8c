CC = gcc -std=c99

CFLAGS = -Wshadow -Wall -Wpedantic -Wextra
CFLAGS+= -Ofast -march=native

LDFLAGS = -lSDL2

all: emu bin asm c8c
	make clean -C tasm
	make clean -C tc8c
	make clean -C examples
	make -C tasm
	make -C tc8c
	make -C examples

emu: emu.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

bin: bin.c
	$(CC) $(CFLAGS) $^ -o $@

asm: asm.c
	$(CC) $(CFLAGS) $^ -o $@

c8c: c8c.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f c8c
	rm -f asm
	rm -f bin
	rm -f emu
	make clean -C tasm
	make clean -C tc8c
	make clean -C examples
