SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -Wno-unused-function -std=c99 -g -DCHPEG_EXTENSION_ALL -DCHPEG_PACKRAT -DDEBUG_CALC
LDFLAGS=

CHPEG_UTIL=../../util/chpeg
AMALGAMATION_DIR=../../build
AMALGAMATION_ALL=$(AMALGAMATION_DIR)/chpeg_all.h
INCLUDES=-I$(AMALGAMATION_DIR)

BIN=calc
SRC=calc.c
BYTECODE=calc_bytecode.h
GRAMMAR=calc.chpeg

# rules
all: calc

$(BYTECODE): $(GRAMMAR)
	-$(CHPEG_UTIL) --bytecodec -g $(GRAMMAR) -basename calc_bytecode -prefix calc > $(BYTECODE)

$(BIN): $(SRC) $(AMALGAMATION_ALL) $(BYTECODE)
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) $(SRC) -o $(BIN)

.PHONY: clean
clean:
	-rm -f $(BIN) $(BYTECODE)
