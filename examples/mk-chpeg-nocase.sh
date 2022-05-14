#./parse_file --cbytecode chpeg_hex_bytecode chpeg-hex.chpeg 
./chpeg-hex --cbytecode chpeg_nocase_bytecode cbyte-nocase.peg 
cat \
    chpeg-nocase-cfg.h \
    ../src/bytecode.h \
    ../src/mem.h \
    ../src/opcodes.h \
    ../src/util.h \
    chpeg_nocase_bytecode.h \
    ../src/parser.h \
    ../src/compiler.h \
    ../src/util.c \
    ../src/mem.c \
    ../src/opcodes.c \
    chpeg_nocase_bytecode.c \
    ../src/bytecode.c \
    ../src/compiler.c \
    ../src/parser.c \
    parse_file.c \
    > chpeg-nocase.c

#gcc -g -Wall -DCHPEG_AMALGAMATION -DCHPEG_API=static -o chpeg-hex chpeg-hex.c
gcc -g -Wall -o chpeg-nocase chpeg-nocase.c