#ifndef CHPEG_CHPEG_BYTECODE_H
#define CHPEG_CHPEG_BYTECODE_H

#include "bytecode.h"
#include "opcodes.h"

#define CHPEG_GRAMMAR 0
#define CHPEG_DEFINITION 1
#define CHPEG_CHOICE 2
#define CHPEG_SEQUENCE 3
#define CHPEG_PREDICATE 4
#define CHPEG_REPEAT 5
#define CHPEG_PRIMARY 6
#define CHPEG_OPTIONS 7
#define CHPEG_IDENTIFIER 8
#define CHPEG_LITERAL 9
#define CHPEG_CHARCLASS 10
#define CHPEG_CHARRANGE 11
#define CHPEG_CHAR 12
#define CHPEG_ESCCHAR 13
#define CHPEG_OCTCHAR 14
#define CHPEG_PLAINCHAR 15
#define CHPEG_PREDOP 16
#define CHPEG_REPOP 17
#define CHPEG_DOT 18
#define CHPEG_S 19

extern ByteCode chpeg_bytecode;

#endif // CHPEG_CHPEG_BYTECODE_H