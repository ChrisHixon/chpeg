#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#include "bytecode.h"

enum ErrorCodes
{
    NO_ERROR = 0,
    PARSE_FAILED = 1,
    EXTRANEOUS_INPUT = 2,
    UNKNOWN_INSTRUCTION = 3,
    RUNAWAY = 4,
    INVALID_LENGTH = 5,
    STACK_OVERFLOW = 6,
    TREE_STACK_OVERFLOW = 7,
    STACK_UNDERFLOW = 8,
    TREE_STACK_UNDERFLOW = 9,
    UNEXPECTED_STACK_DATA = 10,
    UNEXPECTED_TREE_STACK_DATA = 11,
    INVALID_IDENTIFIER = 12,
};

struct _CompilationUnit;
struct _Parser;

//
// Syntax tree Node
//

typedef struct _Node
{
    int def;
    size_t offset;
    size_t length;
    int flags;
    int num_children;
    struct _Node *head;
    struct _Node *next;
} Node;

Node *Node_new(int def, size_t offset, size_t length, int flags);
void Node_free(Node *self);
Node *Node_push_child(Node *self, int def, size_t offset, size_t length, int flags);
void Node_pop_child(Node *self);
Node *Node_unwrap(Node *self);

// debugging / dev aid. TODO: should disappear based on some DEBUG/NDEBUG macro
void Node_print(Node *self, struct _Parser *parser, const unsigned char *input, int depth);

//
// Parser
//

typedef struct _Parser
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

    Node *tree_root;
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

} Parser;

Parser *Parser_new(const ByteCode *byte);
void Parser_free(Parser *self);
int Parser_parse(Parser *self, const unsigned char *input, size_t length, size_t *consumed);
void Parser_print_tree(Parser *self, const unsigned char *input);
const char *Parser_def_name(Parser *self, int index);
void Parser_expected(Parser *self, int parent_def, int def, int inst, size_t offset, int expected);
void Parser_print_error(Parser *self, const unsigned char *input);

#endif // #ifndef CHPEG_PARSER_H
