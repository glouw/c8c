ASM = ../asm
BIN = ../bin
CMP = ../c8c

BINS = mul.bin maze.bin tty.bin
BINS+= collision.bin invaders.bin
HEXS = $(BINS:.bin=.hex)
ASMS = $(BINS:.bin=.asm)

all: $(BINS)

collision.bin: collision.hex
	$(BIN) $^ $@
collision.hex: collision.asm
	$(ASM) $^ $@
collision.asm: collision.c8
	$(CMP) $^ $@

invaders.bin: invaders.hex
	$(BIN) $^ $@
invaders.hex: invaders.asm
	$(ASM) $^ $@
invaders.asm: invaders.c8
	$(CMP) $^ $@

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
