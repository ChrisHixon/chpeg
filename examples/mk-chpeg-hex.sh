#./parse_file --cbytecode chpeg_hex_bytecode chpeg-hex.chpeg 
./chpeg --cbytecode chpeg_hex_bytecode chpeg-hex.chpeg 
cat \
    chpeg-hex-cfg.h \
    ../src/bytecode.h \
    ../src/mem.h \
    ../src/opcodes.h \
    ../src/util.h \
    chpeg_hex_bytecode.h \
    ../src/parser.h \
    ../src/compiler.h \
    ../src/util.c \
    ../src/mem.c \
    ../src/opcodes.c \
    chpeg_hex_bytecode.c \
    ../src/bytecode.c \
    ../src/compiler.c \
    ../src/parser.c \
    parse_file.c \
    > chpeg-hex.c

#gcc -g -Wall -DCHPEG_AMALGAMATION -DCHPEG_API=static -o chpeg-hex chpeg-hex.c
gcc -g -Wall -o chpeg-hex chpeg-hex.c