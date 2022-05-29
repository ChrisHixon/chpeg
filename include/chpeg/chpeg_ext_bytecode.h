#ifndef CHPEG_CHPEG_BYTECODE_H
#define CHPEG_CHPEG_BYTECODE_H

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
#define CHPEG_DEF_TRIM 7
#define CHPEG_DEF_OPTIONS 8
#define CHPEG_DEF_IDENTIFIER 9
#define CHPEG_DEF_LITERAL 10
#define CHPEG_DEF_CHARCLASS 11
#define CHPEG_DEF_CHARRANGE 12
#define CHPEG_DEF_CHAR 13
#define CHPEG_DEF_ESCCHAR 14
#define CHPEG_DEF_OCTCHAR 15
#define CHPEG_DEF_PLAINCHAR 16
#define CHPEG_DEF_PREDOP 17
#define CHPEG_DEF_REPOP 18
#define CHPEG_DEF_DOT 19
#define CHPEG_DEF_S 20

CHPEG_API const ChpegByteCode chpeg_bytecode;

#endif // #ifndef CHPEG_CHPEG_BYTECODE_H
