#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#include "bytecode.h"

enum ChpegErrorCodes
{
    CHPEG_ERR_NONE = 0,
    CHPEG_ERR_PARSE_FAILED = 1,
    CHPEG_ERR_EXTRANEOUS_INPUT = 2,
    CHPEG_ERR_UNKNOWN_INSTRUCTION = 3,
    CHPEG_ERR_RUNAWAY = 4,
    CHPEG_ERR_INVALID_LENGTH = 5,
    CHPEG_ERR_STACK_OVERFLOW = 6,
    CHPEG_ERR_TREE_STACK_OVERFLOW = 7,
    CHPEG_ERR_STACK_UNDERFLOW = 8,
    CHPEG_ERR_TREE_STACK_UNDERFLOW = 9,
    CHPEG_ERR_UNEXPECTED_STACK_DATA = 10,
    CHPEG_ERR_UNEXPECTED_TREE_STACK_DATA = 11,
    CHPEG_ERR_INVALID_IDENTIFIER = 12,
};

struct _ChpegParser;

//
// Syntax tree ChpegNode
//

typedef struct _ChpegNode
{
    int def;
    size_t offset;
    size_t length;
    int flags;
    int num_children;
    struct _ChpegNode *head;
    struct _ChpegNode *next;
} ChpegNode;

ChpegNode *ChpegNode_new(int def, size_t offset, size_t length, int flags);
void ChpegNode_free(ChpegNode *self);
ChpegNode *ChpegNode_push_child(ChpegNode *self, int def, size_t offset, size_t length, int flags);
void ChpegNode_pop_child(ChpegNode *self);
ChpegNode *ChpegNode_unwrap(ChpegNode *self);

// debugging / dev aid. TODO: should disappear based on some DEBUG/NDEBUG macro
void ChpegNode_print(ChpegNode *self, struct _ChpegParser *parser, const unsigned char *input, int depth);

//
// ChpegParser
//

typedef struct _ChpegParser
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

    ChpegNode *tree_root;
    int max_tree_depth;
    int max_stack_size;
    size_t error_offset;
    int error_def;
    int error_parent_def;
    int error_inst;
    int error_expected;
    int parse_result;

#if VM_TRACE
    int vm_trace;
#endif
#if VM_PRINT_TREE
    int vm_print_tree;
#endif

} ChpegParser;

ChpegParser *ChpegParser_new(const ChpegByteCode *byte_code);
void ChpegParser_free(ChpegParser *self);
int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed);
void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input);
const char *ChpegParser_def_name(ChpegParser *self, int index);
void ChpegParser_expected(ChpegParser *self, int parent_def, int def, int inst, size_t offset, int expected);
void ChpegParser_print_error(ChpegParser *self, const unsigned char *input);

#endif // #ifndef CHPEG_PARSER_H
