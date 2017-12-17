ASM = ../asm
BIN = xxd

BINS = registers.bin flow.bin subroutines.bin skips.bin
BINS+= timers.bin keypad.bin graphics.bin storage.bin
HEXS = $(BINS:.bin=.hex)

all: $(BINS)

registers.bin: registers.hex
	$(BIN) -r -p registers.hex > registers.bin
registers.hex: registers.asm
	$(ASM) registers.asm registers.hex

flow.bin: flow.hex
	$(BIN) -r -p flow.hex > flow.bin
flow.hex: flow.asm
	$(ASM) flow.asm flow.hex

subroutines.bin: subroutines.hex
	$(BIN) -r -p subroutines.hex > subroutines.bin
subroutines.hex: subroutines.asm
	$(ASM) subroutines.asm subroutines.hex

skips.bin: skips.hex
	$(BIN) -r -p skips.hex > skips.bin
skips.hex: skips.asm
	$(ASM) skips.asm skips.hex

timers.bin: timers.hex
	$(BIN) -r -p timers.hex > timers.bin
timers.hex: timers.asm
	$(ASM) timers.asm timers.hex

keypad.bin: keypad.hex
	$(BIN) -r -p keypad.hex > keypad.bin
keypad.hex: keypad.asm
	$(ASM) keypad.asm keypad.hex

graphics.bin: graphics.hex
	$(BIN) -r -p graphics.hex > graphics.bin
graphics.hex: graphics.asm
	$(ASM) graphics.asm graphics.hex

storage.bin: storage.hex
	$(BIN) -r -p storage.hex > storage.bin
storage.hex: storage.asm
	$(ASM) storage.asm storage.hex

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
