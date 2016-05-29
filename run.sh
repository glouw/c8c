#
# run.sh
#   Assembles and runs a chip8 program
#

asm/asm $1.asm $1.hex && # Generates hex from asm
xxd -r -plain $1.hex $1.bin # Generates bin from hex

if [ $? -eq 0 ]; then
    clear;
    setfont ter-v12n # Sets font
    emu/emu $1.bin   # Executes bin
    clear;
    setfont ter-v16n # Revert font
fi
