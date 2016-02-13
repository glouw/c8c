CC = gcc
CFLAGS = -std=gnu11
OFLAGS = -Og -g -flto
WFLAGS = -Wshadow -Wall -Wextra -pedantic

OBJS = main.o tree.o assemble.o
PROJ = asm

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c tree.h assemble.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c main.c

tree.o: tree.[ch] Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c tree.c

assemble.o: assemble.[ch] tree.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c assemble.c

test: $(PROJ)
	#all
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./asm tests/all/main.asm tests/all/main.hex
	valgrind --tool=exp-sgcheck ./asm tests/all/main.asm tests/all/main.hex
	rm -f callgrind*
		valgrind --tool=callgrind ./asm tests/all/main.asm tests/all/main.hex
		callgrind_annotate callgrind* main.c     > tests/all/profile/main.c
		callgrind_annotate callgrind* tree.c     > tests/all/profile/tree.c
		callgrind_annotate callgrind* assemble.c > tests/all/profile/assemble.c
		callgrind_annotate callgrind*            > tests/all/profile/overall.performance
	rm -f callgrind*
	./asm tests/all/main.asm tests/all/main.hex; xxd -r -plain tests/all/main.hex tests/all/main.bin;
	#labels
	./asm tests/labels/call.asm tests/labels/call.hex; xxd -r -plain tests/labels/call.hex tests/labels/call.bin;
	./asm tests/labels/cls.asm  tests/labels/cls.hex;  xxd -r -plain tests/labels/cls.hex  tests/labels/cls.bin;
	./asm tests/labels/jp.asm   tests/labels/jp.hex;   xxd -r -plain tests/labels/jp.hex   tests/labels/jp.bin;
	./asm tests/labels/ret.asm  tests/labels/ret.hex;  xxd -r -plain tests/labels/ret.hex  tests/labels/ret.bin;

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
	#all
	rm -f tests/all/main.hex
	rm -f tests/all/main.bin
	rm -f tests/all/profile/*.c
	rm -f tests/all/profile/overall.performance
	#labels
	rm -f tests/labels/call.hex
	rm -f tests/labels/call.bin
	rm -f tests/labels/cls.hex
	rm -f tests/labels/cls.bin
	rm -f tests/labels/jp.hex
	rm -f tests/labels/jp.bin
	rm -f tests/labels/ret.hex
	rm -f tests/labels/ret.bin
