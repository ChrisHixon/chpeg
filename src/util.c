//
// chpeg: util.c {
//

#ifndef CHPEG_AMALGAMATION
#include "chpeg/util.h"
#endif

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// escape bytes like a C literal string, optionally truncating to limit, adding "..." at the end
// caller must free() returned value
CHPEG_API char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit)
{
    static const char *hex_digit = "0123456789ABCDEF";
#ifdef CHPEG_HAS_EXTRA_ESCAPE
    static const char *esc_chars = "\\\"\a\b\e\f\t\v\r\n";
#else
    static const char *esc_chars = "\\\"\t\r\n";
#endif

    if (!bytes || length < 0) { return NULL; }
    if (limit > 0 && limit < 3) { return NULL; }

    int blen = 0, slen = 0, i = 0, trunc = 0, blimit = 0;
    unsigned char b = 0;
    char *res = NULL;

    for (i = 0; i < length; ++i) {
        blen = 1;
        b = bytes[i];
        if (b == 0 || strchr(esc_chars, b)) {
            blen = 2;
        }
        else if (b < 32 || b > 126) {
            blen = 4;
        }

        if (limit && slen + blen > limit - 3) {
            trunc = 1;
            break;
        }
        slen += blen;
    }

    blimit = i;
    if (trunc) { slen += 3; }

    res = (char *)CHPEG_MALLOC(slen + 1);
    char *p = res;

    for (i = 0; i < blimit; ++i) {
        b = bytes[i];
        if (b == 0) {
            *p++ = '\\';
            *p++ = '0';
        }
        else if (strchr(esc_chars, b)) {
            *p++ = '\\';
            switch (b) {
#ifdef CHPEG_HAS_EXTRA_ESCAPE
                case '\a': *p++ = 'a'; break;
                case '\b': *p++ = 'b'; break;
                case '\e': *p++ = 'e'; break;
                case '\f': *p++ = 'f'; break;
                case '\v': *p++ = 'v'; break;
#endif
                case '\r': *p++ = 'r'; break;
                case '\t': *p++ = 't'; break;
                case '\n': *p++ = 'n'; break;
                default: *p++ = (char)b; break;
            }
        }
        else if (b < 32 || b > 126) {
            *p++ = '\\';
            *p++ = 'x';
            *p++ = hex_digit[(b >> 4) & 0xf];
            *p++ = hex_digit[b & 0xf];
        }
        else {
            *p++ = (char)b;
        }
    }
    if (trunc) { *p++ = '.'; *p++ = '.'; *p++ = '.'; }
    *p = '\0';
    return res;
}

#ifndef CHPEG_READ_FILE_BLOCKSIZE
#define CHPEG_READ_FILE_BLOCKSIZE 4096
#endif

#ifndef CHPEG_READ_FILE_INITIAL_SIZE
#define CHPEG_READ_FILE_INITIAL_SIZE 4096 * 4
#endif

CHPEG_API int chpeg_read_file(const char *filename, unsigned char **data, isz_t *length)
{
    int ret = 0;
    isz_t bsize = 0, remain = 0, bytes_read = 0, len = 0;
    unsigned char *buf = NULL, *p = NULL;
    FILE *fp;

    // return error if either data or length return address is NULL
    if (data == NULL || length == NULL) {
        return 1;
    }

    // read from stdin if filename is NULL or "-"
    if (filename == NULL || strcmp(filename, "-") == 0) {
        filename = NULL;
        fp = stdin;
    }
    // else, open file
    else {
        fp = fopen(filename, "r");
        if (fp == NULL) {
            perror(filename);
            ret = 1;
            goto cleanup;
        }
    }

    // initial allocation
    buf = (unsigned char*)CHPEG_MALLOC(CHPEG_READ_FILE_INITIAL_SIZE);
    if (buf == NULL) {
        perror("malloc");
        ret = 1;
        goto cleanup;
    }
    p = buf;
    bsize = remain = CHPEG_READ_FILE_INITIAL_SIZE;

    for (;;) {
        bytes_read = fread(p, 1, remain < CHPEG_READ_FILE_BLOCKSIZE ?
            remain : CHPEG_READ_FILE_BLOCKSIZE, fp);
        if (bytes_read > 0) {
            remain -= bytes_read;
            p += bytes_read;
            len += bytes_read;
            if (remain == 0) {
                buf = (unsigned char*)CHPEG_REALLOC(buf, bsize * 2);
                if (buf == NULL) {
                    perror("realloc");
                    ret = 1;
                    break;
                }
                p = buf + bsize;
                remain = bsize;
                bsize *= 2;
            }
        }
        else {
            if (feof(fp)) {
                ret = 0;
            }
            else if (ferror(fp)) {
                fprintf(stderr, "Error reading file: %s\n",
                    filename ? filename : "<stdin>");
                ret = 1;
            }
            else {
                fprintf(stderr, "Unknown error reading file: %s\n",
                    filename ? filename : "<stdin>");
                ret = 1;
            }
            break;
        }
    }

cleanup:
    if (filename && fp) { // close only if fopen'd a file
        fclose(fp);
    }
    if (ret) { // if we're returning error
        if (buf) {
            CHPEG_FREE(buf); // free the allocated buffer
        }
        *data = NULL;
        *length = 0;
    }
    else {
        if (len > CHPEG_READ_FILE_INITIAL_SIZE) {
            buf = (unsigned char*)CHPEG_REALLOC(buf, len);
        }
        *data = buf;
        *length = len;
    }
    return ret;
}

void chpeg_line_col(const unsigned char *input, isz_t offset, isz_t *line_out, isz_t *col_out)
{
    isz_t i, line = 1;

    for (i = 0; i <= offset; ++i) {
        if(input[i] == '\n') {
            ++line;
        }
    }

    while (i > 0) {
        if(input[--i] == '\n') {
            ++i;
            break;
        }
    }

    *line_out = line;
    *col_out = 1 + offset - i;
}

#ifndef CHPEG_CUSTOM_SHOW_MESSAGE
CHPEG_API int chpeg_show_message(ChpegParserPtr parser, int msg_type, const char *fmt, ...) {
	va_list argp;
	int rc;
	va_start(argp, fmt);
	rc = vfprintf(stderr, fmt, argp);
	va_end(argp);
	return rc;
}
#endif

#ifndef CHPEG_NO_MALLOC_WRAPPER
CHPEG_API void *chpeg_malloc(size_t sz) {
    void *ptr = malloc(sz);
    if(!ptr) {
        fprintf(stderr, "Failed to allocate %zd bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_API void *chpeg_realloc(void *prev_ptr, size_t sz) {
    void *ptr = realloc(prev_ptr, sz);
    if(!ptr) {
        fprintf(stderr, "Failed to reallocate %zd bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_API void *chpeg_calloc(size_t count, size_t sz) {
    void *ptr = calloc(count, sz);
    if(!ptr) {
        fprintf(stderr, "Failed to allocate and initialize %zd bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_API void chpeg_free(void *ptr) {
    free(ptr);
}
#endif /*CHPEG_NO_MALLOC_WRAPPER*/
// } chpeg: util.c

