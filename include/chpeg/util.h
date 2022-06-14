//
// chpeg: util.h {
//

#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#endif

#define CHPEG_FLAGS_DISPLAY_LENGTH 3
#define CHPEG_FLAGS_DISPLAY_SIZE (CHPEG_FLAGS_DISPLAY_LENGTH+1)

CHPEG_API char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit);
CHPEG_API int chpeg_read_file(const char *filename, unsigned char **data, size_t *length);
CHPEG_API void chpeg_line_col(const unsigned char *input, size_t offset,
    size_t *line_out, size_t *col_out);
CHPEG_API char *chpeg_flags(char *buf, int flags);

#ifndef chpeg_abort
#define chpeg_abort(msg) \
    do { \
        fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, __func__, msg); \
        abort(); \
    } while(0)
#endif

#endif // #ifndef CHPEG_UTIL_H

// } chpeg: util.h

