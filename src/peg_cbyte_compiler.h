#ifndef PEG_CBYTE_COMPILER_H
#define PEG_CBYTE_COMPILER_H
//
// Grammar tree Node (temporary use during compilation)
//

typedef struct _GNode
{
    Node *node;
    int type;

    int parse_state;
    int parent_next_state;
    int parent_fail_state;

    union {
        unsigned char cval[4];
        int ival;
    } val;
    int value_len;


    int num_children;
    struct _GNode *head;
    struct _GNode *next;
} GNode;

GNode *GNode_new();
void GNode_free(GNode *self);
GNode *GNode_push_child(GNode *self, GNode *child);
void GNode_pop_child(GNode *self);

// debugging / dev aid. TODO: should disappear based on some DEBUG/NDEBUG macro
void GNode_print(GNode *self, struct _CompilationUnit *cu, const unsigned char *input, int depth);

//
// Compiler
//

typedef struct _CompilationUnit
{
    Parser *parser;
    const unsigned char *input;
    ByteCode *bc;
    GNode *root;
    int strings_allocated;
} CompilationUnit;

extern ByteCode *Compiler_compile(const unsigned char *input, int size, int *result_return, int verbose);

#endif //PEG_CBYTE_COMPILER_H
