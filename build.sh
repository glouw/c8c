echo "Compiling asm..."
make -s -C asm
echo "Compiling emu..."
make -s -C emu

#echo "...Regression testing and assembling test scripts"
#valgrind -q ./asm/asm tests/full.asm  tests/full.hex;  xxd -r -plain tests/full.hex  tests/full.bin;
#valgrind -q ./asm/asm tests/call.asm  tests/call.hex;  xxd -r -plain tests/call.hex  tests/call.bin;
#valgrind -q ./asm/asm tests/cls.asm   tests/cls.hex;   xxd -r -plain tests/cls.hex   tests/cls.bin;
#valgrind -q ./asm/asm tests/jp.asm    tests/jp.hex;    xxd -r -plain tests/jp.hex    tests/jp.bin;
#valgrind -q ./asm/asm tests/ret.asm   tests/ret.hex;   xxd -r -plain tests/ret.hex   tests/ret.bin;
#valgrind -q ./asm/asm tests/se.asm    tests/se.hex;    xxd -r -plain tests/se.hex    tests/se.bin;
#valgrind -q ./asm/asm tests/sne.asm   tests/sne.hex;   xxd -r -plain tests/sne.hex   tests/sne.bin;

echo "Assembling test scripts..."
./asm/asm tests/full.asm  tests/full.hex;  xxd -r -plain tests/full.hex  tests/full.bin;
./asm/asm tests/call.asm  tests/call.hex;  xxd -r -plain tests/call.hex  tests/call.bin;
./asm/asm tests/cls.asm   tests/cls.hex;   xxd -r -plain tests/cls.hex   tests/cls.bin;
./asm/asm tests/jp.asm    tests/jp.hex;    xxd -r -plain tests/jp.hex    tests/jp.bin;
./asm/asm tests/ret.asm   tests/ret.hex;   xxd -r -plain tests/ret.hex   tests/ret.bin;
./asm/asm tests/se.asm    tests/se.hex;    xxd -r -plain tests/se.hex    tests/se.bin;
./asm/asm tests/sne.asm   tests/sne.hex;   xxd -r -plain tests/sne.hex   tests/sne.bin;

echo "DONE"
