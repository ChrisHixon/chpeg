#include <stdio.h>
#include <stdlib.h>

#ifndef CHPEG_AMALGAMATION
#include "mem.h"
#endif /*CHPEG_AMALGAMATION*/

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
