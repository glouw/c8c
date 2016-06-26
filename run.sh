# run.sh
#   Compiles assembler and emulator
#   Assembles and runs a chip8 program on emulator

function setup {
    clear
    setfont ter-v12n
}

function clean {
    printf "Press any key to continue\n"
    read
    clear
    setfont ter-v16n
}

setup
trap clean EXIT

# Compile assembler and emulator
make -s -C asm && # Compile assembler
make -s -C emu    # Compile emulator
if [ $? -eq 0 ];
    then
    printf "Build success\n"
else
    printf "Build failed\n"
    exit
fi

# Assemble chip8 binary
valgrind asm/asm $1.asm $1.hex && # Assemble hex file
xxd -r -plain $1.hex $1.bin       # Morph to bin file
if [ $? -eq 0 ];
    then
    printf "Binary creation success\n"
else
    printf "Binary creation failed\n"
    exit
fi

# Check binary size before continuing
bytes=$(wc -c < $1.bin); limit=4096
printf "%s / %s bytes\n" "$bytes" "$limit"
if [ "$bytes" -gt "$limit" ];
    then
    printf "Binary is too large to fit in memory\n"
    exit
else
    printf "Press any key to continue\n"
    read
fi

# Run binary on interpreter
valgrind emu/emu $1.bin
