//
// chpeg: mem.h {
//

#ifndef CHPEG_MEM_H
#define CHPEG_MEM_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#endif

#include <stdlib.h>

#ifndef CHPEG_MALLOC
#define CHPEG_MALLOC(sz) malloc(sz)
#define CHPEG_REALLOC(ptr, sz) realloc(ptr, sz)
#define CHPEG_CALLOC(count, sz) calloc(count, sz)
#define CHPEG_FREE(ptr) free(ptr)
#endif

#endif // #ifndef CHPEG_MEM_H

// } chpeg: mem.h

