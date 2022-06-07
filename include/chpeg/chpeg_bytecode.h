#ifndef CHPEG_CHPEG_BYTECODE_H
#define CHPEG_CHPEG_BYTECODE_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/util.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

#define CHPEG_BC_GRAMMAR 0
#define CHPEG_BC_DEFINITION 1
#define CHPEG_BC_CHOICE 2
#define CHPEG_BC_SEQUENCE 3
#define CHPEG_BC_PREDICATE 4
#define CHPEG_BC_REPEAT 5
#define CHPEG_BC_PRIMARY 6
#define CHPEG_BC_OPTIONS 7
#define CHPEG_BC_IDENTIFIER 8
#define CHPEG_BC_LITERAL 9
#define CHPEG_BC_CHARCLASS 10
#define CHPEG_BC_CHARRANGE 11
#define CHPEG_BC_CHAR 12
#define CHPEG_BC_ESCCHAR 13
#define CHPEG_BC_OCTCHAR 14
#define CHPEG_BC_PLAINCHAR 15
#define CHPEG_BC_PREDOP 16
#define CHPEG_BC_REPOP 17
#define CHPEG_BC_DOT 18
#define CHPEG_BC_S 19

CHPEG_API const ChpegByteCode chpeg_bytecode;

#endif // #ifndef CHPEG_CHPEG_BYTECODE_H
