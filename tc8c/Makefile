ASM = ../asm
BIN = ../bin
CMP = ../c8c

BINS = logical.bin mul.bin maze.bin tty.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

tty.bin: tty.hex
	$(BIN) $^ $@
tty.hex: tty.asm
	$(ASM) $^ $@
tty.asm: tty.c8
	$(CMP) $^ $@

maze.bin: maze.hex
	$(BIN) $^ $@
maze.hex: maze.asm
	$(ASM) $^ $@
maze.asm: maze.c8
	$(CMP) $^ $@

logical.bin: logical.hex
	$(BIN) $^ $@
logical.hex: logical.asm
	$(ASM) $^ $@
logical.asm: logical.c8
	$(CMP) $^ $@

mul.bin: mul.hex
	$(BIN) $^ $@
mul.hex: mul.asm
	$(ASM) $^ $@
mul.asm: mul.c8
	$(CMP) $^ $@

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
	rm -f $(ASMS)
