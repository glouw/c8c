all:
	make -C asm
	make -C emu
	make -C c8c
	make -C test

clean:
	make clean -C asm
	make clean -C emu
	make clean -C c8c
	make clean -C test
