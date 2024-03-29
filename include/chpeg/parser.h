//
// chpeg: parser.h {
//

#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

#include <stdio.h>

#ifndef CHPEG_STACK_SIZE
#define CHPEG_STACK_SIZE 2048
#endif

#ifndef CHPEG_TSTACK_SIZE
#define CHPEG_TSTACK_SIZE 256
#endif


#ifndef CHPEG_SANITY_CHECKS
#define CHPEG_SANITY_CHECKS 1
#endif

#ifndef CHPEG_DEBUG_ERRORS
#define CHPEG_DEBUG_ERRORS 0
#endif

// Set CHPEG_ERRORS to non-zero to enable error tracking. Without error
// tracking, failure is detected, but it can't tell you where an error
// occurred.
#ifndef CHPEG_ERRORS
#define CHPEG_ERRORS 1
#endif

#ifndef CHPEG_ERRORS_PRED
#define CHPEG_ERRORS_PRED 1
#endif

#ifndef CHPEG_ERRORS_IDENT
#define CHPEG_ERRORS_IDENT 1
#endif

#ifndef CHPEG_ERRORS_TERMINALS
#define CHPEG_ERRORS_TERMINALS 1
#endif

#ifndef CHPEG_ERRORS_REFS
#define CHPEG_ERRORS_REFS 1
#endif

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
#define CHPEG_PACKRAT_NODE_LIMIT 64
#endif

#if CHPEG_PACKRAT
#define CHPEG_NODE_REF_COUNT 1
#endif

// Undo action support (enable if needed as a dependency)
#if CHPEG_EXTENSION_REFS
#define CHPEG_UNDO 1
#endif

#if CHPEG_UNDO
#define CHPEG_NODE_FAIL_POP_CHILD ChpegNode_pop_child_undo
#else
#define CHPEG_NODE_FAIL_POP_CHILD ChpegNode_pop_child
#endif

//
// stack push counts
//

#if CHPEG_UNDO

#define CHPEG_CHOICE_PUSHES 3
#define CHPEG_RP_PUSHES 4
#define CHPEG_RSQ_PUSHES 3
#if CHPEG_EXTENSION_MINMAX
#define CHPEG_RMM_PUSHES 6
#endif

#else

#define CHPEG_CHOICE_PUSHES 2
#define CHPEG_RP_PUSHES 3
#define CHPEG_RSQ_PUSHES 2
#if CHPEG_EXTENSION_MINMAX
#define CHPEG_RMM_PUSHES 5
#endif

#endif

#if CHPEG_EXTENSION_TRIM
#define CHPEG_IDENT_PUSHES 6
#define CHPEG_TRIM_PUSHES 2
#else
#define CHPEG_IDENT_PUSHES 4
#endif

#if CHPEG_EXTENSION_REFS
#define CHPEG_MARK_PUSHES 2
#endif

#if CHPEG_ERRORS
  #if CHPEG_ERRORS_PRED
    #define CHPEG_PRED_PUSHES 4
  #else
    #define CHPEG_PRED_PUSHES 3
  #endif
#else
  #define CHPEG_PRED_PUSHES 2
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
    CHPEG_ERR_INVALID_IDENTIFIER = 5, // shouldn't be allowed in bytecode
    CHPEG_ERR_COMPILE = 6, // compiler error; shouldn't really be in here

    // internal errors that shouldn't happen
    CHPEG_ERR_STACK_RANGE = 7,
    CHPEG_ERR_STACK_UNDERFLOW = 8,
    CHPEG_ERR_STACK_DATA = 9,
    CHPEG_ERR_TSTACK_RANGE = 10,
    CHPEG_ERR_TSTACK_UNDERFLOW = 11,
    CHPEG_ERR_TSTACK_DATA = 12,
    CHPEG_ERR_INVALID_PC = 13,
    CHPEG_ERR_INVALID_INSTRUCTION = 14,
    CHPEG_ERR_INVALID_OFFSET = 15,
    CHPEG_ERR_RUNAWAY = 16,
    CHPEG_NUM_ERR = 17,
};

struct _ChpegParser;

#if CHPEG_EXTENSION_REFS
typedef struct _ChpegReferenceInfo
{
    size_t offset;
    size_t length;
    int flags;
} ChpegReferenceInfo;
#endif

//
// Parse/syntax tree Node
//

typedef struct _ChpegNode
{
    int def;
    int flags;

    size_t offset; // full match offset (without TRIM) / token offset (with TRIM)
    size_t length; // full match length (without TRIM) / token length (with TRIM)

#if CHPEG_EXTENSION_TRIM
    size_t match_offset; // full match offset
    size_t match_length; // full match length
#endif

    struct _ChpegNode *parent;
    struct _ChpegNode *head;
    struct _ChpegNode *next;
    int num_children;

#if CHPEG_NODE_REF_COUNT
    int ref_count;
#endif

#if CHPEG_EXTENSION_REFS
    ChpegReferenceInfo *refs;
#endif

#if CHPEG_UNDO
    struct _ChpegUndoAction *undo_action;
    int num_undo;
#endif
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

typedef void (*ChpegUndoFunction) (ChpegNode *node, void *data);
typedef struct _ChpegUndoAction
{
    struct _ChpegUndoAction *next;
    ChpegUndoFunction func;
    void *data;
    size_t data_size;
#if CHPEG_NODE_REF_COUNT
    int ref_count;
#endif
} ChpegUndoAction;


//
// ChpegParser
//

typedef struct _ChpegParser
{
    const ChpegByteCode *bc;
    ChpegNode *tree_root;
    const unsigned char *input;
    size_t length;

    int simplification; // 0=none; 1=unwrap; 2=simplify
    int parse_result;

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
    struct _ChpegPackrat *packrat;
    int packrat_enabled;
    int packrat_window_size;
#endif

} ChpegParser;

CHPEG_API ChpegParser *ChpegParser_new(const ChpegByteCode *byte_code);
CHPEG_API void ChpegParser_free(ChpegParser *self);
CHPEG_API int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed);
CHPEG_API void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input, FILE *fp);
CHPEG_API void ChpegParser_print_errors(ChpegParser *self, const unsigned char *input, int limit);
CHPEG_API void ChpegParser_print_error(ChpegParser *self, const unsigned char *input);
#if CHPEG_VM_PROFILE
CHPEG_API void ChpegParser_print_profile(ChpegParser *self,
    const unsigned char *input, size_t length, FILE *fp);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef CHPEG_PARSER_H

// } chpeg: parser.h

