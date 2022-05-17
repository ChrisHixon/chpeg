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
#ifndef CHPEG_DISABLE_MALLOC_WRAPPERS

#define CHPEG_MALLOC_WRAPPERS
CHPEG_API void *chpeg_malloc(size_t sz);
CHPEG_API void *chpeg_realloc(void *prev_ptr, size_t sz);
CHPEG_API void *chpeg_calloc(size_t count, size_t sz);
CHPEG_API void chpeg_free(void *ptr);
#define CHPEG_MALLOC(sz) chpeg_malloc(sz)
#define CHPEG_REALLOC(ptr, sz) chpeg_realloc(ptr, sz)
#define CHPEG_CALLOC(count, sz) chpeg_calloc(count, sz)
#define CHPEG_FREE(ptr) chpeg_free(ptr)

#else

#define CHPEG_MALLOC(sz) malloc(sz)
#define CHPEG_REALLOC(ptr, sz) realloc(ptr, sz)
#define CHPEG_CALLOC(count, sz) calloc(count, sz)
#define CHPEG_FREE(ptr) free(ptr)

#endif
#endif

#endif // #ifndef CHPEG_MEM_H

// } chpeg: mem.h

