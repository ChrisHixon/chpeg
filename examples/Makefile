SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -std=c99 -O3 -DCHPEG_EXTENSION_ALL -DPACKRAT
LDFLAGS=

AMALGAMATION_DIR=../build
AMALGAMATION_ALL=$(AMALGAMATION_DIR)/chpeg_all.h
INCLUDES=-I$(AMALGAMATION_DIR)

EXAMPLES=parse_file

# rules
all: $(EXAMPLES)

parse_file: parse_file.c $(AMALGAMATION_ALL)
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) $< -o parse_file

.PHONY: clean
clean:
	-rm -f $(EXAMPLES)
