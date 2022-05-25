SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -std=c17 -pedantic -O3
CFLAGS_TRACE=-Wall -std=c17 -pedantic -g -DVM_TRACE=1 -DVM_PRINT_TREE
INCLUDES=-I$(INCLUDE) -I./src
LDFLAGS=

INCLUDE=../include

BIN = chpeg
BUILD = build
SRC = $(wildcard src/*.c) $(wildcard src/actions/*.c)
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(OBJ:%.o=%.d)

BIN_TRACE = chpeg-trace
BUILD_TRACE = build_trace
OBJ_TRACE = $(SRC:%.c=$(BUILD_TRACE)/%.o)
DEP_TRACE = $(OBJ_TRACE:%.o=%.d)

all: $(BIN) $(BIN_TRACE)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(BIN_TRACE): $(OBJ_TRACE)
	$(CC) $(LDFLAGS) $^ -o $@

-include $(DEP)

-include $(DEP_TRACE)

$(BUILD)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(BUILD_TRACE)/%.o : %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS_TRACE) $(INCLUDES) -MMD -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(BIN) $(BIN_TRACE)
	-rm -rf $(BUILD) $(BUILD_TRACE)

