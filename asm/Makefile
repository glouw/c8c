CC = gcc
CFLAGS = -std=gnu11
OFLAGS = -Og -g -flto
WFLAGS = -Wshadow -Wall -Wextra -pedantic

OBJS = main.o
PROJ = asm

$(PROJ) : $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) $(WFLAGS) -c main.c

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
