ASM = ../../asm/asm
BIN = xxd -r -p

BINS = registers.bin flow.bin subroutines.bin skips.bin
BINS+= timers.bin keypad.bin graphics.bin storage.bin
HEXS = $(BINS:.bin=.hex)

all: $(BINS)

registers.bin: registers.hex
	$(BIN) $^ $@
registers.hex: registers.asm
	$(ASM) $^ $@

flow.bin: flow.hex
	$(BIN) $^ $@
flow.hex: flow.asm
	$(ASM) $^ $@

subroutines.bin: subroutines.hex
	$(BIN) $^ $@
subroutines.hex: subroutines.asm
	$(ASM) $^ $@

skips.bin: skips.hex
	$(BIN) $^ $@
skips.hex: skips.asm
	$(ASM) $^ $@

timers.bin: timers.hex
	$(BIN) $^ $@
timers.hex: timers.asm
	$(ASM) $^ $@

keypad.bin: keypad.hex
	$(BIN) $^ $@
keypad.hex: keypad.asm
	$(ASM) $^ $@

graphics.bin: graphics.hex
	$(BIN) $^ $@
graphics.hex: graphics.asm
	$(ASM) $^ $@

storage.bin: storage.hex
	$(BIN) $^ $@
storage.hex: storage.asm
	$(ASM) $^ $@

clean:
	rm -f $(BINS)
	rm -f $(HEXS)
