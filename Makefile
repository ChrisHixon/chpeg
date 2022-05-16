SHELL=/bin/sh

# directories
INCLUDE=include
CHPEG_INCLUDE=$(INCLUDE)/chpeg
CHPEG_AMALGAMATION=$(CHPEG_INCLUDE)/chpeg.h
CHPEG_SRC=src

# list these in amalgamation order
CHPEG_HEADER_FILES=chpeg_api.h mem.h \
		   opcodes.h bytecode.h chpeg_bytecode.h util.h parser.h compiler.h
CHPEG_SOURCE_FILES=opcodes.c bytecode.c chpeg_bytecode.c util.c parser.c compiler.c

CHPEG_SOURCES=$(patsubst %.c,$(CHPEG_SRC)/%.c,$(CHPEG_SOURCE_FILES))
CHPEG_OBJECTS=$(patsubst $(CHPEG_SRC)/%.c,$(CHPEG_OBJDIR)/%.o,$(CHPEG_SOURCES))
CHPEG_HEADERS=$(patsubst %.h,$(CHPEG_INCLUDE)/%.h,$(CHPEG_HEADER_FILES))

# rules

all: $(CHPEG_AMALGAMATION)

# concatenate amalgamation files
$(CHPEG_AMALGAMATION): $(CHPEG_HEADERS) $(CHPEG_SOURCES)
	-printf "#define CHPEG_AMALGAMATION\n\n" > $(CHPEG_AMALGAMATION)
	-cat $(CHPEG_HEADERS) $(CHPEG_SOURCES) >> $(CHPEG_AMALGAMATION)

.PHONY: clean
clean:
	-rm -f $(CHPEG_AMALGAMATION)

