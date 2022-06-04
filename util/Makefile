SHELL=/bin/sh

CC=gcc

CFLAGS_COMMON = -Wall -std=c17 -pedantic -DCHPEG_EXTENSIONS -DCHPEG_PACKRAT
CFLAGS = $(CFLAGS_COMMON) -O3
CFLAGS_TRACE = $(CFLAGS_COMMON) -g -DCHPEG_VM_TRACE -DCHPEG_VM_PRINT_TREE -DCHPEG_VM_PROFILE

INCLUDES=-I$(INCLUDE) -I./src
LDFLAGS=

INCLUDE=../include


CHPEG_SOURCE_FILES=mem.c opcodes.c bytecode.c chpeg_ext_bytecode.c util.c parser.c compiler.c
CHPEG_SRC=../src

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

$(BUILD)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(BUILD)/chpeg/%.o : $(CHPEG_SRC)/%.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(BUILD_TRACE)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS_TRACE) $(INCLUDES) -MMD -c $< -o $@

$(BUILD_TRACE)/chpeg/%.o : $(CHPEG_SRC)/%.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS_TRACE) $(INCLUDES) -MMD -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(BIN) $(BIN_TRACE)
	-rm -rf $(BUILD) $(BUILD_TRACE)
