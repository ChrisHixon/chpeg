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
CPPFLAGS=-Wall -std=c++17 -pedantic $(EXTENSION_DEFS) $(FEATURE_DEFS)
INCLUDES=-I$(INCLUDE)
LDFLAGS=
INCLUDE=../include
BUILD=build

# chpeg lib vars
BYTECODE_C=$(if $(EXTENSIONS),chpeg_ext_bytecode.c,chpeg_bytecode.c)
CHPEG_SOURCE_FILES=mem.c opcodes.c bytecode.c util.c parser.c compiler.c $(BYTECODE_C)
CHPEG_SRC=../src
CHPEG_BUILD=../build
CHPEG_OBJ = $(CHPEG_SOURCE_FILES:%.c=$(BUILD)/chpeg/%.o)
CHPEG_DEP = $(CHPEG_OBJ:%.o=%.d) $(BUILD)/chpeg.d

# test vars
TESTS_EXT=$(if $(EXTENSIONS),1,0)
TESTS_EXT_DEF=-DUSE_EXTENSIONS=$(TESTS_EXT)
TEST_CFLAGS=$(CFLAGS) $(TESTS_EXT_DEF)
TEST_CPPFLAGS=$(CPPFLAGS) $(TESTS_EXT_DEF)
TESTS_OBJDIR=$(BUILD)
GENERATED=generated
TESTS=bytecode_output_c_pt1_output \
      bytecode_output_c_pt2_compare \
      compare_bytecode_with_internal \
      line_col
TESTS_SOURCES=$(patsubst %,%.c,$(TESTS))
TESTS_OBJECTS=$(patsubst %.c,$(TESTS_OBJDIR)/%.o,$(TESTS_SOURCES))
TESTS_DEP = $(TESTS_OBJECTS:%.o=%.d)
AMALGAMATIONS=parse_file_amalg_c parse_file_amalg_o parse_file_amalg_all parse_file_cpp_amalg_all parse_file_cpp_amalg_o

TEST_BYTECODE=$(GENERATED)/test_bytecode.c $(GENERATED)/test_bytecode.h

#
# rules
#

all: $(TESTS) $(AMALGAMATIONS)

#
# chpeg lib / object files
#

# separate object files for each source
$(BUILD)/chpeg/%.o: $(CHPEG_SRC)/%.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

# one object file from amalgamation .c file
$(BUILD)/chpeg.o: $(CHPEG_BUILD)/chpeg.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

-include $(CHPEG_DEP)

#
# tests
#

$(TESTS_OBJDIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(TEST_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

-include $(TESTS_DEP)

# binaries

line_col: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJ) -o $@

compare_bytecode_with_internal: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJ) -o $@

bytecode_output_c_pt1_output: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(CHPEG_OBJ) -o $@

bytecode_output_c_pt2_compare: %: $(TESTS_OBJDIR)/%.o $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $< $(GENERATED)/test_bytecode.o $(CHPEG_OBJ) -o $@

# various amalgamation / linking tests
# (currently these are just tests to see if they build, not tested in `make test`)

# .c amalgamation, include .h, .c file added directly
parse_file_amalg_c: ../build/chpeg.c ../build/chpeg.h
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) -DAMALG $< parse_file.c -o $@

# .c amalgamation, include .h, link with .o file compiled separately
parse_file_amalg_o: $(BUILD)/chpeg.o
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) -DAMALG $< parse_file.c -o $@

# all-in-one amalgamation, .h included in source code
parse_file_amalg_all: ../build/chpeg_all.h
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) -Wno-unused-function \
		-DAMALG -DAMALG_ONE_FILE parse_file.c -o $@

# CPP: all-in-one amalgamation, .h included in source code
parse_file_cpp_amalg_all: ../build/chpeg_all.h
	$(CXX) $(TEST_CPPFLAGS) $(LDFLAGS) -Wno-unused-function \
		-DAMALG -DAMALG_ONE_FILE parse_file.cpp -o $@

# CPP: include .h amalgamation, link with chpeg.o
parse_file_cpp_amalg_o: $(BUILD)/chpeg.o
	$(CXX) $(TEST_CPPFLAGS) $(LDFLAGS) -Wno-unused-function \
		-DAMALG $< parse_file.cpp -o $@

# generated

$(TEST_BYTECODE): bytecode_output_c_pt1_output | $(GENERATED)
	-./bytecode_output_c_pt1_output

$(GENERATED)/test_bytecode.o: $(TEST_BYTECODE) | $(GENERATED)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(TESTS_OBJDIR)/bytecode_output_c_pt2_compare.o: $(GENERATED)/test_bytecode.o


# directories

$(TESTS_OBJDIR):
	mkdir -p $(TESTS_OBJDIR)

$(GENERATED):
	mkdir -p $(GENERATED)

.PHONY: test
test:
	-./run_tests.sh

#.PHONY: clean
clean:
	-rm -rf $(BUILD)
	-rm -rf $(GENERATED)
	-rm -f $(TESTS) $(AMALGAMATIONS)

