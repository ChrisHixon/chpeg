//
// chpeg: parser.h {
//

#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/util.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

#include <stdio.h>

// CHPEG_VM_TRACE:
// Set to non-zero to compile in support for tracing parser VM instruction execution.
// To use, set parser->vm_trace to non-zero before calling ChpegParser_parse()
#ifndef CHPEG_VM_TRACE
#define CHPEG_VM_TRACE 0
#endif

// CHPEG_VM_PROFILE:
// Set to non-zero to compile in support for profiling parser VM instruction execution.
// To use, set parser->vm_profile to non-zero before calling ChpegParser_parse()
#ifndef CHPEG_VM_PROFILE
#define CHPEG_VM_PROFILE 0
#endif

// CHPEG_VM_PRINT_TREE:
// Set to non-zero to compile in support for printing the parse tree as it is being built.
// To use, set parser->vm_print_tree to non-zero before calling ChpegParser_parse()
#ifndef CHPEG_VM_PRINT_TREE
#define CHPEG_VM_PRINT_TREE 0
#endif


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

#ifdef CHPEG_DEFINITION_TRACE
    int vm_count;
    int *def_count;
    int *def_succ_count;
    int *def_fail_count;
#endif

#if CHPEG_VM_TRACE
    int vm_trace;
#endif

#if CHPEG_VM_PRINT_TREE
    int vm_print_tree;
#endif

#if CHPEG_VM_PROFILE
    int vm_profile;
    int prof_inst_cnt;
    int prof_op_cnt[CHPEG_NUM_OPS];
    int *prof_ident_cnt;
    int *prof_isucc_cnt;
    int *prof_ifail_cnt;
    int *prof_choice_cnt;
    int *prof_cisucc_cnt;
    int *prof_cifail_cnt;
#endif

} ChpegParser;

#ifndef CHPEG_PARSER_MAX_STACK_SIZE
#define CHPEG_PARSER_MAX_TREE_DEPTH 256
#define CHPEG_PARSER_MAX_STACK_SIZE (CHPEG_PARSER_MAX_TREE_DEPTH * 8)
#endif /*CHPEG_PARSER_STACK_SIZE*/

CHPEG_API ChpegParser *ChpegParser_new(const ChpegByteCode *byte_code);
CHPEG_API void ChpegParser_free(ChpegParser *self);
CHPEG_API int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed);
CHPEG_API void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input, FILE *fp);
CHPEG_API void ChpegParser_expected(ChpegParser *self, int parent_def, int def, int inst, size_t offset, int expected);
CHPEG_API void ChpegParser_print_error(ChpegParser *self, const unsigned char *input);
#if CHPEG_VM_PROFILE
CHPEG_API void ChpegParser_print_profile(ChpegParser *self, FILE *fp);
#endif

#endif // #ifndef CHPEG_PARSER_H

// } chpeg: parser.h

