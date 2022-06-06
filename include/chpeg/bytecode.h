//
// chpeg: bytecode.h {
//

#ifndef CHPEG_BYTECODE_H
#define CHPEG_BYTECODE_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#endif

#include <stdio.h>

#define CHPEG_INST(op, arg) (((arg) << 8) | (op))

//
// Byte Code
//

typedef struct _ChpegByteCode
{
    int num_defs;
    char **def_names;
    int *def_flags;
    int *def_addrs;
    int num_instructions;
    int *instructions;
    int num_strings;
    unsigned char **strings;
    int *str_len;

#if CHPEG_EXTENSION_REFS
    int num_refs;
    char **refs;
#endif

} ChpegByteCode;

CHPEG_API ChpegByteCode *ChpegByteCode_new();
CHPEG_API void ChpegByteCode_free(ChpegByteCode *self);

CHPEG_API const char *ChpegByteCode_def_name(const ChpegByteCode *self, int index);
CHPEG_API int ChpegByteCode_compare(const ChpegByteCode *a, const ChpegByteCode *b);
CHPEG_API void ChpegByteCode_print_instructions(const ChpegByteCode *self);
CHPEG_API void ChpegByteCode_print_defs(const ChpegByteCode *self);
CHPEG_API void ChpegByteCode_print(const ChpegByteCode *self);
CHPEG_API void ChpegByteCode_output_h(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes);
CHPEG_API void ChpegByteCode_output_c(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname);

// node flags
enum ChpegFlags {
    // These flags correspond to Options used in the Definition:
    CHPEG_FLAG_STOP    = 1<<0,  // {C|S} stop automatic unwrapping, forcing this node to be a container
    CHPEG_FLAG_IGNORE  = 1<<1,  // {I} deletes nodes matching this identifier
    CHPEG_FLAG_LEAF    = 1<<2,  // {L|T} collects this node and anything underneath as a final leaf (text) node
    CHPEG_FLAG_PACKRAT = 1<<3,  // {P} add node to packrat cache

#ifdef CHPEG_EXTENSIONS
    // These flags are used internally
    CHPEG_FLAG_TRIMMED_LEFT  = 1<<4, // node has been 'trimmed left': offset has been adjusted
    CHPEG_FLAG_TRIMMED_RIGHT = 1<<5, // node has been 'trimmed right': length has been adjusted
#endif

};

#endif // #ifndef CHPEG_BYTECODE_H

// } chpeg: bytecode.h

