#ifndef CHPEG_PARSER_H
#define CHPEG_PARSER_H

#include "bytecode.h"

enum ErrorCodes
{
    PARSE_FAILED = -1,
    UNKNOWN_INSTRUCTION = -2,
    RUNAWAY = -3,
    INVALID_SIZE = -4,
    STACK_OVERFLOW = -5,
    TREE_STACK_OVERFLOW = -6,
    STACK_UNDERFLOW = -7,
    TREE_STACK_UNDERFLOW = -8,
    UNEXPECTED_STACK_DATA = -9,
    UNEXPECTED_TREE_STACK_DATA = -10,
    INVALID_IDENTIFIER = -11,
};

char *esc_bytes(const unsigned char *bytes, int length, int limit);

struct _CompilationUnit;
struct _Parser;

//
// Syntax tree Node
//

typedef struct _Node
{
    int def;
    int offset;
    int length;
    int flags;
    int num_children;
    struct _Node *head;
    struct _Node *next;
} Node;

Node *Node_new(int def, int offset, int length, int flags);
void Node_free(Node *self);
Node *Node_push_child(Node *self, int def, int offset, int length, int flags);
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
    int error_offset;
    int error_def;
    int error_parent_def;
    int error_inst;
    int error_expected;

#if VM_TRACE
    int vm_trace;
#endif
#if VM_PRINT_TREE
    int vm_print_tree;
#endif

} Parser;

Parser *Parser_new(const ByteCode *byte);
void Parser_free(Parser *self);
int Parser_parse(Parser *self, const unsigned char *input, int size);
void Parser_print_tree(Parser *self, const unsigned char *input);
const char *Parser_def_name(Parser *self, int index);
void Parser_expected(Parser *self, int parent_def, int def, int inst, int offset, int expected);
void Parser_print_error(Parser *self, const unsigned char *input);

#endif // #ifndef CHPEG_PARSER_H
