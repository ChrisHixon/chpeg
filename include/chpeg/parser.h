//
// chpeg: parser.h {
//

#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#include "chpeg/bytecode.h"
#endif

#include <stdio.h>

enum ChpegErrorCodes
{
    CHPEG_ERR_NONE = 0,
    CHPEG_ERR_PARSE_FAILED = 1,
    CHPEG_ERR_EXTRANEOUS_INPUT = 2,
    CHPEG_ERR_STACK_RANGE = 3,
    CHPEG_ERR_STACK_OVERFLOW = 4,
    CHPEG_ERR_STACK_UNDERFLOW = 5,
    CHPEG_ERR_TREE_STACK_RANGE = 6,
    CHPEG_ERR_TREE_STACK_OVERFLOW = 7,
    CHPEG_ERR_TREE_STACK_UNDERFLOW = 8,
    CHPEG_ERR_UNEXPECTED_STACK_DATA = 9,
    CHPEG_ERR_UNEXPECTED_TREE_STACK_DATA = 10,
    CHPEG_ERR_INVALID_IDENTIFIER = 11,
    CHPEG_ERR_INVALID_PC = 12,
    CHPEG_ERR_INVALID_INSTRUCTION = 13,
    CHPEG_ERR_RUNAWAY = 14,
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

CHPEG_API ChpegNode *ChpegNode_new(int def, size_t offset, size_t length, int flags);
CHPEG_API void ChpegNode_free(ChpegNode *self);
CHPEG_API ChpegNode *ChpegNode_push_child(ChpegNode *self, int def, size_t offset, size_t length, int flags);
CHPEG_API void ChpegNode_pop_child(ChpegNode *self);
CHPEG_API ChpegNode *ChpegNode_unwrap(ChpegNode *self);
CHPEG_API void ChpegNode_print(ChpegNode *self, struct _ChpegParser *parser, const unsigned char *input, int depth, FILE *fp);

//
// ChpegParser
//

typedef struct _ChpegParser
{
    const ChpegByteCode *bc;
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

CHPEG_API ChpegParser *ChpegParser_new(const ChpegByteCode *byte_code);
CHPEG_API void ChpegParser_free(ChpegParser *self);
CHPEG_API int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed);
CHPEG_API void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input, FILE *fp);
CHPEG_API void ChpegParser_expected(ChpegParser *self, int parent_def, int def, int inst, size_t offset, int expected);
CHPEG_API void ChpegParser_print_error(ChpegParser *self, const unsigned char *input);

#endif // #ifndef CHPEG_PARSER_H

// } chpeg: parser.h

