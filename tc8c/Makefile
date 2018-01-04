ASM = ../asm
BIN = ../bin
CMP = ../c8c

BINS = fun.bin logical.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

fun.bin: fun.hex
	$(BIN) $^ $@
fun.hex: fun.asm
	$(ASM) $^ $@
fun.asm: fun.c8
	$(CMP) $^ $@

logical.bin: logical.hex
	$(BIN) $^ $@
logical.hex: logical.asm
	$(ASM) $^ $@
logical.asm: logical.c8
	$(CMP) $^ $@

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
	rm -f $(ASMS)
