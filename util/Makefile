SHELL=/bin/sh

# List grammar extensions to include here; use ALL to include all; or leave blank for none
# extensions: TRIM REFS ESCAPES HEX
# WARNING: currently, the only supported option is ALL (or none)
EXTENSIONS = ALL

# list optional features here
# current features: PACKRAT
FEATURES = PACKRAT DEBUG_COMPILER

# set to non-empty value to use the chpeg.c amalgamation instead of separate chpeg library sources
USE_AMALGAMATION =

CC=gcc

EXTENSION_DEFS = $(EXTENSIONS:%=-DCHPEG_EXTENSION_%)
FEATURE_DEFS = $(FEATURES:%=-DCHPEG_%)

CFLAGS_COMMON  = -Wall -std=c99 -pedantic $(EXTENSION_DEFS) $(FEATURE_DEFS)
LDFLAGS=

INCLUDES=-I$(INCLUDE) -I./src
INCLUDE=../include

BYTECODE_C=$(if $(EXTENSIONS),chpeg_ext_bytecode.c,chpeg_bytecode.c)

CHPEG_LIB_SOURCE_FILES=mem.c opcodes.c bytecode.c util.c parser.c compiler.c $(BYTECODE_C)
CHPEG_LIB_SRC_DIR=../src

CHPEG_AMALGAMATION_SOURCE=chpeg.c
CHPEG_AMALGAMATION_SRC_DIR=../build

CHPEG_SOURCE_FILES=$(if $(USE_AMALGAMATION),$(CHPEG_AMALGAMATION_SOURCE),$(CHPEG_LIB_SOURCE_FILES))
CHPEG_SRC=$(if $(USE_AMALGAMATION),$(CHPEG_AMALGAMATION_SRC_DIR),$(CHPEG_LIB_SRC_DIR))

UTIL_SRC = $(wildcard src/*.c) $(wildcard src/actions/*.c)

BUILD = build

STD_BIN         = chpeg
STD_CFLAGS      = $(CFLAGS_COMMON) -O3
STD_BUILD       = $(BUILD)/std
STD_OBJ         = $(UTIL_SRC:%.c=$(STD_BUILD)/%.o)
STD_DEP         = $(STD_OBJ:%.o=%.d)
STD_CHPEG_OBJ   = $(CHPEG_SOURCE_FILES:%.c=$(STD_BUILD)/chpeg/%.o)
STD_CHPEG_DEP   = $(STD_CHPEG_OBJ:%.o=%.d)

PROFILE_BIN         = chpeg-profile
PROFILE_CFLAGS      = $(CFLAGS_COMMON) -O3 -DCHPEG_VM_PROFILE
PROFILE_BUILD       = $(BUILD)/profile
PROFILE_OBJ         = $(UTIL_SRC:%.c=$(PROFILE_BUILD)/%.o)
PROFILE_DEP         = $(PROFILE_OBJ:%.o=%.d)
PROFILE_CHPEG_OBJ   = $(CHPEG_SOURCE_FILES:%.c=$(PROFILE_BUILD)/chpeg/%.o)
PROFILE_CHPEG_DEP   = $(PROFILE_CHPEG_OBJ:%.o=%.d)

TRACE_BIN       = chpeg-trace
TRACE_CFLAGS    = $(CFLAGS_COMMON)  -g -DCHPEG_VM_PROFILE -DCHPEG_VM_TRACE -DCHPEG_VM_PRINT_TREE
TRACE_BUILD     = $(BUILD)/trace
TRACE_OBJ       = $(UTIL_SRC:%.c=$(TRACE_BUILD)/%.o)
TRACE_DEP       = $(TRACE_OBJ:%.o=%.d)
TRACE_CHPEG_OBJ = $(CHPEG_SOURCE_FILES:%.c=$(TRACE_BUILD)/chpeg/%.o)
TRACE_CHPEG_DEP = $(TRACE_CHPEG_OBJ:%.o=%.d)

all: $(STD_BIN) $(PROFILE_BIN) $(TRACE_BIN)

$(STD_BIN): $(STD_OBJ) $(STD_CHPEG_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROFILE_BIN): $(PROFILE_OBJ) $(PROFILE_CHPEG_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(TRACE_BIN): $(TRACE_OBJ) $(TRACE_CHPEG_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

-include $(STD_DEP)
-include $(STD_CHPEG_DEP)

-include $(PROFILE_DEP)
-include $(PROFILE_CHPEG_DEP)

-include $(TRACE_DEP)
-include $(TRACE_CHPEG_DEP)

$(STD_BUILD)/%.o : %.c | $(STD_BUILD)/src/actions
	$(CC) $(STD_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(STD_BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c | $(STD_BUILD)/chpeg
	$(CC) $(STD_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(PROFILE_BUILD)/%.o : %.c | $(PROFILE_BUILD)/src/actions
	$(CC) $(PROFILE_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(PROFILE_BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c | $(PROFILE_BUILD)/chpeg
	$(CC) $(PROFILE_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(TRACE_BUILD)/%.o : %.c | $(TRACE_BUILD)/src/actions
	$(CC) $(TRACE_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(TRACE_BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c | $(TRACE_BUILD)/chpeg
	$(CC) $(TRACE_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

# directories

$(STD_BUILD)/src/actions:
	-mkdir -p $(STD_BUILD)/src/actions

$(STD_BUILD)/chpeg:
	-mkdir -p $(STD_BUILD)/chpeg

$(PROFILE_BUILD)/src/actions:
	-mkdir -p $(PROFILE_BUILD)/src/actions

$(PROFILE_BUILD)/chpeg:
	-mkdir -p $(PROFILE_BUILD)/chpeg

$(TRACE_BUILD)/src/actions:
	-mkdir -p $(TRACE_BUILD)/src/actions

$(TRACE_BUILD)/chpeg:
	-mkdir -p $(TRACE_BUILD)/chpeg

.PHONY: clean
clean:
	-rm -f $(STD_BIN) $(TRACE_BIN) $(PROFILE_BIN)
	-rm -rf $(BUILD)

# vim: ts=4
