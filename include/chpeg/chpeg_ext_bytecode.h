#ifndef CHPEG_CHPEG_EXT_BYTECODE_H
#define CHPEG_CHPEG_EXT_BYTECODE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

#define CHPEG_DEF_GRAMMAR 0
#define CHPEG_DEF_DEFINITION 1
#define CHPEG_DEF_CHOICE 2
#define CHPEG_DEF_SEQUENCE 3
#define CHPEG_DEF_ELEMENT 4
#define CHPEG_DEF_PREDICATE 5
#define CHPEG_DEF_REPEAT 6
#define CHPEG_DEF_PRIMARY 7
#define CHPEG_DEF_MARK 8
#define CHPEG_DEF_REFERENCE 9
#define CHPEG_DEF_TRIM 10
#define CHPEG_DEF_MINMAX 11
#define CHPEG_DEF_MINMAXVAL 12
#define CHPEG_DEF_OPTIONS 13
#define CHPEG_DEF_IDENTIFIER 14
#define CHPEG_DEF_LITERAL 15
#define CHPEG_DEF_CHARCLASS 16
#define CHPEG_DEF_NCHARCLASS 17
#define CHPEG_DEF_CHARRANGE 18
#define CHPEG_DEF_CHAR 19
#define CHPEG_DEF_ESCCHAR 20
#define CHPEG_DEF_OCTCHAR 21
#define CHPEG_DEF_HEXCHAR 22
#define CHPEG_DEF_PLAINCHAR 23
#define CHPEG_DEF_PREDOP 24
#define CHPEG_DEF_REPOP 25
#define CHPEG_DEF_DOT 26
#define CHPEG_DEF_COMMA 27
#define CHPEG_DEF_S 28

extern const ChpegByteCode chpeg_ext_bytecode;

#ifdef __cplusplus
}
#endif

#endif // #ifndef CHPEG_CHPEG_EXT_BYTECODE_H
