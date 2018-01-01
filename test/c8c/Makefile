ASM = ../../asm/asm
BIN = xxd -r -p
CMP = ../../c8c/c8c

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
