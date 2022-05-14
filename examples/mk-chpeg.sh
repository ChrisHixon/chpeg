cat \
    ../src/bytecode.h \
    ../src/mem.h \
    ../src/opcodes.h \
    ../src/util.h \
    ../src/chpeg_bytecode.h \
    ../src/parser.h \
    ../src/compiler.h \
    ../src/util.c \
    ../src/mem.c \
    ../src/opcodes.c \
    ../src/chpeg_bytecode.c \
    ../src/bytecode.c \
    ../src/compiler.c \
    ../src/parser.c \
    parse_file.c \
    > chpeg.c

gcc -g -Wall -DCHPEG_AMALGAMATION -DCHPEG_API=static -o chpeg chpeg.c