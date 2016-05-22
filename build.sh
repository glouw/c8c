make -s -C asm &&
make -s -C emu &&

# Assemble all .asm test scripts
./asm/asm tests/full.asm tests/full.hex && xxd -r -plain tests/full.hex tests/full.bin &&
./asm/asm tests/call.asm tests/call.hex && xxd -r -plain tests/call.hex tests/call.bin &&
./asm/asm tests/cls.asm  tests/cls.hex  && xxd -r -plain tests/cls.hex  tests/cls.bin  &&
./asm/asm tests/jp.asm   tests/jp.hex   && xxd -r -plain tests/jp.hex   tests/jp.bin   &&
./asm/asm tests/ret.asm  tests/ret.hex  && xxd -r -plain tests/ret.hex  tests/ret.bin  &&
./asm/asm tests/se.asm   tests/se.hex   && xxd -r -plain tests/se.hex   tests/se.bin   &&
./asm/asm tests/sne.asm  tests/sne.hex  && xxd -r -plain tests/sne.hex  tests/sne.bin  &&
./asm/asm tests/add.asm  tests/add.hex  && xxd -r -plain tests/add.hex  tests/add.bin  &&
./asm/asm tests/ld.asm   tests/ld.hex   && xxd -r -plain tests/ld.hex   tests/ld.bin   &&
./asm/asm tests/or.asm   tests/or.hex   && xxd -r -plain tests/or.hex   tests/or.bin   &&
./asm/asm tests/and.asm  tests/and.hex  && xxd -r -plain tests/and.hex  tests/and.bin  &&
./asm/asm tests/xor.asm  tests/xor.hex  && xxd -r -plain tests/xor.hex  tests/xor.bin  &&
./asm/asm tests/sub.asm  tests/sub.hex  && xxd -r -plain tests/sub.hex  tests/sub.bin  &&
./asm/asm tests/shr.asm  tests/shr.hex  && xxd -r -plain tests/shr.hex  tests/shr.bin  &&
./asm/asm tests/shl.asm  tests/shl.hex  && xxd -r -plain tests/shl.hex  tests/shl.bin  &&
./asm/asm tests/rnd.asm  tests/rnd.hex  && xxd -r -plain tests/rnd.hex  tests/rnd.bin  &&
./asm/asm tests/drw.asm  tests/drw.hex  && xxd -r -plain tests/drw.hex  tests/drw.bin  &&
./asm/asm tests/skp.asm  tests/skp.hex  && xxd -r -plain tests/skp.hex  tests/skp.bin  &&
./asm/asm tests/sknp.asm tests/sknp.hex && xxd -r -plain tests/sknp.hex tests/sknp.bin

# Execute assembled binaries if all .asm test scripts assembled correctly
if [ $? -eq 0 ]; then
    valgrind ./emu/emu tests/drw.bin
fi
