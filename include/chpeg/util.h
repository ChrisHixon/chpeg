//
// chpeg: util.h {
//

#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#endif

#define CHPEG_FLAGS_DISPLAY_LENGTH 3
#define CHPEG_FLAGS_DISPLAY_SIZE (CHPEG_FLAGS_DISPLAY_LENGTH+1)

CHPEG_API char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit);
CHPEG_API int chpeg_read_file(const char *filename, unsigned char **data, size_t *length);
CHPEG_API char *chpeg_flags(char *buf, int flags);
CHPEG_API void chpeg_line_col(const unsigned char *input, size_t offset,
    size_t *line_out, size_t *col_out);
CHPEG_API int chpeg_bytes2uint(const unsigned char *input, size_t length, unsigned int max, unsigned int *uint_out);

// chpeg_abort() should only be used to abort upon internal/programming errors, not
// errors that can occur from user input, similar to how assert() should be used.
#ifndef chpeg_abort
#define chpeg_abort(fmt, ...) \
    do { \
        fprintf(stderr, "%s:%d: %s: " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        abort(); \
    } while(0)
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CHPEG_UTIL_H

// } chpeg: util.h

