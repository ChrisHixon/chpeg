SHELL=/bin/sh

# directories
BUILD=build
INCLUDE=include
CHPEG_INCLUDE=$(INCLUDE)/chpeg
CHPEG_SRC=src

# amalgamations
AMALGAMATIONS = chpeg_all.h chpeg.c chpeg.h
AMALGAMATION_FILES=$(AMALGAMATIONS:%=$(BUILD)/%)

# header and source files: list these in amalgamation order (normal)
CHPEG_HEADER_FILES=chpeg_api.h \
		   mem.h opcodes.h bytecode.h chpeg_bytecode.h chpeg_ext_bytecode.h \
		   util.h parser.h compiler.h
CHPEG_SOURCE_FILES=mem.c opcodes.c bytecode.c chpeg_bytecode.c chpeg_ext_bytecode.c \
		   util.c parser.c compiler.c

# header and source files: list these in amalgamation order (extensions)
CHPEG_EXT_HEADER_FILES=chpeg_api.h \
		   mem.h opcodes.h bytecode.h chpeg_ext_bytecode.h util.h parser.h compiler.h
CHPEG_EXT_SOURCE_FILES=mem.c opcodes.c bytecode.c chpeg_ext_bytecode.c util.c parser.c compiler.c

CHPEG_SOURCES=$(CHPEG_SOURCE_FILES:%.c=$(CHPEG_SRC)/%.c)
CHPEG_HEADERS=$(CHPEG_HEADER_FILES:%.h=$(CHPEG_INCLUDE)/%.h)

CHPEG_EXT_SOURCES=$(CHPEG_EXT_SOURCE_FILES:%.c=$(CHPEG_SRC)/%.c)
CHPEG_EXT_HEADERS=$(CHPEG_EXT_HEADER_FILES:%.h=$(CHPEG_INCLUDE)/%.h)

# rules

all: amalgamations

# bytecode

bytecode: bytecode-std bytecode-ext

bytecode-std: grammars/chpeg.chpeg
	-util/chpeg --bytecodec -g $< -basename chpeg_bytecode -cout src/chpeg_bytecode.c -hout include/chpeg/chpeg_bytecode.h
	-sed -i -E 's,^#include "chpeg_bytecode.h",#include "chpeg/chpeg_bytecode.h",' src/chpeg_bytecode.c

bytecode-ext: grammars/chpeg-ext.chpeg
	-util/chpeg --bytecodec -g $< -basename chpeg_ext_bytecode -cout src/chpeg_ext_bytecode.c -hout include/chpeg/chpeg_ext_bytecode.h
	-sed -i -E 's,^#include "chpeg_ext_bytecode.h",#include "chpeg/chpeg_ext_bytecode.h",' src/chpeg_ext_bytecode.c



#
# separate .c and .h amalgamation (in progress experiment)
#

.PHONY: amalgamations
amalgamations: $(AMALGAMATION_FILES)

# all-in-one amalgamation file: chpeg_all.h
$(BUILD)/chpeg_all.h: $(CHPEG_HEADERS) $(CHPEG_SOURCES)
	-mkdir -p $(@D)
	-printf "#define CHPEG_AMALGAMATION\n#define CHPEG_AMALGAMATION_ONE_FILE\n\n" > $@
	-cat $^ >> $@

# separate .c / .h files: chpeg.c
$(BUILD)/chpeg.c: $(CHPEG_SOURCES)
	-mkdir -p $(@D)
	-printf '#define CHPEG_AMALGAMATION\n#include "chpeg.h"\n\n' > $@
	-cat $^ >> $@

# separate .c / .h files: chpeg.h
$(BUILD)/chpeg.h: $(CHPEG_HEADERS)
	-mkdir -p $(@D)
	-printf '#define CHPEG_AMALGAMATION\n\n' > $@
	-cat $^ >> $@

.PHONY: clean
clean:
	-rm -rf $(BUILD)

