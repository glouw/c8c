ASM = ../asm
BIN = ../bin
CMP = ../c8c

BINS = logical0.bin logical1.bin assignment.bin
BINS+= sizeof.bin branching.bin while.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

while.bin: while.hex
	$(BIN) $^ $@
while.hex: while.asm
	$(ASM) $^ $@
while.asm: while.c8
	$(CMP) $^ $@

branching.bin: branching.hex
	$(BIN) $^ $@
branching.hex: branching.asm
	$(ASM) $^ $@
branching.asm: branching.c8
	$(CMP) $^ $@

sizeof.bin: sizeof.hex
	$(BIN) $^ $@
sizeof.hex: sizeof.asm
	$(ASM) $^ $@
sizeof.asm: sizeof.c8
	$(CMP) $^ $@

assignment.bin: assignment.hex
	$(BIN) $^ $@
assignment.hex: assignment.asm
	$(ASM) $^ $@
assignment.asm: assignment.c8
	$(CMP) $^ $@

logical0.bin: logical0.hex
	$(BIN) $^ $@
logical0.hex: logical0.asm
	$(ASM) $^ $@
logical0.asm: logical0.c8
	$(CMP) $^ $@

logical1.bin: logical1.hex
	$(BIN) $^ $@
logical1.hex: logical1.asm
	$(ASM) $^ $@
logical1.asm: logical1.c8
	$(CMP) $^ $@

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
	rm -f $(ASMS)
