#ifndef CHPEG_COMPILER_H
#define CHPEG_COMPILER_H

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

#include "bytecode.h"

//
// Compiler
//

extern ByteCode *Compiler_compile(const unsigned char *input, int size, int *result_return, int verbose);
extern const ByteCode *Compiler_bytecode();

#endif // #ifndef CHPEG_COMPILER_H
