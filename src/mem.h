#ifndef CHPEG_MEM_H
#define CHPEG_MEM_H

#ifndef CHPEG_AMALGAMATION
#include "bytecode.h"
#endif /*CHPEG_AMALGAMATION*/

#ifndef CHPEG_MALLOC
CHPEG_API void *chpeg_malloc(size_t sz);
CHPEG_API void *chpeg_realloc(void *prev_ptr, size_t sz);
CHPEG_API void *chpeg_calloc(size_t count, size_t sz);
CHPEG_API void chpeg_free(void *ptr);
#define CHPEG_MALLOC(sz) chpeg_malloc(sz)
#define CHPEG_REALLOC(ptr, sz) chpeg_realloc(ptr, sz)
#define CHPEG_CALLOC(count, sz) chpeg_calloc(count, sz)
#define CHPEG_FREE(ptr) chpeg_free(ptr)
#endif

#endif // #ifndef CHPEG_MEM_H
