CC = gcc
CFLAGS = -std=gnu11
OFLAGS = -Og -g -flto
WFLAGS = -Wshadow -Wall -Wextra -pedantic

OBJS = main.o tree.o opcode.o
PROJ = asm

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c tree.h opcode.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c main.c

tree.o: tree.[ch] Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c tree.c

opcode.o: opcode.[ch] tree.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c opcode.c

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
