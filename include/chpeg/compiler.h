//
// chpeg: compiler.h {
//

#ifndef CHPEG_COMPILER_H
#define CHPEG_COMPILER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#include "chpeg/bytecode.h"
#endif

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

//
// Compiler
//

CHPEG_API int chpeg_compile(const unsigned char *input, size_t length,
    ChpegByteCode **bytecode_return, int verbose);

CHPEG_API const ChpegByteCode *chpeg_default_bytecode(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CHPEG_COMPILER_H

// } chpeg: compiler.h

