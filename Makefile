SHELL=/bin/sh

# directories
INCLUDE=include
CHPEG_INCLUDE=$(INCLUDE)/chpeg
CHPEG_SRC=src
CHPEG_AMALG=$(INCLUDE)/chpeg.h
CHPEG_EXT_AMALG=$(INCLUDE)/chpeg_ext.h

# list these in amalgamation order (normal)
CHPEG_HEADER_FILES=util.h opcodes.h bytecode.h chpeg_bytecode.h parser.h compiler.h
CHPEG_SOURCE_FILES=util.c opcodes.c bytecode.c chpeg_bytecode.c parser.c compiler.c

# list these in amalgamation order (extensions)
CHPEG_EXT_HEADER_FILES=util.h opcodes.h bytecode.h chpeg_ext_bytecode.h parser.h compiler.h
CHPEG_EXT_SOURCE_FILES=util.c opcodes.c bytecode.c chpeg_ext_bytecode.c parser.c compiler.c

CHPEG_SOURCES=$(patsubst %.c,$(CHPEG_SRC)/%.c,$(CHPEG_SOURCE_FILES))
CHPEG_HEADERS=$(patsubst %.h,$(CHPEG_INCLUDE)/%.h,$(CHPEG_HEADER_FILES))

CHPEG_EXT_SOURCES=$(patsubst %.c,$(CHPEG_SRC)/%.c,$(CHPEG_EXT_SOURCE_FILES))
CHPEG_EXT_HEADERS=$(patsubst %.h,$(CHPEG_INCLUDE)/%.h,$(CHPEG_EXT_HEADER_FILES))

# rules

all: $(CHPEG_AMALG) $(CHPEG_EXT_AMALG)

bytecode-std: grammars/chpeg.chpeg
	-util/chpeg --bytecodec -g $< -basename chpeg_bytecode -cout src/chpeg_bytecode.c -hout include/chpeg/chpeg_bytecode.h
	-sed -i -E 's,^#include "chpeg_bytecode.h",#include "chpeg/chpeg_bytecode.h",' src/chpeg_bytecode.c

bytecode-ext: grammars/chpeg-ext.chpeg
	-util/chpeg --bytecodec -g $< -basename chpeg_bytecode -cout src/chpeg_ext_bytecode.c -hout include/chpeg/chpeg_ext_bytecode.h
	-sed -i -E 's,^#include "chpeg_bytecode.h",#include "chpeg/chpeg_ext_bytecode.h",' src/chpeg_ext_bytecode.c

bytecode: bytecode-std bytecode-ext

# concatenate amalgamation files (normal)
$(CHPEG_AMALG): $(CHPEG_HEADERS) $(CHPEG_SOURCES)
	-printf "#define CHPEG_AMALGAMATION\n\n" > $(CHPEG_AMALG)
	-cat $(CHPEG_HEADERS) $(CHPEG_SOURCES) >> $(CHPEG_AMALG)

# concatenate amalgamation files (extensions)
$(CHPEG_EXT_AMALG): $(CHPEG_EXT_HEADERS) $(CHPEG_EXT_SOURCES)
	-printf "#define CHPEG_AMALGAMATION\n\n" > $(CHPEG_EXT_AMALG)
	-cat $(CHPEG_EXT_HEADERS) $(CHPEG_EXT_SOURCES) >> $(CHPEG_EXT_AMALG)

.PHONY: clean
clean:
	-rm -f $(CHPEG_AMALG) $(CHPEG_EXT_AMALG)

