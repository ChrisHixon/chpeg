//
// chpeg: util.h {
//

#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

#ifndef CHPEG_API
#define CHPEG_API extern
#define CHPEG_DEF
#endif

#include <stddef.h>

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

CHPEG_API char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit);
CHPEG_API int chpeg_read_file(const char *filename, unsigned char **data, size_t *length);

#endif // #ifndef CHPEG_UTIL_H

// } chpeg: util.h

