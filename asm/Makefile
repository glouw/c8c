PROJ = asm
# Source files
SRCS = main.c files.c generator.c opcodes.c tree.c flags.c string.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
# Compiler
CC = clang -std=c99
# Warnings flags
CFLAGS = -Wshadow -Wall -Wpedantic -Wextra
# Debugging flags
CFLAGS+= -g
# Optimization flags
CFLAGS+= -Ofast -flto
ARCHITECTURE = -march=native
# Linking flags
LDFLAGS = -lm
# Dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $*.Td
# Compile, link, and post process
COMP = $(CC) $(ARCHITECTURE) $(CFLAGS) $(DEPFLAGS) -c
LINK = $(CC) $(ARCHITECTURE) $(CFLAGS) $(OBJS) $(LDFLAGS) -o
POST = mv -f $*.Td $*.d
$(PROJ): $(OBJS)
	$(LINK) $(PROJ)
%.o : %.c
%.o : %.c %.d
	$(COMP) $(OUTPUT_OPTION) $<
	$(POST)
%.d: ;

.PRECIOUS: %.d
-include $(patsubst %,%.d,$(basename $(SRCS)))

.PHONY: clean
clean:
	rm -f cachegrind.out.*
	rm -f vgcore.*
	rm -f $(DEPS)
	rm -f $(PROJ)
	rm -f $(OBJS)
