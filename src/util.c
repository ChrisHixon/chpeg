//
// chpeg: util.c {
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/util.h"
#include "chpeg/bytecode.h"
#endif

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

// escape bytes like a C literal string, optionally truncating to limit, adding "..." at the end
// caller must free() returned value
CHPEG_DEF char *chpeg_esc_bytes(const unsigned char *bytes, int length, int limit)
{
    static const char *hex_digit = "0123456789ABCDEF";
    static const char *esc_chars = "\\\"\t\r\n";

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

CHPEG_DEF int chpeg_read_file(const char *filename, unsigned char **data, size_t *length)
{
    int err = 0;
    size_t bsize = 0, remain = 0, bytes_read = 0, len = 0;
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
            err = 1;
            goto cleanup;
        }
    }

    // initial allocation
    buf = (unsigned char *)CHPEG_MALLOC(CHPEG_READ_FILE_INITIAL_SIZE);
    if (buf == NULL) {
        perror("malloc");
        err = 1;
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
                buf = (unsigned char *)CHPEG_REALLOC(buf, bsize * 2);
                if (buf == NULL) {
                    perror("realloc");
                    err = 1;
                    goto cleanup;
                }
                p = buf + bsize;
                remain = bsize;
                bsize *= 2;
            }
        }
        if (feof(fp)) {
            err = 0;
            break;
        }
        else if (ferror(fp)) {
            fprintf(stderr, "Error reading file: %s\n",
                filename ? filename : "<stdin>");
            err = 1;
            goto cleanup;
        }
    }

cleanup:
    if (filename && fp) { // close only if fopen'd a file
        fclose(fp);
    }
    if (err) { // if we're returning error
        if (buf) {
            free(buf); // free the allocated buffer
        }
        *data = NULL;
        *length = 0;
    }
    else {
        if (len > CHPEG_READ_FILE_INITIAL_SIZE) {
            buf = (unsigned char *)CHPEG_REALLOC(buf, len);
        }
        *data = buf;
        *length = len;
    }
    return err;
}

CHPEG_DEF char *chpeg_flags(char *buf, int flags)
{
    buf[CHPEG_FLAGS_DISPLAY_LENGTH] = '\0';

    if (flags & CHPEG_FLAG_IGNORE) {
        buf[2] = 'I';
    }
    else if (flags & CHPEG_FLAG_STOP) {
        buf[2] = 'S';
    }
    else if (flags & CHPEG_FLAG_LEAF) {
        buf[2] = 'L';
    }
    else {
        buf[2] = '-';
    }

    if (flags & CHPEG_FLAG_PACKRAT) {
        buf[1] = 'P';
    }
    else {
        buf[1] = '-';
    }

    if (flags & CHPEG_FLAG_REFSCOPE) {
        buf[0] = 'R';
    }
    else {
        buf[0] = '-';
    }
    return buf;
}

CHPEG_DEF void chpeg_line_col(const unsigned char *input, size_t offset, size_t *line_out, size_t *col_out)
{
    size_t i, line = 1;

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

// Convert `length` bytes containing '0'-'9', from `input`, to an unsigned int
// `*uint_out`, with `max` value. This is meant to convert a known detected
// chunk of '0'-'9' to an unsigned int.
// Returns: 0 on success; non-zero on error; unsigned int result is returned via *uint_out
CHPEG_DEF int chpeg_bytes2uint(const unsigned char *input, size_t length,
    unsigned int max, unsigned int *uint_out)
{
    if (!uint_out) { return 1; }
    if (!length) { *uint_out = 0; return 2; }

    unsigned int val = 0, digit = 0, max_div_10 = max/10, max_mod_10 = max%10;

    for (size_t z = 0; z < length; ++z, ++input) {
        if (*input < '0' || *input > '9') {
            *uint_out = 0;
            return 3;
        }
        digit = (*input - '0');
        if (val > max_div_10 || (val == max_div_10 && digit > max_mod_10)) {
            *uint_out = 0;
            return 4;
        }
        val = val*10 + digit;
    }
    assert(val <= max);
    *uint_out = val;
    return 0;
}

// Return a zero-terminated token from `input` at offset `offset` with length `length`.
// Result must be freed with CHPEG_FREE().
CHPEG_DEF char *chpeg_token(const unsigned char *input, size_t offset, size_t length)
{
    char *token = (char *)CHPEG_MALLOC(length + 1);
    memcpy(token, input + offset, length);
    token[length] = '\0';
    return token;
}


#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: util.c

