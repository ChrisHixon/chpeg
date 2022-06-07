SHELL=/bin/sh

# list grammar extensions here, or use ALL to include all
# current extensions: TRIM REFS
EXTENSIONS = ALL

# list optional features here
# current features: PACKRAT
FEATURES = PACKRAT

CC=gcc
EXTENSION_DEFS = $(EXTENSIONS:%=-DCHPEG_EXTENSION_%)
FEATURE_DEFS = $(FEATURES:%=-DCHPEG_%)
CFLAGS=-Wall -std=c99 -pedantic $(EXTENSION_DEFS) $(FEATURE_DEFS)
INCLUDES=-I$(INCLUDE)
LDFLAGS=

INCLUDE=../include
BUILD=build

BYTECODE_C=$(if $(EXTENSIONS), chpeg_ext_bytecode.c, chpeg_bytecode.c)

# chpeg lib
CHPEG_SOURCE_FILES=mem.c opcodes.c bytecode.c util.c parser.c compiler.c $(BYTECODE_C)
CHPEG_SRC=../src
CHPEG_OBJ = $(CHPEG_SOURCE_FILES:%.c=$(BUILD)/chpeg/%.o)
CHPEG_DEP = $(CHPEG_OBJ:%.o=%.d)
-include $(CHPEG_DEP)

$(BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

# dir for generated files
GENERATED=generated

# tests
TESTS_OBJDIR=$(BUILD)
TESTS=bytecode_output_c_pt1_output \
      bytecode_output_c_pt2_compare \
      compare_bytecode_with_internal
TESTS_SOURCES=$(patsubst %,%.c,$(TESTS))
TESTS_OBJECTS=$(patsubst %.c,$(TESTS_OBJDIR)/%.o,$(TESTS_SOURCES))

TEST_BYTECODE=$(GENERATED)/test_bytecode.c $(GENERATED)/test_bytecode.h

# rules
all: $(TESTS)

$(TESTS_OBJECTS): $(CHPEG_OBJ)

compare_bytecode_with_internal: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJ) -o $@

$(TEST_BYTECODE): bytecode_output_c_pt1_output | $(GENERATED)
	-./bytecode_output_c_pt1_output

bytecode_output_c_pt1_output: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJ) -o $@

$(GENERATED)/test_bytecode.o: $(TEST_BYTECODE) | $(GENERATED)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(TESTS_OBJDIR)/bytecode_output_c_pt2_compare.o: $(GENERATED)/test_bytecode.o

bytecode_output_c_pt2_compare: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(GENERATED)/test_bytecode.o $(CHPEG_OBJ) -o $@

$(TESTS_OBJDIR)/%.o: %.c | $(TESTS_OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# directories

$(TESTS_OBJDIR):
	mkdir -p $(TESTS_OBJDIR)

$(GENERATED):
	mkdir $(GENERATED)

.PHONY: test
test:
	-./run_tests.sh

#.PHONY: clean
clean:
	-rm -rf $(BUILD)
	-rm -rf $(GENERATED)
	-rm -f $(TESTS)

