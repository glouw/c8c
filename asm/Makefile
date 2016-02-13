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

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
	rm -f profile/*.profile
	rm -f profile/overall.performance
	rm -f gmon.out

debug: $(PROJ)
	# memory
	valgrind -q --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(PROJ) MAIN.asm MAIN.hex
	# range check
	valgrind -q --tool=exp-sgcheck ./$(PROJ) MAIN.asm MAIN.hex
	# function cycle check
	rm -f callgrind*
	valgrind -q --tool=callgrind ./$(PROJ) MAIN.asm MAIN.hex
	callgrind_annotate callgrind* main.c > profile/main.c
	callgrind_annotate callgrind* tree.c > profile/tree.c
	callgrind_annotate callgrind* assemble.c > profile/assemble.c
	callgrind_annotate callgrind* > profile/overall.performance
	rm -f callgrind*
	cat MAIN.hex
	cat profile/overall.performance | grep "PROGRAM TOTALS"
