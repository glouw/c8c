make clean -s -C asm
make clean -s -C emu

rm -f tests/*.bin
rm -f tests/*.hex

echo "Cleaned"
