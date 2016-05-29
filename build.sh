make -s -C asm &&
make -s -C emu &&

asm/asm tests/full.asm        tests/full.hex        && xxd -r -plain tests/full.hex        tests/full.bin        &&
asm/asm tests/call.asm        tests/call.hex        && xxd -r -plain tests/call.hex        tests/call.bin        &&
asm/asm tests/cls.asm         tests/cls.hex         && xxd -r -plain tests/cls.hex         tests/cls.bin         &&
asm/asm tests/jp.asm          tests/jp.hex          && xxd -r -plain tests/jp.hex          tests/jp.bin          &&
asm/asm tests/ret.asm         tests/ret.hex         && xxd -r -plain tests/ret.hex         tests/ret.bin         &&
asm/asm tests/se.asm          tests/se.hex          && xxd -r -plain tests/se.hex          tests/se.bin          &&
asm/asm tests/sne.asm         tests/sne.hex         && xxd -r -plain tests/sne.hex         tests/sne.bin         &&
asm/asm tests/add.asm         tests/add.hex         && xxd -r -plain tests/add.hex         tests/add.bin         &&
asm/asm tests/ld.asm          tests/ld.hex          && xxd -r -plain tests/ld.hex          tests/ld.bin          &&
asm/asm tests/or.asm          tests/or.hex          && xxd -r -plain tests/or.hex          tests/or.bin          &&
asm/asm tests/and.asm         tests/and.hex         && xxd -r -plain tests/and.hex         tests/and.bin         &&
asm/asm tests/xor.asm         tests/xor.hex         && xxd -r -plain tests/xor.hex         tests/xor.bin         &&
asm/asm tests/sub.asm         tests/sub.hex         && xxd -r -plain tests/sub.hex         tests/sub.bin         &&
asm/asm tests/shr.asm         tests/shr.hex         && xxd -r -plain tests/shr.hex         tests/shr.bin         &&
asm/asm tests/shl.asm         tests/shl.hex         && xxd -r -plain tests/shl.hex         tests/shl.bin         &&
asm/asm tests/rnd.asm         tests/rnd.hex         && xxd -r -plain tests/rnd.hex         tests/rnd.bin         &&
asm/asm tests/drw.asm         tests/drw.hex         && xxd -r -plain tests/drw.hex         tests/drw.bin         &&
asm/asm tests/skp.asm         tests/skp.hex         && xxd -r -plain tests/skp.hex         tests/skp.bin         &&
asm/asm tests/sknp.asm        tests/sknp.hex        && xxd -r -plain tests/sknp.hex        tests/sknp.bin        &&
asm/asm tests/bitwise.asm     tests/bitwise.hex     && xxd -r -plain tests/bitwise.hex     tests/bitwise.bin     &&
asm/asm tests/bitshift.asm    tests/bitshift.hex    && xxd -r -plain tests/bitshift.hex    tests/bitshift.bin    &&
asm/asm tests/subroutines.asm tests/subroutines.hex && xxd -r -plain tests/subroutines.hex tests/subroutines.bin &&
asm/asm tests/branching.asm   tests/branching.hex   && xxd -r -plain tests/branching.hex   tests/branching.bin   &&
asm/asm tests/timers.asm      tests/timers.hex      && xxd -r -plain tests/timers.hex      tests/timers.bin      &&
asm/asm tests/keypad.asm      tests/keypad.hex      && xxd -r -plain tests/keypad.hex      tests/keypad.bin      &&
asm/asm tests/graphics.asm    tests/graphics.hex    && xxd -r -plain tests/graphics.hex    tests/graphics.bin    &&
asm/asm tests/storage.asm     tests/storage.hex     && xxd -r -plain tests/storage.hex     tests/storage.bin     &&
asm/asm tests/fonts.asm       tests/fonts.hex       && xxd -r -plain tests/fonts.hex       tests/fonts.bin

if [ $? -eq 0 ]; then
    echo "Build success!"
fi
