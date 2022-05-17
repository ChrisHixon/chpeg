cat \
    ../include/chpeg/util.h \
    ../include/chpeg/bytecode.h \
    ../include/chpeg/opcodes.h \
    ../include/chpeg/chpeg_bytecode.h \
    ../include/chpeg/parser.h \
    ../include/chpeg/compiler.h \
    ../src/util.c \
    ../src/opcodes.c \
    ../src/chpeg_bytecode.c \
    ../src/bytecode.c \
    ../src/compiler.c \
    ../src/parser.c \
    parse_file.c \
    > chpeg.c

gcc -g -Wall -DCHPEG_AMALGAMATION -DCHPEG_API=static -DCHPEG_DEF=static -o chpeg chpeg.c