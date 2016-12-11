CC = gcc
PROJ = c8c
CPPFLAGS = -Iinclude
SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Wshadow -std=gnu11 -pedantic -Og -g
LDFLAGS = -lm -lncurses
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
