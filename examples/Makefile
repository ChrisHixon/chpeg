SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -std=c99 -O3
INCLUDES=-I$(INCLUDE)
LDFLAGS=

INCLUDE=../include

EXAMPLES=parse_file

# rules
all: $(EXAMPLES)

parse_file:
	$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) parse_file.c -o parse_file

.PHONY: clean
clean:
	-rm -f $(EXAMPLES)
	-rm chpeg chpeg.c chpeg_bc.[ch]
	-rm chpeg_chpeg chpeg_chpeg.c chpeg_chpeg_bc.[ch]
	-rm chpeg_hex chpeg_hex.c chpeg_hex_bc.[ch]
	-rm chpeg_nocase chpeg_nocase.c chpeg_nocase_bc.[ch]
