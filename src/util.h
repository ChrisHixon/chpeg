#ifndef CHPEG_UTIL_H
#define CHPEG_UTIL_H

extern char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit);
extern int chpeg_read_file(const char *filename, unsigned char **data, size_t *length);

#endif // #ifndef CHPEG_UTIL_H
