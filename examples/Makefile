SHELL=/bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

CC=gcc
CFLAGS=-Wall -std=c99 -O3
INCLUDES=-I$(CHPEG_SRC)
LDFLAGS=

# chpeg pseudo-lib
CHPEG_OBJDIR=chpeg
CHPEG_SRC=../src

CHPEG_SOURCE_FILES=parser.c compiler.c bytecode.c chpeg_bytecode.c opcodes.c util.c mem.c
CHPEG_SOURCES=$(patsubst %.c,$(CHPEG_SRC)/%.c,$(CHPEG_SOURCE_FILES))
CHPEG_OBJECTS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_OBJDIR)/%.o,$(CHPEG_SOURCES))
CHPEG_HEADERS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_SRC)/%.h,$(CHPEG_SOURCES)) $(CHPEG_SRC)/mem.h

# examples
EXAMPLES_OBJDIR=obj
EXAMPLES=parse_file
EXAMPLES_SOURCES=$(patsubst %,%.c,$(EXAMPLES))
EXAMPLES_OBJECTS=$(patsubst %.c,$(EXAMPLES_OBJDIR)/%.o,$(EXAMPLES_SOURCES))

# rules
all: $(EXAMPLES)

$(EXAMPLES): $(EXAMPLES_OBJECTS)
$(EXAMPLES_OBJECTS): $(CHPEG_OBJECTS)

$(EXAMPLES): %: $(EXAMPLES_OBJDIR)/%.o $(CHPEG_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(EXAMPLES_OBJDIR)/%.o: %.c | $(EXAMPLES_OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(EXAMPLES_OBJDIR):
	mkdir $(EXAMPLES_OBJDIR)

$(CHPEG_OBJECTS): $(CHPEG_HEADERS)

$(CHPEG_OBJDIR)/%.o: $(CHPEG_SRC)/%.c | $(CHPEG_OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CHPEG_OBJDIR):
	mkdir $(CHPEG_OBJDIR)

.PHONY: clean
clean:
	-rm -rf $(EXAMPLES_OBJDIR)
	-rm -rf $(CHPEG_OBJDIR)
	-rm -f $(EXAMPLES)
