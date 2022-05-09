
// rule/node flags AKA options
enum Flags {
    STOP = 1<<0,    // used - stop automatic unwrapping, forcing this node to be a container
    IGNORE = 1<<1,  // used - deletes nodes matching this identifier
    LEAF = 1<<2,    // used - collects this node and anything underneath as a final leaf (text) node
};

enum OpCodes
{
    GOTO,
    IDENT,
    ISUCC,
    IFAIL,
    RSBEG,
    RQBEG,
    CHOICE,
    CISUCC,
    CFAIL,
    CIFAIL,
    RPBEG,
    RPMAT,
    RPDONE,
    RSMAT,
    RSDONE,
    RQDONE,
    RQMAT,
    PRED,
    PMATCHF,
    PNOMATF,
    PMATCHS,
    PNOMATS,
    CHRCLS,
    LIT,
    DOT,
    SUCC,
    FAIL,
    NUM_OPS
};

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
};

#define INST(op, arg) (((arg) << 8) | (op))

unsigned char *esc_string(const unsigned char *str, int length, int trunc);

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


ByteCode *ByteCode_new();
void ByteCode_print_instructions(ByteCode *self);
void ByteCode_print_defs(ByteCode *self);
void ByteCode_print(ByteCode *self);
void ByteCode_free(ByteCode *self);
int ByteCode_compare(ByteCode *a, ByteCode *b);

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
    int lineno;
    int lineno_offset;
    int error_offset;
    int error_def;
    int error_parent_def;
    int error_inst;
    int error_expected;
} Parser;

Parser *Parser_new(const ByteCode *byte);
void Parser_free(Parser *self);
int Parser_parse(Parser *self, const unsigned char *input, int size);
void Parser_print_tree(Parser *self, const unsigned char *input);
const char *Parser_def_name(Parser *self, int index);
void Parser_expected(Parser *self, int parent_def, int def, int inst, int offset, int expected);
void Parser_print_error(Parser *self, const unsigned char *input);

