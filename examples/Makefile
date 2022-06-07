SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -std=c99 -O3 -DUSE_EXTENSIONS
INCLUDES=-I$(INCLUDE)
LDFLAGS=

INCLUDE=../include
CHPEG_AMALG=$(INCLUDE)/chpeg_ext.h # used to remake when this has changed

EXAMPLES=parse_file

# rules
all: $(EXAMPLES)

parse_file: $(CHPEG_AMALG) parse_file.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) parse_file.c -o parse_file

.PHONY: clean
clean:
	-rm -f $(EXAMPLES)
