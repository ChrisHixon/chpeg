//
// chpeg: mem.c {
//

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#endif /*CHPEG_AMALGAMATION*/

#ifdef CHPEG_MALLOC_WRAPPERS

CHPEG_DEF void *chpeg_malloc(size_t sz) {
    void *ptr = malloc(sz);
    if(ptr == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_DEF void *chpeg_realloc(void *prev_ptr, size_t sz) {
    void *ptr = realloc(prev_ptr, sz);
    if(ptr == NULL) {
        fprintf(stderr, "Failed to reallocate %zu bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_DEF void *chpeg_calloc(size_t count, size_t sz) {
    void *ptr = calloc(count, sz);
    if(ptr == NULL) {
        fprintf(stderr, "Failed to allocate and initialize %zu bytes of memory", sz);
        exit(-1);
    }
    return ptr;
}

CHPEG_DEF void chpeg_free(void *ptr) {
    free(ptr);
}

#endif

#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: mem.c

