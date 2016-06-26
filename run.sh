# run.sh
#   Compiles assembler and emulator
#   Assembles and runs a chip8 program on emulator

# Compile assembler and emulator
make -s -C asm && # Assembler
make -s -C emu    # Emulator
if [ $? -eq 0 ]; then
    echo "Build success!"
fi

# Assembler chip8 Program
valgrind asm/asm $1.asm $1.hex && # Generate hex from asm
xxd -r -plain $1.hex $1.bin    && # Generate bin from hex
echo "Binary size (Bytes):"       # Print file size
wc -c < $1.bin
echo "Press any key to continue"
read                              # Wait for user unput
if [ $? -eq 0 ]; then
    clear; setfont ter-v12n # Set font
    valgrind emu/emu $1.bin # Execute bin
    echo "Press any key to continue"
    read                    # Wait for user input for valgrind output to show
    clear; setfont ter-v16n # Revert font
fi
