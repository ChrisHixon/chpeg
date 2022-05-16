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
