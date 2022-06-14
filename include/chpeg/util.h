//
// chpeg: util.h {
//

#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

#ifndef CHPEG_API
#define CHPEG_API extern
#define CHPEG_DEF
#endif

// define to enable ALL extensions
#ifdef CHPEG_EXTENSION_ALL
#define CHPEG_EXTENSION_TRIM 1
#define CHPEG_EXTENSION_REFS 1
#endif

#ifndef CHPEG_EXTENSION_TRIM
#define CHPEG_EXTENSION_TRIM 0
#endif

#ifndef CHPEG_EXTENSION_REFS
#define CHPEG_EXTENSION_REFS 0
#endif

// used to check if using ANY extension
#if CHPEG_EXTENSION_TRIM || CHPEG_EXTENSION_REFS
#define CHPEG_USES_EXTENSIONS 1
#endif

#include <stddef.h>
#include <stdarg.h>

#define CHPEG_FLAGS_DISPLAY_LENGTH 3
#define CHPEG_FLAGS_DISPLAY_SIZE (CHPEG_FLAGS_DISPLAY_LENGTH+1)

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

#ifndef CHPEG_INPUT_SIZE_T
#define CHPEG_INPUT_SIZE_T
//typedef size_t isz_t;
//#define ISZ_FMT2 "zu"
//#define ISZ_FMT "%" ISZ_FMT2
typedef unsigned int isz_t;
#define ISZ_FMT2 "u"
#define ISZ_FMT "%" ISZ_FMT2
#endif

CHPEG_API char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit);
CHPEG_API int chpeg_read_file(const char *filename, unsigned char **data, isz_t *length);
CHPEG_API void chpeg_line_col(const unsigned char *input, isz_t offset,
    isz_t *line_out, isz_t *col_out);

#ifndef chpeg_abort
#define chpeg_abort(msg) \
    do { \
        fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, __func__, msg); \
        abort(); \
    } while(0)
#endif

#ifndef CHPEG_CUSTOM_SHOW_MESSAGE
CHPEG_API int chpeg_show_message(int msg_type, const char *fmt, ...);
#endif

#endif // #ifndef CHPEG_UTIL_H

// } chpeg: util.h

