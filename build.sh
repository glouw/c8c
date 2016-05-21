make -s -C asm
make -s -C emu

# Assemble all .asm test scripts
valgrind -q ./asm/asm tests/full.asm tests/full.hex  && xxd -r -plain tests/full.hex tests/full.bin  &&
valgrind -q ./asm/asm tests/call.asm tests/call.hex  && xxd -r -plain tests/call.hex tests/call.bin  &&
valgrind -q ./asm/asm tests/cls.asm  tests/cls.hex   && xxd -r -plain tests/cls.hex  tests/cls.bin   &&
valgrind -q ./asm/asm tests/jp.asm   tests/jp.hex    && xxd -r -plain tests/jp.hex   tests/jp.bin    &&
valgrind -q ./asm/asm tests/ret.asm  tests/ret.hex   && xxd -r -plain tests/ret.hex  tests/ret.bin   &&
valgrind -q ./asm/asm tests/se.asm   tests/se.hex    && xxd -r -plain tests/se.hex   tests/se.bin    &&
valgrind -q ./asm/asm tests/sne.asm  tests/sne.hex   && xxd -r -plain tests/sne.hex  tests/sne.bin   &&
valgrind -q ./asm/asm tests/add.asm  tests/add.hex   && xxd -r -plain tests/add.hex  tests/add.bin   &&
valgrind -q ./asm/asm tests/ld.asm   tests/ld.hex    && xxd -r -plain tests/ld.hex   tests/ld.bin

# Execute assembled binaries if all .asm test scripts assembled correctly
if [ $? -eq 0 ]; then
    ./emu/emu tests/ld.bin
fi
