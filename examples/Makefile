SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -std=c99 -O3
INCLUDES=-I$(INCLUDE)
LDFLAGS=

INCLUDE=../include
CHPEG_INCLUDE=$(INCLUDE)/chpeg
CHPEG_AMALGAMATION=$(CHPEG_INCLUDE)/chpeg.h # used to remake when this has changed

EXAMPLES=parse_file

# rules
all: $(EXAMPLES)

parse_file: $(CHPEG_AMALGAMATION)
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) parse_file.c -o parse_file

.PHONY: clean
clean:
	-rm -f $(EXAMPLES)
