//
// chpeg: parser.h {
//

#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
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

// Packrat
#ifndef CHPEG_PACKRAT
#define CHPEG_PACKRAT 0
#endif

// limit on sub-tree nodes per packrat entry (sub-trees with more nodes than this amount will not be cached)
#ifndef CHPEG_PACKRAT_NODE_LIMIT
#define CHPEG_PACKRAT_NODE_LIMIT 8
#endif

#if CHPEG_PACKRAT
#define CHPEG_NODE_REF_COUNT 1
#endif

// parser error codes
enum ChpegErrorCodes
{
    CHPEG_ERR_NONE = 0,

    // errors possible from user input
    CHPEG_ERR_PARSE_FAILED = 1,
    CHPEG_ERR_EXTRANEOUS_INPUT = 2,
    CHPEG_ERR_STACK_OVERFLOW = 3,
    CHPEG_ERR_TSTACK_OVERFLOW = 4,
    CHPEG_ERR_RSTACK_OVERFLOW = 5,
    CHPEG_ERR_INVALID_IDENTIFIER = 6, // shouldn't be allowed in bytecode
    CHPEG_ERR_COMPILE = 7, // compiler error; shouldn't really be in here

    // internal errors that shouldn't happen
    CHPEG_ERR_STACK_RANGE = 8,
    CHPEG_ERR_STACK_UNDERFLOW = 9,
    CHPEG_ERR_STACK_DATA = 10,
    CHPEG_ERR_TSTACK_RANGE = 11,
    CHPEG_ERR_TSTACK_UNDERFLOW = 12,
    CHPEG_ERR_TSTACK_DATA = 13,
    CHPEG_ERR_RSTACK_RANGE = 14,
    CHPEG_ERR_RSTACK_UNDERFLOW = 15,
    CHPEG_ERR_RSTACK_DATA = 16,
    CHPEG_ERR_INVALID_PC = 17,
    CHPEG_ERR_INVALID_INSTRUCTION = 18,
    CHPEG_ERR_INVALID_OFFSET = 19,
    CHPEG_ERR_RUNAWAY = 20,
    CHPEG_NUM_ERR = 21,
};

struct _ChpegParser;

//
// Syntax tree ChpegNode
//

typedef struct _ChpegNode
{
    size_t offset; // token offset (may be adjusted by trim '<')
    size_t length; // token length (may be adjusted by trim '>')
#ifdef CHPEG_PACKRAT
    size_t match_length; // full match length
#endif
#ifdef CHPEG_NODE_REF_COUNT
    int refs;
#endif
    int def;
    int flags;
    int num_children;
    struct _ChpegNode *head;
    struct _ChpegNode *next;
} ChpegNode;

CHPEG_API ChpegNode *ChpegNode_new(int def, size_t offset, size_t length, int flags);
CHPEG_API void ChpegNode_free(ChpegNode *self);
CHPEG_API ChpegNode *ChpegNode_push_child(ChpegNode *self, ChpegNode *other);
CHPEG_API void ChpegNode_pop_child(ChpegNode *self);
CHPEG_API ChpegNode *ChpegNode_unwrap(ChpegNode *self);
CHPEG_API void ChpegNode_print(ChpegNode *self, struct _ChpegParser *parser, const unsigned char *input, int depth, FILE *fp);

typedef struct _ChpegErrorInfo
{
    int depth;
    int def;
    int pc;
} ChpegErrorInfo;

//
// ChpegParser
//

typedef struct _ChpegParser
{
    const ChpegByteCode *bc;
    ChpegNode *tree_root;
    int max_tree_depth;
    int max_stack_size;
    int parse_result;

    /*
    size_t error_offset;
    int error_def;
    int error_parent_def;
    int error_inst;
    int error_expected;
    */

    size_t error_offset;
    ChpegErrorInfo *errors;
    int errors_capacity;
    int errors_size;

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
    size_t farthest_backtrack;
#endif

#if CHPEG_PACKRAT
    int packrat;
    int packrat_window_size;
#endif

#if CHPEG_EXTENSION_REFS
    int rstack_size;
#endif

} ChpegParser;

CHPEG_API ChpegParser *ChpegParser_new(const ChpegByteCode *byte_code);
CHPEG_API void ChpegParser_free(ChpegParser *self);
CHPEG_API int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed);
CHPEG_API void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input, FILE *fp);
CHPEG_API void ChpegParser_expected(ChpegParser *self, size_t offset, int depth, int def, int pc);
CHPEG_API void ChpegParser_print_errors(ChpegParser *self, const unsigned char *input, int all);
CHPEG_API void ChpegParser_print_error(ChpegParser *self, const unsigned char *input);
#if CHPEG_VM_PROFILE
CHPEG_API void ChpegParser_print_profile(ChpegParser *self,
    const unsigned char *input, size_t length, FILE *fp);
#endif

#endif // #ifndef CHPEG_PARSER_H

// } chpeg: parser.h

