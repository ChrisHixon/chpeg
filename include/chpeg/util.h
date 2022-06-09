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
CHPEG_API char *chpeg_flags(char *buf, int flags);

#endif // #ifndef CHPEG_UTIL_H

// } chpeg: util.h

