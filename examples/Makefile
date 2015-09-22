SHELL=/bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

CC=gcc
CFLAGS=-Wall -std=c99
INCLUDES=-I$(CHPEG_SRC)
LDFLAGS=

# chpeg pseudo-lib
CHPEG_OBJDIR=chpeg
CHPEG_SRC=../src
CHPEG_SOURCES=$(CHPEG_SRC)/peg_cbyte_parser.c $(CHPEG_SRC)/peg_cbyte_compiler.c
CHPEG_OBJECTS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_OBJDIR)/%.o,$(CHPEG_SOURCES))
CHPEG_HEADERS=$(CHPEG_SRC)/peg_cbyte_parser.h $(CHPEG_SRC)/peg_cbyte_compiler.h

# examples
EXAMPLES_OBJDIR=obj
EXAMPLES=peg_parse_file
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
	$(CC) -c $(CFLAGS) $< -o $@

$(CHPEG_OBJDIR):
	mkdir $(CHPEG_OBJDIR)

.PHONY: clean
clean:
	-rm -rf $(EXAMPLES_OBJDIR)
	-rm -rf $(CHPEG_OBJDIR)
	-rm $(EXAMPLES)
