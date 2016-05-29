#
# build.sh
#   Makes the chip8 assembler and emulator
#

# Compile assembler and emulator
make -s -C asm &&
make -s -C emu

if [ $? -eq 0 ]; then
    echo "Build success!"
fi
