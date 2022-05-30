out_bfn=chpeg_chpeg

#./parse_file --cbytecode chpeg_hex_bytecode chpeg-hex.chpeg
./chpeg --cbytecode ${out_bfn=chpeg_chpeg}_bc ../grammars/chpeg.chpeg


cat <<EOS > $out_bfn.c
#define CHPEG_AMALGAMATION
#define CHPEG_API static
#define CHPEG_DEF static
//#define CHPEG_HAS_EXTRA_ESCAPE
//#define CHPEG_HAS_HEXCHAR
#define CHPEG_BC(bc) CHPEG_CHPEG_BC_##bc
#define CHPEG_DEFAULT_BYTECODE (&${out_bfn}_bc)
#define CHPEG_DEFINITION_TRACE

EOS

cat \
    ../include/chpeg/util.h \
    ../include/chpeg/bytecode.h \
   ../include/chpeg/opcodes.h \
    ${out_bfn}_bc.h \
    ../include/chpeg/parser.h \
    ../include/chpeg/compiler.h \
    ../src/util.c \
    ../src/opcodes.c \
    ${out_bfn}_bc.c \
    ../src/bytecode.c \
    ../src/compiler.c \
    ../src/parser.c \
    parse_file.c \
    >> $out_bfn.c

#gcc -g -Wall -DCHPEG_AMALGAMATION -DCHPEG_API=static -o chpeg-hex chpeg-hex.c
gcc -g -Wall -o $out_bfn $out_bfn.c