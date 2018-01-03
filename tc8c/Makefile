ASM = ../asm
BIN = ../bin
CMP = valgrind --leak-check=full ../c8c

BINS = fun.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

fun.bin: fun.hex
	$(BIN) $^ $@
fun.hex: fun.asm
	$(ASM) $^ $@
fun.asm: fun.c8
	$(CMP) $^ $@

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
	rm -f $(ASMS)
