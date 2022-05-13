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

CHPEG_SOURCE_FILES=parser.c compiler.c bytecode.c chpeg_bytecode.c opcodes.c util.c
CHPEG_SOURCES=$(patsubst %.c,$(CHPEG_SRC)/%.c,$(CHPEG_SOURCE_FILES))
CHPEG_OBJECTS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_OBJDIR)/%.o,$(CHPEG_SOURCES))
CHPEG_HEADERS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_SRC)/%.h,$(CHPEG_SOURCES)) $(CHPEG_SRC)/mem.h

# dir for generated files
GENERATED=generated

# tests
TESTS_OBJDIR=obj
TESTS=bytecode_output_c_pt1_output bytecode_output_c_pt2_compare compare_bytecode_with_internal
TESTS_SOURCES=$(patsubst %,%.c,$(TESTS))
TESTS_OBJECTS=$(patsubst %.c,$(TESTS_OBJDIR)/%.o,$(TESTS_SOURCES))

TEST_BYTECODE=$(GENERATED)/test_bytecode.c $(GENERATED)/test_bytecode.h

# rules
all: $(TESTS)

$(TESTS_OBJECTS): $(CHPEG_OBJECTS)

compare_bytecode_with_internal: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJECTS)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJECTS) -o $@

$(TEST_BYTECODE): bytecode_output_c_pt1_output | $(GENERATED)
	-./bytecode_output_c_pt1_output

bytecode_output_c_pt1_output: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJECTS)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJECTS) -o $@

$(GENERATED)/test_bytecode.o: $(TEST_BYTECODE) | $(GENERATED)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(TESTS_OBJDIR)/bytecode_output_c_pt2_compare.o: $(GENERATED)/test_bytecode.o

bytecode_output_c_pt2_compare: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJECTS)
	$(CC) $(LDFLAGS) $< $(GENERATED)/test_bytecode.o $(CHPEG_OBJECTS) -o $@

$(TESTS_OBJDIR)/%.o: %.c | $(TESTS_OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@


# chpeg

$(CHPEG_OBJECTS): $(CHPEG_HEADERS)

$(CHPEG_OBJDIR)/%.o: $(CHPEG_SRC)/%.c | $(CHPEG_OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# directories

$(TESTS_OBJDIR):
	mkdir $(TESTS_OBJDIR)

$(CHPEG_OBJDIR):
	mkdir $(CHPEG_OBJDIR)

$(GENERATED):
	mkdir $(GENERATED)

.PHONY: clean
clean:
	-rm -rf $(TESTS_OBJDIR)
	-rm -rf $(CHPEG_OBJDIR)
	-rm -rf $(GENERATED)
	-rm -f $(TESTS) $(TEST_BYTECODE)
