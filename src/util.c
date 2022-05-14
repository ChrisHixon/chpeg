#include <stddef.h>
#include <string.h>

#ifndef CHPEG_AMALGAMATION
#include "mem.h"
#include "util.h"
#endif /*CHPEG_AMALGAMATION*/

// escape bytes like a C literal string, optionally truncating to limit, adding "..." at the end
// caller must free() returned value
CHPEG_API char *esc_bytes(const unsigned char *bytes, int length, int limit)
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
