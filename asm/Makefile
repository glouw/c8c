CC = gcc
CFLAGS = -std=gnu11 -std=gnu11
OFLAGS = -Og -g
WFLAGS = -Wshadow -Wall -Wextra -pedantic

OBJS = main.o node.o opcode.o
PROJ = asm

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c main.c

node.o: node.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c node.c

opcode.o: opcode.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c opcode.c

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)

debug: $(PROJ)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROJ) file.hex;
	cat file.hex;
