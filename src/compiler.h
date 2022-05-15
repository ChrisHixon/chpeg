#ifndef CHPEG_COMPILER_H
#define CHPEG_COMPILER_H

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

#include "bytecode.h"

//
// Compiler
//

extern int chpeg_compile(const unsigned char *input, size_t length,
    ChpegByteCode **bytecode_return, int verbose);

extern const ChpegByteCode *chpeg_default_bytecode();

#endif // #ifndef CHPEG_COMPILER_H
