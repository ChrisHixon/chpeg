SHELL=/bin/sh

# list grammar extensions here, or use ALL to include all
# current extensions: TRIM REFS
EXTENSIONS = ALL

# list optional features here
# current features: PACKRAT
FEATURES = PACKRAT

# set to non-empty value to use the chpeg.c amalgamation instead of separate chpeg library sources
USE_AMALGAMATION =

CC=gcc
#CC=g++
EXTENSION_DEFS = $(EXTENSIONS:%=-DCHPEG_EXTENSION_%)
FEATURE_DEFS = $(FEATURES:%=-DCHPEG_%)
CFLAGS_COMMON = -Wall -std=c99 -pedantic $(EXTENSION_DEFS) $(FEATURE_DEFS)
#CFLAGS_COMMON = -Wall -std=c++11 -pedantic $(EXTENSION_DEFS) $(FEATURE_DEFS)
CFLAGS = $(CFLAGS_COMMON) -O3
CFLAGS_TRACE = $(CFLAGS_COMMON) -g -DCHPEG_VM_TRACE -DCHPEG_VM_PRINT_TREE -DCHPEG_VM_PROFILE
INCLUDES=-I$(INCLUDE) -I./src
LDFLAGS=

INCLUDE=../include

BYTECODE_C=$(if $(EXTENSIONS),chpeg_ext_bytecode.c,chpeg_bytecode.c)

CHPEG_LIB_SOURCE_FILES=mem.c opcodes.c bytecode.c util.c parser.c compiler.c $(BYTECODE_C)
CHPEG_LIB_SRC_DIR=../src

CHPEG_AMALGAMATION_SOURCE=chpeg.c
CHPEG_AMALGAMATION_SRC_DIR=../build

CHPEG_SOURCE_FILES=$(if $(USE_AMALGAMATION),$(CHPEG_AMALGAMATION_SOURCE),$(CHPEG_LIB_SOURCE_FILES))
CHPEG_SRC=$(if $(USE_AMALGAMATION),$(CHPEG_AMALGAMATION_SRC_DIR),$(CHPEG_LIB_SRC_DIR))

SRC = $(wildcard src/*.c) $(wildcard src/actions/*.c)

BIN = chpeg
BUILD = build
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(OBJ:%.o=%.d)
CHPEG_OBJ = $(CHPEG_SOURCE_FILES:%.c=$(BUILD)/chpeg/%.o)
CHPEG_DEP = $(CHPEG_OBJ:%.o=%.d)

BIN_TRACE = chpeg-trace
BUILD_TRACE = build_trace
OBJ_TRACE = $(SRC:%.c=$(BUILD_TRACE)/%.o)
DEP_TRACE = $(OBJ_TRACE:%.o=%.d)
CHPEG_TRACE_OBJ = $(CHPEG_SOURCE_FILES:%.c=$(BUILD_TRACE)/chpeg/%.o)
CHPEG_TRACE_DEP = $(CHPEG_TRACE_OBJ:%.o=%.d)

all: $(BIN) $(BIN_TRACE)

$(BIN): $(OBJ) $(CHPEG_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(BIN_TRACE): $(OBJ_TRACE) $(CHPEG_TRACE_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

-include $(DEP)
-include $(CHPEG_DEP)

-include $(DEP_TRACE)
-include $(CHPEG_TRACE_DEP)

$(BUILD)/%.o : %.c | $(BUILD)/src/actions
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c | $(BUILD)/chpeg
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(BUILD_TRACE)/%.o : %.c | $(BUILD_TRACE)/src/actions
	$(CC) $(CFLAGS_TRACE) $(INCLUDES) -MMD -c $< -o $@

$(BUILD_TRACE)/chpeg/%.o : $(CHPEG_SRC)/%.c | $(BUILD_TRACE)/chpeg
	$(CC) $(CFLAGS_TRACE) $(INCLUDES) -MMD -c $< -o $@

# directories

$(BUILD)/src/actions:
	-mkdir -p $(BUILD)/src/actions

$(BUILD)/chpeg:
	-mkdir -p $(BUILD)/chpeg

$(BUILD_TRACE)/src/actions:
	-mkdir -p $(BUILD_TRACE)/src/actions

$(BUILD_TRACE)/chpeg:
	-mkdir -p $(BUILD_TRACE)/chpeg

.PHONY: clean
clean:
	-rm -f $(BIN) $(BIN_TRACE)
	-rm -rf $(BUILD) $(BUILD_TRACE)

