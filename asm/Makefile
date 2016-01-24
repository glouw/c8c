CC = gcc
CFLAGS = -std=gnu11 -std=gnu11
OFLAGS = -Og -g
WFLAGS = -Wshadow -Wall -Wextra -pedantic

OBJS = main.o node.o opcode.o error.o
PROJ = asm

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c public.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c main.c

node.o: node.c public.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c node.c

opcode.o: opcode.c public.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c opcode.c

error.o: error.c public.h Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c error.c

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)

debug: $(PROJ)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROJ) file.hex;
	cat file.hex;
