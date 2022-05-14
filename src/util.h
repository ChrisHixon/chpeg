#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

#ifndef CHPEG_AMALGAMATION
#include "bytecode.h"
#endif /*CHPEG_AMALGAMATION*/

CHPEG_API char *esc_bytes(const unsigned char *bytes, int length, int limit);

#endif // #ifndef CHPEG_UTIL_H
