CC = gcc
PROJ = asm
CPPFLAGS = -Iinclude
SRCS = files.c main.c opcodes.c generator.c tree.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Wshadow -std=gnu11 -pedantic -Og -g
LDFLAGS = -lm
TARGET_ARCH = -march=native
DEPDIR := deps
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK = $(CC) $(LDFLAGS) $(OBJS) $(TARGET_ARCH) -o
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(PROJ): $(OBJS)
	$(LINK) $(PROJ)

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE) $(OUTPUT_OPTION) $<
	@$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

clean:
	rm -f $(PROJ)
	rm -f $(OBJS)
