all:
	make -C emu
	make -C asm
	make -C c8c
	make -C test/asm
	make -C test/c8c

clean:
	make clean -C emu
	make clean -C asm
	make clean -C c8c
	make clean -C test/asm
	make clean -C test/c8c
