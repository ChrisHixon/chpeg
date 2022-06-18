#ifndef CHPEG_CHPEG_BYTECODE_H
#define CHPEG_CHPEG_BYTECODE_H

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
#define CHPEG_DEF_PREDICATE 4
#define CHPEG_DEF_REPEAT 5
#define CHPEG_DEF_PRIMARY 6
#define CHPEG_DEF_OPTIONS 7
#define CHPEG_DEF_IDENTIFIER 8
#define CHPEG_DEF_LITERAL 9
#define CHPEG_DEF_CHARCLASS 10
#define CHPEG_DEF_CHARRANGE 11
#define CHPEG_DEF_CHAR 12
#define CHPEG_DEF_ESCCHAR 13
#define CHPEG_DEF_OCTCHAR 14
#define CHPEG_DEF_PLAINCHAR 15
#define CHPEG_DEF_PREDOP 16
#define CHPEG_DEF_REPOP 17
#define CHPEG_DEF_DOT 18
#define CHPEG_DEF_S 19

extern const ChpegByteCode chpeg_bytecode;

#ifdef __cplusplus
}
#endif

#endif // #ifndef CHPEG_CHPEG_BYTECODE_H
