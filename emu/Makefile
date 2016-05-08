CC = gcc -std=c11
CFLAGS = -Og -g -flto -Wshadow -Wall -Wextra -pedantic
 
OBJS = main.o
PROJ = emu

$(PROJ): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROJ)

main.o: main.c Makefile
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
