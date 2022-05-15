#ifndef CHPEG_BYTECODE_H
#define CHPEG_BYTECODE_H

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
} ChpegByteCode;

extern ChpegByteCode *ChpegByteCode_new();
extern void ChpegByteCode_free(ChpegByteCode *self);

extern int ChpegByteCode_compare(const ChpegByteCode *a, const ChpegByteCode *b);
extern void ChpegByteCode_print_instructions(const ChpegByteCode *self);
extern void ChpegByteCode_print_defs(const ChpegByteCode *self);
extern void ChpegByteCode_print(const ChpegByteCode *self);
extern void ChpegByteCode_output_h(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes);
extern void ChpegByteCode_output_c(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname);

// rule/node flags AKA options
enum ChpegFlags {
    CHPEG_STOP = 1<<0,    // stop automatic unwrapping, forcing this node to be a container
    CHPEG_IGNORE = 1<<1,  // deletes nodes matching this identifier
    CHPEG_LEAF = 1<<2,    // collects this node and anything underneath as a final leaf (text) node
};

#endif // #ifndef CHPEG_BYTECODE_H
