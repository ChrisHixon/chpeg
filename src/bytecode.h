#ifndef CHPEG_BYTECODE_H
#define CHPEG_BYTECODE_H

#include <stdio.h>

#define INST(op, arg) (((arg) << 8) | (op))

//
// Byte Code
//

typedef struct _ByteCode
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
} ByteCode;

extern ByteCode *ByteCode_new();
extern void ByteCode_free(ByteCode *self);

extern int ByteCode_compare(const ByteCode *a, const ByteCode *b);
extern void ByteCode_print_instructions(const ByteCode *self);
extern void ByteCode_print_defs(const ByteCode *self);
extern void ByteCode_print(const ByteCode *self);
extern void ByteCode_output_h(const ByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes);
extern void ByteCode_output_c(const ByteCode *self, FILE *fp,
    const char *basename, const char *varname);

// rule/node flags AKA options
enum Flags {
    STOP = 1<<0,    // used - stop automatic unwrapping, forcing this node to be a container
    IGNORE = 1<<1,  // used - deletes nodes matching this identifier
    LEAF = 1<<2,    // used - collects this node and anything underneath as a final leaf (text) node
};

#endif // #ifndef CHPEG_BYTECODE_H
