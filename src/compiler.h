#ifndef CHPEG_COMPILER_H
#define CHPEG_COMPILER_H

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

#ifndef CHPEG_AMALGAMATION
#include "bytecode.h"
#endif /*CHPEG_AMALGAMATION*/

//
// Compiler
//

CHPEG_API ByteCode *Compiler_compile(const unsigned char *input, int size, int *result_return, int verbose);
CHPEG_API const ByteCode *Compiler_bytecode();

#endif // #ifndef CHPEG_COMPILER_H
