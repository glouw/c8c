ASM = ../asm/asm
BIN = xxd -r -p
CMP = ../c8c/c8c

BINS = registers.bin flow.bin subroutines.bin skips.bin
BINS+= timers.bin keypad.bin graphics.bin storage.bin
BINS+= helloworld.bin
HEXS = $(BINS:.bin=.hex)

all: $(BINS)

registers.bin: registers.hex
	$(BIN) registers.hex registers.bin
registers.hex: registers.asm
	$(ASM) registers.asm registers.hex

flow.bin: flow.hex
	$(BIN) flow.hex flow.bin
flow.hex: flow.asm
	$(ASM) flow.asm flow.hex

subroutines.bin: subroutines.hex
	$(BIN) subroutines.hex subroutines.bin
subroutines.hex: subroutines.asm
	$(ASM) subroutines.asm subroutines.hex

skips.bin: skips.hex
	$(BIN) skips.hex skips.bin
skips.hex: skips.asm
	$(ASM) skips.asm skips.hex

timers.bin: timers.hex
	$(BIN) timers.hex timers.bin
timers.hex: timers.asm
	$(ASM) timers.asm timers.hex

keypad.bin: keypad.hex
	$(BIN) keypad.hex keypad.bin
keypad.hex: keypad.asm
	$(ASM) keypad.asm keypad.hex

graphics.bin: graphics.hex
	$(BIN) graphics.hex graphics.bin
graphics.hex: graphics.asm
	$(ASM) graphics.asm graphics.hex

storage.bin: storage.hex
	$(BIN) storage.hex storage.bin
storage.hex: storage.asm
	$(ASM) storage.asm storage.hex

helloworld.bin: helloworld.hex
	$(BIN) helloworld.hex helloworld.bin
helloworld.hex: helloworld.asm
	$(ASM) helloworld.asm helloworld.hex
helloworld.asm: helloworld.c8
	$(CMP) helloworld.c8 helloworld.asm

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
