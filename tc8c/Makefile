ASM = ../asm
BIN = ../bin
CMP = ../c8c

BINS = fun.bin logical.bin mul.bin maze.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

fun.bin: fun.hex
	$(BIN) $^ $@
fun.hex: fun.asm
	$(ASM) $^ $@
fun.asm: fun.c8
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
