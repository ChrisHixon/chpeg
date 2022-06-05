//
// chpeg: parser.c {
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/util.h"
#include "chpeg/parser.h"
#endif

#ifndef CHPEG_MAX_TREE_DEPTH
#define CHPEG_MAX_TREE_DEPTH 256
#endif

#ifndef CHPEG_STACK_SIZE
#define CHPEG_STACK_SIZE 1024
#endif

#ifndef SANITY_CHECKS
#define SANITY_CHECKS 1
#endif

#ifndef DEBUG_ERRORS
#define DEBUG_ERRORS 0
#endif

// Set ERRORS to non-zero to enable error tracking. Without error tracking, errors are still detected, but it can't tell you where an error occurred.
#ifndef ERRORS
#define ERRORS 1
#endif

#ifndef ERRORS_PRED
#define ERRORS_PRED 1
#endif

#ifndef ERRORS_IDENT
#define ERRORS_IDENT 1
#endif

#ifndef ERRORS_TERMINALS
#define ERRORS_TERMINALS 1
#endif

//
// ChpegNode
//

void ChpegNode_print(ChpegNode *self, ChpegParser *parser, const unsigned char *input, int depth, FILE *fp)
{
    int flags = self->flags;
    char *data = chpeg_esc_bytes(&input[self->offset], self->length, 40);
    const char *def_name = ChpegByteCode_def_name(parser->bc, self->def);

    if (depth == 0) {
        fprintf(fp,
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
            "     "
#endif
            " offset   len     id dp flg ident \"data\"\n");
    }

    fprintf(fp,
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
        "%4d "
#endif
        "%6zu %6zu %6d %2d %s%s%s%s %*s%s \"%s\"\n",
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
        self->refs,
#endif
        self->offset,
        self->length,
        self->def,
        depth,
        flags & CHPEG_FLAG_STOP ? "S" : "-",
        flags & CHPEG_FLAG_IGNORE ? "I" : "-",
        flags & CHPEG_FLAG_LEAF ? "L" : "-",
        flags & CHPEG_FLAG_PACKRAT ? "P" : "-",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : "<NULL>"
        );

    if (data) { CHPEG_FREE(data); data = NULL; }
    for (ChpegNode *p = self->head; p; p = p->next) {
        ChpegNode_print(p, parser, input, depth + 1, fp);
    }
}

ChpegNode *ChpegNode_new(int def, size_t offset, size_t length, int flags)
{
    ChpegNode *self = (ChpegNode *)CHPEG_MALLOC(sizeof(ChpegNode));
    self->offset = offset;
    self->length = length;
#if CHPEG_PACKRAT
    self->match_length = 0;
#endif
#if CHPEG_NODE_REF_COUNT
    self->refs = 1;
#endif
    self->def = def;
    self->flags = flags;
    self->num_children = 0;
    self->head = NULL;
    self->next = NULL;
    return self;
}

ChpegNode *ChpegNode_copy(ChpegNode *other)
{
    ChpegNode *self = (ChpegNode *)CHPEG_MALLOC(sizeof(ChpegNode));
    memcpy(self, other, sizeof(ChpegNode));
#if CHPEG_NODE_REF_COUNT
    self->refs = 1;
#endif
    self->next = NULL;
    return self;
}

// free entire sub-tree recursively
void ChpegNode_free(ChpegNode *self)
{
    if (self) {
        ChpegNode *tmp;
        for (ChpegNode *p = self->head; p; p = tmp) {
            tmp = p->next;
            ChpegNode_free(p);
        }
        self->head = NULL;
#if CHPEG_NODE_REF_COUNT
        assert(self->refs > 0);
        if (--self->refs == 0) {
#endif
            CHPEG_FREE(self);
#if CHPEG_NODE_REF_COUNT
        }
#endif
    }
}

// free one node, non-recursively
void ChpegNode_free_nr(ChpegNode *self)
{
    if (self) {
#if CHPEG_NODE_REF_COUNT
        if (--self->refs == 0) {
#endif
            CHPEG_FREE(self);
#if CHPEG_NODE_REF_COUNT
        }
#endif
    }
}

ChpegNode *ChpegNode_push_child(ChpegNode *self, ChpegNode *node)
{
    node->next = self->head;
    self->head = node;
    ++(self->num_children);
    return node;
}

void ChpegNode_pop_child(ChpegNode *self)
{
    if (self->head) {
        ChpegNode *tmp = self->head;
        self->head = self->head->next;
        ChpegNode_free(tmp);
        --(self->num_children);
    }
}

// 'Unwrap' a ChpegNode, recursively removing unnecessary parent nodes containing only one child.
// In the process, this reverses the reverse node insertion used in tree building, so should only
// be called once on the tree root after a successful parse.
ChpegNode *ChpegNode_unwrap(ChpegNode *self)
{
    if (!(self->flags & (CHPEG_FLAG_STOP | CHPEG_FLAG_LEAF)) && self->num_children == 1) {
        ChpegNode *tmp = ChpegNode_unwrap(self->head);
        self->head = NULL;
        ChpegNode_free(self);
        return tmp;
    }
    ChpegNode *p = self->head; self->head = NULL;
    ChpegNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = ChpegNode_unwrap(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

#if CHPEG_PACKRAT
typedef struct _ChpegPNode
{
    ChpegNode *node;
    struct _ChpegPNode *head;
    struct _ChpegPNode *next;
} ChpegPNode;

static ChpegPNode *ChpegPNode_push_child(ChpegPNode *self, ChpegPNode *child)
{
    child->next = self->head;
    self->head = child;
    return child;
}


static int chpeg_packrat_node_count(ChpegNode *node, int *count, int depth, int limit)
{
    // 'unwrap' nodes except the top one (skip useless intermediate nodes with only one child)
    if (depth > 0) {
        while (node->num_children == 1 && !(node->flags & (CHPEG_FLAG_STOP | CHPEG_FLAG_LEAF))) {
            node = node->head;
        }
    }

    for (ChpegNode *p = node->head; p; p = p->next) {
        chpeg_packrat_node_count(p, count, depth + 1, limit);
        if (limit && *count >= limit) {
            return limit;
        }
    }
    if (limit && *count >= limit) {
        return limit;
    }
    ++(*count);
    return *count;
}


// wraps Node sub-tree in PNodes; call with depth=0
static ChpegPNode *ChpegPNode_from_node(ChpegPNode *pnode, ChpegNode *node, int depth)
{
    if (depth == 0) {
        int count = 0;
        if (chpeg_packrat_node_count(node, &count, 0, CHPEG_PACKRAT_NODE_LIMIT) >= CHPEG_PACKRAT_NODE_LIMIT) {
            return NULL;
        }
    }
    // 'unwrap' nodes except the top one (skip useless intermediate nodes with only one child)
    // this can lead to quite a performance increase by cutting down on nodes created in _node_copy()
    if (depth > 0) {
        while (node->num_children == 1 && !(node->flags & (CHPEG_FLAG_STOP | CHPEG_FLAG_LEAF))) {
            node = node->head;
        }
    }

    if (!pnode) {
        pnode = CHPEG_CALLOC(1, sizeof(ChpegPNode));
    }
    pnode->node = node;
    ++node->refs;
    for (ChpegNode *p = node->head; p; p = p->next) {
        ChpegPNode *pchild = CHPEG_CALLOC(1, sizeof(ChpegPNode));
        ChpegPNode_from_node(pchild, p, depth + 1) ;
        ChpegPNode_push_child(pnode, pchild);
    }
    return pnode;
}

static ChpegNode *ChpegPNode_node_copy(ChpegPNode *pnode)
{
    assert(pnode);
    ChpegNode *node = CHPEG_MALLOC(sizeof(ChpegNode));
    memcpy(node, pnode->node, sizeof(ChpegNode));
    node->next = NULL;
    node->head = NULL;
    node->num_children = 0;
    node->refs = 1;
    for (ChpegPNode *p = pnode->head; p; p = p->next) {
        ChpegNode *child = ChpegPNode_node_copy(p) ;
        ChpegNode_push_child(node, child);
    }
    return node;
}

static void ChpegPNode_free(ChpegPNode *self)
{
    if (self) {
        ChpegPNode *tmp;
        for (ChpegPNode *p = self->head; p; p = tmp) {
            tmp = p->next;
            ChpegPNode_free(p);
        }
        self->head = NULL;
        ChpegNode_free_nr(self->node);
        CHPEG_FREE(self);
    }
}

#if CHPEG_VM_TRACE
static void ChpegPNode_print(ChpegPNode *self, ChpegParser *parser,
    const unsigned char *input, int depth, FILE *fp)
{
    if (!self) return;

    int flags = self->node->flags;
    char *data = chpeg_esc_bytes(&input[self->node->offset], self->node->length, 40);
    const char *def_name = ChpegByteCode_def_name(parser->bc, self->node->def);

    if (depth == 0) {
        fprintf(fp, "PNODE BEG ---------------------------------- \n");
        fprintf(fp, " OFFSET   LEN     ID NC DP FLG IDENT \"DATA\"\n");
    }

    fprintf(fp, "%6zu %6zu %6d %2d %2d %s%s%s%s %*s%s \"%s\"\n",
        self->node->offset,
        self->node->length,
        self->node->def,
        self->node->num_children,
        depth,
        flags & CHPEG_FLAG_STOP ? "S" : "-",
        flags & CHPEG_FLAG_IGNORE ? "I" : "-",
        flags & CHPEG_FLAG_LEAF ? "L" : "-",
        flags & CHPEG_FLAG_PACKRAT ? "P" : "-",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : "<NULL>"
        );

    if (data) { CHPEG_FREE(data); data = NULL; }
    for (ChpegPNode *p = self->head; p; p = p->next) {
        ChpegPNode_print(p, parser, input, depth + 1, fp);
    }

    if (depth == 0) {
        fprintf(fp, "PNODE END ---------------------------------- \n");
    }
}
#endif

#endif // #if CHPEG_PACKRAT

//
// ChpegParser
//

ChpegParser *ChpegParser_new(const ChpegByteCode *bc)
{
    ChpegParser *self = (ChpegParser *)CHPEG_CALLOC(1, sizeof(ChpegParser));

    self->bc = bc;
    self->tree_root = NULL;
    self->max_tree_depth = CHPEG_MAX_TREE_DEPTH;
    self->max_stack_size = CHPEG_STACK_SIZE;
    self->error_offset = 0;
    self->errors = NULL;
    self->errors_size = 0;
    self->errors_capacity = 0;

#if CHPEG_VM_TRACE
    self->vm_trace = 0;
#endif
#if CHPEG_VM_PRINT_TREE
    self->vm_print_tree = 0;
#endif
#if CHPEG_VM_PROFILE
    self->vm_profile = 0;
    self->prof_inst_cnt = 0;
    self->prof_ident_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_isucc_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_ifail_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_choice_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_cisucc_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_cifail_cnt = CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->farthest_backtrack = 0;
#endif
#if CHPEG_PACKRAT
    self->packrat = 0;
    self->packrat_window_size = 0;
#endif

    return self;
}

void ChpegParser_free(ChpegParser *self)
{
    if (self) {
        if (self->tree_root) {
            ChpegNode_free(self->tree_root);
            self->tree_root = NULL;
        }
#if ERRORS
        if (self->errors) {
            CHPEG_FREE(self->errors);
            self->errors = NULL;
            self->errors_capacity = 0;
            self->errors_size = 0;
        }
#endif
#if CHPEG_VM_PROFILE
        if (self->prof_ident_cnt) {
            CHPEG_FREE(self->prof_ident_cnt);
            self->prof_ident_cnt = NULL;
        }
        if (self->prof_isucc_cnt) {
            CHPEG_FREE(self->prof_isucc_cnt);
            self->prof_isucc_cnt = NULL;
        }
        if (self->prof_ifail_cnt) {
            CHPEG_FREE(self->prof_ifail_cnt);
            self->prof_ifail_cnt = NULL;
        }
        if (self->prof_choice_cnt) {
            CHPEG_FREE(self->prof_choice_cnt);
            self->prof_choice_cnt = NULL;
        }
        if (self->prof_cisucc_cnt) {
            CHPEG_FREE(self->prof_cisucc_cnt);
            self->prof_cisucc_cnt = NULL;
        }
        if (self->prof_cifail_cnt) {
            CHPEG_FREE(self->prof_cifail_cnt);
            self->prof_cifail_cnt = NULL;
        }
#endif
        CHPEG_FREE(self);
    }
}

void ChpegParser_print_tree(ChpegParser *self, const unsigned char *input, FILE *fp)
{
    ChpegNode_print(self->tree_root, self, input, 0, fp);
}

#if CHPEG_VM_PROFILE
void ChpegParser_print_profile(ChpegParser *self,
    const unsigned char *input, size_t length, FILE *fp)
{
    fprintf(fp, "Instructions executed:\n");
    fprintf(fp, "%6s %8s %11s %6s\n", "OP-", "opcode", "count", "%");
    for (int i = 0; i < CHPEG_NUM_OPS; ++i) {
        fprintf(fp, "%6s %8s %11d %6.2f\n", "OP ",
            Chpeg_op_name(i), self->prof_op_cnt[i],
            100.0 * (float)self->prof_op_cnt[i] / (float)self->prof_inst_cnt);
    }
    fprintf(fp, "%6s %8s %11d %6.2f\n", "OP=", "Total", self->prof_inst_cnt, 100.0);

    int total_ident = self->prof_op_cnt[CHPEG_OP_IDENT];
    int total_isucc = self->prof_op_cnt[CHPEG_OP_ISUCC];
    int total_ifail = self->prof_op_cnt[CHPEG_OP_IFAIL];

    fprintf(fp, "\n");
    fprintf(fp, "Definition identifier calls:\n");
    fprintf(fp, "  DEF-   id       IDENT      %%       ISUCC       IFAIL  name\n");
    for (int i = 0; i < self->bc->num_defs; ++i) {
        fprintf(fp, "%6s %4d %11d %6.2f %11d %11d  %s\n", "DEF ", i,
            self->prof_ident_cnt[i],
            100.0 * (float)self->prof_ident_cnt[i] / (float)total_ident,
            self->prof_isucc_cnt[i],
            self->prof_ifail_cnt[i],
            ChpegByteCode_def_name(self->bc, i));
    }
    fprintf(fp, "%6s %4s %11d %6.2f %11d %11d  %s\n", "DEF=", "--",
        total_ident, 100.0, total_isucc, total_ifail, "--");

    int total_choice = self->prof_op_cnt[CHPEG_OP_CHOICE];
    int total_cisucc = self->prof_op_cnt[CHPEG_OP_CISUCC];
    int total_cifail = self->prof_op_cnt[CHPEG_OP_CIFAIL];

    fprintf(fp, "\n");
    fprintf(fp, "Choice calls per definition:\n");
    fprintf(fp, "  CHOICE-  def      CHOICE      %%      CISUCC      CIFAIL  def_name\n");
    for (int i = 0; i < self->bc->num_defs; ++i) {
        fprintf(fp, "%9s %4d %11d %6.2f %11d %11d  %s\n", "CHOICE ", i,
            self->prof_choice_cnt[i],
            100.0 * (float)self->prof_choice_cnt[i] / (float)total_choice,
            self->prof_cisucc_cnt[i],
            self->prof_cifail_cnt[i],
            ChpegByteCode_def_name(self->bc, i));
    }
    fprintf(fp, "%9s %4s %11d %6.2f %11d %11d  %s\n", "CHOICE=", "--",
        total_choice, 100.0, total_cisucc, total_cifail, "--");

    fprintf(fp, "\nFarthest backtrack: %zu\n", self->farthest_backtrack);

    fprintf(fp, "\nInstructions executed: %d, instructions per input byte: %.2f\n",
        self->prof_inst_cnt, (float)self->prof_inst_cnt / (float)length);
}
#endif

void ChpegParser_expected(ChpegParser *self, size_t offset, int depth, int def, int pc)
{
    if (offset >= self->error_offset) {
        if (offset > self->error_offset) {
            self->error_offset = offset;
            self->errors_size = 0;
        }
        if (self->errors_size + 1 > self->errors_capacity) {
            if (!self->errors) {
                self->errors_capacity = 1;
                self->errors = CHPEG_MALLOC(self->errors_capacity * sizeof(ChpegErrorInfo));
            }
            else {
                self->errors_capacity *= 2;
            }
            self->errors = CHPEG_REALLOC(self->errors, self->errors_capacity * sizeof(ChpegErrorInfo));
        }
        self->errors[self->errors_size++] = (ChpegErrorInfo)
        {
            .depth = depth,
            .def = def,
            .pc = pc,
        };
    }
}

void ChpegParser_print_errors(ChpegParser *self, const unsigned char *input, int all)
{
#if ERRORS
    if (self->errors_size == 0) {
        fprintf(stderr, "No errors detected / tracked.\n");
        return;
    }

    assert(self->errors_size > 0);
    for (int i = all ? 0 : self->errors_size - 1; i < self->errors_size; ++i) {
        ChpegErrorInfo *error = &self->errors[i];
        const char *def_name = ChpegByteCode_def_name(self->bc, error->def);
        int inst = self->bc->instructions[error->pc];
        int op = inst & 0xff;
        int arg = inst >> 8;

        char *esc = NULL;
        const char *str = NULL;
        int depth_indent = (error->depth - 1) *2;
        const char *expected = "Expected:";

        // if a predicate is followed by an instruction case we can display,
        // use that, switching to 'unexpected' for PREDN (predicate '!')
        // (otherwise, it'll fall through to the default 'syntax error' case below)
        if (op == CHPEG_OP_PREDN || op == CHPEG_OP_PREDA) {
            int next_inst = self->bc->instructions[error->pc + 1];
            switch (next_inst & 0xff) {
                case CHPEG_OP_IDENT:
                case CHPEG_OP_LIT:
                case CHPEG_OP_CHRCLS:
                case CHPEG_OP_DOT:
                    if (op == CHPEG_OP_PREDN) {
                        expected = "Unexpected:";
                    }
                    op = next_inst & 0xff;
                    arg = next_inst >> 8;
                    break;
            }
        }

        switch (op) {
            case CHPEG_OP_IDENT:
                str = ChpegByteCode_def_name(self->bc, arg);
                fprintf(stderr, "%11s %*s%s in %s at offset %lu\n",
                    expected,
                    depth_indent, "",
                    str ? str : "<N/A>>",
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
                break;
            case CHPEG_OP_LIT:
                esc = chpeg_esc_bytes((unsigned char *)self->bc->strings[arg],
                    self->bc->str_len[arg], 20);
                fprintf(stderr, "%11s %*s\"%s\" in %s at offset %lu\n",
                    expected,
                    depth_indent, "",
                    esc ? esc : "<NULL>",
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
                CHPEG_FREE(esc); esc = NULL;
                break;
            case CHPEG_OP_CHRCLS:
                esc = chpeg_esc_bytes((unsigned char *)self->bc->strings[arg],
                    self->bc->str_len[arg], 20);
                fprintf(stderr, "%11s %*s[%s] in %s at offset %lu\n",
                    expected,
                    depth_indent, "",
                    esc ? esc : "<NULL>",
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
                CHPEG_FREE(esc); esc = NULL;
                break;
            case CHPEG_OP_DOT:
                fprintf(stderr, "%11s %*scharacter in %s at offset %lu\n",
                    expected,
                    depth_indent, "",
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
                break;
            default: // unhandled op, show instruction in <> for debugging
                fprintf(stderr, "%11s %*sSyntax error in %s at offset %lu <%s,%d>\n",
                    "Error:",
                    depth_indent, "",
                    def_name ? def_name : "<N/A>",
                    self->error_offset,
                    Chpeg_op_name(op), arg);
                break;
        }
    }
#else
    fprintf(stderr, "Error tracking disabled at compile time.\n");
#endif
}

void ChpegParser_print_error(ChpegParser *self, const unsigned char *input)
{
    ChpegParser_print_errors(self, input, 0);
}

// TODO: work on check sanity checks and underflow/overflow/range checks, make macros to make it easier
//
// SANITY_CHECKS should only contain checks for conditions that should never
// occur, and if they do occur, it's a bug/programming error. It shouldn't
// cover any cases that can be triggered by user input (grammar or input data)
// Same idea for any assert() in the code.
//
// SANITY_CHECKS and assert() are enabled by default, but this may change
// later. Stack overflow (either stack) can happen due to user input (grammar
// and/or input), so checks for that should never be removed (don't include
// within SANITY_CHECKS)
//
// Things that should be checked for in SANITY_CHECKS:
// - Stack underflow
// - Accessing anything out of range (stack, arrays)
//
// These are things that should not happen, and if they do it is a
// programming/bytecode error. We kind of have to trust the bytecode is
// correct. If there are errors in bytecode, then there are bugs in the
// compiler. If the user is creating their own bytecode, it's on them to
// make sure it's correct.
//
// TODO: cnt_max/RUNAWAY may not fit SANITY_CHECKS rules (pondering how to deal
// with things this is meant to detect)

int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed)
{

#define CHPEG_CHECK_STACK_OVERFLOW(size) (top + (size) >= max_stack_size)
#define CHPEG_CHECK_STACK_UNDERFLOW(size) (top - (size) < 0)
#define CHPEG_CHECK_TSTACK_OVERFLOW(size) (tree_top + (size) >= max_tree_depth)
#define CHPEG_CHECK_TSTACK_UNDERFLOW(size) (tree_top - (size) < 0)

    int locked = 0, cur_def = -1, retval = 0;

#if ERRORS
    int err_locked = 0;
#endif

#if CHPEG_VM_PRINT_TREE
    int tree_changed = 0;
#endif

#if CHPEG_PACKRAT
    const int num_defs = self->bc->num_defs;
#endif
    //const char **def_names = (const char **)self->bc->def_names;
    const int *def_flags = self->bc->def_flags;
    const int *def_addrs = self->bc->def_addrs;
    const int *instructions = self->bc->instructions;
    //const int num_strings = self->bc->num_strings;
    const unsigned char **strings = (const unsigned char **)self->bc->strings;
    const int *str_len = self->bc->str_len;

    const int max_stack_size = self->max_stack_size;
    const int max_tree_depth = self->max_tree_depth;

    int i = 0;

#if CHPEG_PACKRAT
    ChpegPNode **packrat = NULL;
    ChpegPNode *packrat_no_match = NULL;
    ChpegPNode *packrat_lookup = NULL;
    int packrat_index = 0;
    int packrat_flags = 0;
    int window_size = 0;
    size_t window_end = 0;
    int poffset = 0, plen = 0, loop_end = 0;

    if (self->packrat) {
        if (self->packrat_window_size) {
            window_size = self->packrat_window_size;
            window_end = window_size - 1;
        }
        else {
            window_size = length + 1;
            window_end = length + 1;
        }
        packrat = CHPEG_CALLOC(num_defs * window_size, sizeof(ChpegPNode **));
        packrat_no_match = CHPEG_MALLOC(0);
        assert(packrat_no_match != NULL);
    }

    for (i = 0; i < num_defs; ++i) {
        if (def_flags[i] & CHPEG_FLAG_PACKRAT) {
            packrat_flags = 1;
            break;
        }
    }

#endif

#if CHPEG_VM_TRACE
    char *esc_str = NULL;
    const char *def_name = NULL;
#endif

    size_t offset = 0;
    int op = 0, arg = 0, pc = 0, top = 0, tree_top = 0;

    size_t stack[max_stack_size];
    ChpegNode *tree_stack[max_tree_depth];

    if (CHPEG_CHECK_STACK_OVERFLOW(1)) {
        retval = CHPEG_ERR_STACK_OVERFLOW;
        goto done;
    }
    stack[top] = 0; // to make life easier, put a zero on the stack;
                    // top is then always in range [0, max_stack_size)
                    // top is incremented before push, so first real element is stack[1]

    if (CHPEG_CHECK_TSTACK_OVERFLOW(1)) {
        retval = CHPEG_ERR_TREE_STACK_OVERFLOW;
        goto done;
    }
    self->tree_root = ChpegNode_new(0, 0, 0, 0);
    tree_stack[tree_top] = self->tree_root;

#if CHPEG_VM_PROFILE
    self->prof_inst_cnt = 0;
    memset(self->prof_op_cnt, 0, CHPEG_NUM_OPS * sizeof(int));
    memset(self->prof_ident_cnt, 0, self->bc->num_defs * sizeof(int));
    memset(self->prof_isucc_cnt, 0, self->bc->num_defs * sizeof(int));
    memset(self->prof_ifail_cnt, 0, self->bc->num_defs * sizeof(int));
    memset(self->prof_choice_cnt, 0, self->bc->num_defs * sizeof(int));
    memset(self->prof_cisucc_cnt, 0, self->bc->num_defs * sizeof(int));
    memset(self->prof_cifail_cnt, 0, self->bc->num_defs * sizeof(int));
    self->farthest_backtrack = 0;
    size_t max_offset = 0;
#endif

#if SANITY_CHECKS || CHPEG_VM_TRACE
    unsigned long long cnt = 0, cnt_max = 0;
    const int num_instructions = self->bc->num_instructions;
    cnt_max = (length <= 2642245) ? (length < 128 ? 2097152 : length * length * length) : (unsigned long long)-1LL;
    for(cnt = 0; cnt < cnt_max; ++cnt)
#else
    for(;;)
#endif
    {

#if SANITY_CHECKS
        if (pc < 0 || pc > num_instructions) {
            retval = CHPEG_ERR_INVALID_PC;
            goto done;
        }
        if (top >= max_stack_size) {
            retval = CHPEG_ERR_STACK_RANGE;
            goto done;
        }
        if (top < 0) {
            retval = CHPEG_ERR_STACK_RANGE;
            goto done;
        }
        if (tree_top >= max_tree_depth) {
            retval = CHPEG_ERR_TREE_STACK_RANGE;
            goto done;
        }
        if (tree_top < 0) {
            retval = CHPEG_ERR_TREE_STACK_RANGE;
            goto done;
        }
        if (offset > length) {
            retval = CHPEG_ERR_INVALID_OFFSET;
            goto done;
        }
#endif
        op = instructions[pc] & 0xff;
        arg = instructions[pc] >> 8;

#if CHPEG_VM_PROFILE
        if (self->vm_profile) {
            self->prof_inst_cnt++;
            self->prof_op_cnt[op]++;
            if (offset > max_offset) {
                max_offset = offset;
            }
            if (max_offset - offset > self->farthest_backtrack) {
                self->farthest_backtrack = max_offset - offset;
            }
        }
#endif

#if CHPEG_VM_TRACE
        if (self->vm_trace & 1) {
            if (cnt == 0) {
                fprintf(stderr, "=     CNT   OFFSET       PC           OP   ARG\n");
            }
            switch (op) {
                case CHPEG_OP_IDENT:
                case CHPEG_OP_ISUCC:
                case CHPEG_OP_IFAIL:
#ifdef CHPEG_EXTENSIONS
                case CHPEG_OP_TRIM:
#endif
                    def_name = ChpegByteCode_def_name(self->bc, arg);
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s%s\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "",
                        def_name ? def_name : "<INVALID>");
                    def_name = NULL;
                    break;
                case CHPEG_OP_LIT:
                case CHPEG_OP_CHRCLS:
                    esc_str = chpeg_esc_bytes(
                        strings[arg], str_len[arg], 28);
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s\"%s\"\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "",
                        esc_str ? esc_str : "<NULL>");
                    if (esc_str) { CHPEG_FREE(esc_str); esc_str = NULL; }
                    break;
                default:
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s-\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "");
            }
        }
#endif

#if CHPEG_VM_PRINT_TREE
        tree_changed = 0;
#endif

        // Note: don't return from inside this switch, set pc = -1, retval = return value, then break;
        // This is to allow finalization like tree printing (if enabled)

        switch (op) {
            case CHPEG_OP_GOTO: // arg = addr; GOTO addr; pc is set to addr
                pc = arg; break;

// Identifier
            case CHPEG_OP_IDENT: // arg = def; Identifier "call"
                                 // on success, next instruction is skipped (See ISUCC, IFAIL)
#if SANITY_CHECKS
                if (arg < 0) {
                    pc = -1; retval = CHPEG_ERR_INVALID_IDENTIFIER; break;
                }
#endif

#if CHPEG_VM_PROFILE
                ++self->prof_ident_cnt[arg];
#endif

#if CHPEG_PACKRAT
                if (self->packrat) {
                    if (offset > window_end) {
                        if (offset - window_end >= window_size) {
#if CHPEG_VM_TRACE
                            if (self->vm_trace & 2) {
                                fprintf(stderr, "packrat MOVE WINDOW END from %zu to %zu "
                                    "(CLEAR ENTIRE WINDOW)\n", window_end, offset);
                            }
#endif
                            loop_end = window_size * num_defs;
                            for (i = 0; i < loop_end; ++i) {
                                ChpegPNode *pnode = packrat[i];
                                if (pnode && pnode != packrat_no_match) {
                                    ChpegPNode_free(pnode);
                                }
                            }
                            memset(packrat, 0, window_size * num_defs * sizeof(ChpegPNode **));
                        }
                        else {
#if CHPEG_VM_TRACE
                            if (self->vm_trace & 2) {
                                fprintf(stderr, "packrat MOVE WINDOW END from %zu to %zu "
                                    "(CLEAR WINDOW from %zu to %zu)\n",
                                    window_end, offset,
                                    (window_end + 1) % window_size, offset % window_size);
                            }
#endif
                            loop_end = (offset + 1) * num_defs;
                            for (i = (window_end + 1) * num_defs; i < loop_end; ++i) {
                                packrat_index = i % (window_size * num_defs);
                                ChpegPNode *pnode = packrat[packrat_index];
                                if (pnode && pnode != packrat_no_match) {
                                    ChpegPNode_free(pnode);
                                }
                                packrat[packrat_index] = NULL;
                            }
                        }
                        window_end = offset;
                    }
                    if (offset >= window_end - window_size + 1 && offset <= window_end) {
                        packrat_index = ((int)offset % window_size) * num_defs + arg;
                        packrat_lookup = packrat[packrat_index];
                    }
                    else {
                        packrat_lookup = NULL;
                    }

#if CHPEG_VM_TRACE
                    if (self->vm_trace & 2) {
                        if (packrat_lookup && packrat_lookup != packrat_no_match) {
                            esc_str = chpeg_esc_bytes(input + offset,
                                packrat_lookup->node->match_length, 40);
                        }
                        fprintf(stderr, "packrat IDENT %6d %4d %12s %s%s%s%s\n",
                            (int)offset, arg,
                            ChpegByteCode_def_name(self->bc, arg),
                            packrat_lookup == NULL ? "<NOT FOUND>" :
                            (packrat_lookup == packrat_no_match ? "<FAIL>" : "<FOUND>"),
                            esc_str ? " \"" : "",
                            esc_str ? esc_str : "",
                            esc_str ? "\"" : ""
                            );
                        if (esc_str) { CHPEG_FREE(esc_str); esc_str = NULL; }
                    }
#endif
                    if (packrat_lookup) {
                        if (packrat_lookup != packrat_no_match) {
                            if (!locked) {
                                if ((packrat_lookup->node->flags & CHPEG_FLAG_IGNORE) == 0) {
                                    ChpegNode_push_child(tree_stack[tree_top],
                                        ChpegPNode_node_copy(packrat_lookup));
                                }
#if CHPEG_VM_PRINT_TREE
                                tree_changed = 1;
#endif
                            }
                            offset += packrat_lookup->node->match_length;
                            pc += 2;
                            break;
                        }
                        else {
                            ++pc;
                            break;
                        }
                    }
                }
#endif // #if CHPEG_PACKRAT

                // top=s+0: stack at beginning of call
                if (CHPEG_CHECK_STACK_OVERFLOW(4)) { // pushes 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                if (!locked) {
                    if (CHPEG_CHECK_TSTACK_OVERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TREE_STACK_OVERFLOW; break;
                    }
                    if (def_flags[arg] & (CHPEG_FLAG_LEAF | CHPEG_FLAG_IGNORE)) {
                        stack[++top] = 1; locked = 1; // top=s+1: locked
                    }
                    else {
                        stack[++top] = 0; // top=s+1: locked
                    }
                    tree_stack[tree_top+1] = ChpegNode_push_child(tree_stack[tree_top],
                        ChpegNode_new(arg, offset, 0, def_flags[arg]));
                    ++tree_top;
                }
                else {
                    stack[++top] = 0; // top=s+1: locked
                }

                stack[++top] = cur_def; // top=s+2: def
                cur_def = arg;

                stack[++top] = offset; // top=s+3: offset
                stack[++top] = pc; // top=s+4: pc

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif

                pc = def_addrs[arg];
                break;

            case CHPEG_OP_ISUCC: // arg = def; Identifier "call" match success, "return"
                                 // pc restored to pc+2, skipping next instruction
#if SANITY_CHECKS
                assert(cur_def == arg);
                if (CHPEG_CHECK_STACK_UNDERFLOW(4)) { // pops 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if CHPEG_VM_PROFILE
                ++self->prof_isucc_cnt[arg];
#endif
                // top=s+4: pc
                pc = stack[top--] + 2; // top=s+3: offset

#if CHPEG_PACKRAT
                poffset = stack[top--]; // top=s+2: def
                plen = offset - poffset;
#else
                --top; // top=s+2: def
#endif

                cur_def = stack[top--]; // top=s+1: locked

                if (stack[top--] == 1) { locked = 0; } // top=s+0: done popping stack
                if (!locked) {

#if SANITY_CHECKS
                    if (CHPEG_CHECK_TSTACK_UNDERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TREE_STACK_UNDERFLOW; break;
                    }
#endif

#ifdef CHPEG_EXTENSIONS
                    // if we haven't set length by trimming right, set length
                    if ((tree_stack[tree_top]->flags & CHPEG_FLAG_TRIMMED_RIGHT) == 0)
                    {
#endif
                        tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
#ifdef CHPEG_EXTENSIONS
                    }
#endif

#if CHPEG_PACKRAT
                    if (self->packrat) {
                        tree_stack[tree_top]->match_length = plen;
                        packrat_index = !packrat_flags || (def_flags[arg] & CHPEG_FLAG_PACKRAT);
#if CHPEG_VM_TRACE
                        if (self->vm_trace & 2) {
                            esc_str = chpeg_esc_bytes(input + poffset, plen, 40);
                            fprintf(stderr, "packrat ISUCC %6d %4d %12s \"%s\" %s\n",
                                poffset, arg,
                                ChpegByteCode_def_name(self->bc, arg), esc_str,
                                packrat_index ? "CACHE" : "NOCACHE");
                            if (esc_str) { CHPEG_FREE(esc_str); esc_str = NULL; }
                        }
#endif

                        if (packrat_index &&
                            poffset >= window_end - window_size + 1 && poffset <= window_end)
                        {
                            packrat_index = (poffset % window_size) * num_defs + arg;
                            packrat[packrat_index] = ChpegPNode_from_node(
                                NULL, tree_stack[tree_top], 0);
#if CHPEG_VM_TRACE
                            if (self->vm_trace & 4) {
                                ChpegPNode_print(packrat[packrat_index],
                                    self, input, 0, stderr);
                            }
#endif
                        }

                    }
#endif

                    --tree_top;
                    if (def_flags[arg] & CHPEG_FLAG_IGNORE) {
                        ChpegNode_pop_child(tree_stack[tree_top]);
                    }
                }


#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                // pc is restored +2 (next instruction skipped)
                break;

            case CHPEG_OP_IFAIL: // Identifier "call" match failure, "return"
                                 // pc restored +1 (next instruction not skipped)
#if SANITY_CHECKS
                assert(cur_def == arg);
                if (CHPEG_CHECK_STACK_UNDERFLOW(4)) { // pops 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if CHPEG_VM_PROFILE
                ++self->prof_ifail_cnt[arg];
#endif
                // top=s+4: pc
                pc = stack[top--]; // top=s+3: offset
                offset = stack[top--]; // top=s+2: def       // backtrack
                cur_def = stack[top--]; // top=s+1: locked

#if CHPEG_PACKRAT
                if (self->packrat) {
#if CHPEG_VM_TRACE
                    if (self->vm_trace & 2) {
                        fprintf(stderr, "packrat IFAIL %6d %4d %12s %s\n",
                            (int)offset, arg,
                            ChpegByteCode_def_name(self->bc, arg), "<RECORD FAIL>");
                    }
#endif
                    if (offset >= window_end - window_size + 1 && offset <= window_end) {
                        packrat_index = ((int)offset % window_size) * num_defs + arg;
                        packrat[packrat_index] = packrat_no_match;
                    }

                }
#endif

#if ERRORS && ERRORS_IDENT
                if (!err_locked) { // Tracking errors here is probably bare minimum of usefulness
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_top > 0 ? tree_stack[tree_top-1]->def : -1, pc); // parent def, pc
#if DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif

                if (stack[top--] == 1) { locked = 0; } // top=s+0 (done popping)
                if (!locked) {
#if SANITY_CHECKS
                    if (CHPEG_CHECK_TSTACK_UNDERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TREE_STACK_UNDERFLOW; break;
                    }
#endif
                    ChpegNode_pop_child(tree_stack[--tree_top]);
                }

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                // pc has been restored, resume execution at next instruction
                ++pc;
                break;

// Choice
            case CHPEG_OP_CHOICE:
                if (CHPEG_CHECK_STACK_OVERFLOW(2)) { // pushes 2 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
#if CHPEG_VM_PROFILE
                ++self->prof_choice_cnt[cur_def];
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                ++pc; // next instruction
                break;

            case CHPEG_OP_CISUCC: // arg = success/fail pc addr
#if CHPEG_VM_PROFILE
                ++self->prof_cisucc_cnt[cur_def];
#endif
                // fallthrough
            case CHPEG_OP_CFAIL:
#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(2)) { // pops 2 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                top -= 2;
                pc = arg;
                break;

            case CHPEG_OP_CIFAIL:
#if CHPEG_VM_PROFILE
                ++self->prof_cifail_cnt[cur_def];
#endif
                // backtrack
                offset = stack[top];
                for (i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    ChpegNode_pop_child(tree_stack[tree_top]);
#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                ++pc; // next instruction
                break;


//
// Repeat +
//
            case CHPEG_OP_RPBEG: // Repeat Plus BEGin (+)
                if (CHPEG_CHECK_STACK_OVERFLOW(3)) { // pushes 3 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children: backtrack point
                stack[++top] = offset; // offset: backtrack point
                stack[++top] = 0; // cnt (match count)
                ++pc; // next instruction
                break;

            case CHPEG_OP_RPMAT: // Repeat Plus MATch; arg = loop pc addr
                ++stack[top]; // incr match count
                if (offset != stack[top-1]) { // only loop if consuming
                    stack[top-2] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top-1] = offset; // update backtrack point
                    pc = arg; // continue looping
                }
                else {
                    ++pc; // next instruction
                }
                break;

            case CHPEG_OP_RPDONE: // Repeat Plus DONE; arg = match fail pc addr
#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(3)) { // pops 3 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                // backtrack to point where match failed
                offset = stack[top-1];
                for (i = tree_stack[tree_top]->num_children - stack[top-2]; i > 0; --i)
                    ChpegNode_pop_child(tree_stack[tree_top]);
                if (stack[top] > 0) { // op+ SUCCESS
                    top -= 3;
                    ++pc; // next instruction
                }
                else { // op+ FAIL
                    top -= 3;
                    pc = arg;
                }

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// Repeat *|?
            case CHPEG_OP_RSBEG: // Repeat Star BEGin (*); arg=unused
            case CHPEG_OP_RQBEG: // Repeat Question BEGin (?); arg=unused
                if (CHPEG_CHECK_STACK_OVERFLOW(2)) { // pushes 2 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children: backtrack point
                stack[++top] = offset; // save offset for backtrack
                ++pc; // next instruction
                break;

            case CHPEG_OP_RSMAT: // Repeat Star MATch; arg = loop pc addr
                if (offset != stack[top]) { // only loop if consuming
                    stack[top-1] = tree_stack[tree_top]->num_children; // update num_children backtrack point
                    stack[top] = offset; // update offset backtrack point
                    pc = arg; // continue looping
                }
                else {
                    ++pc; // next instruction
                }
                break;

            case CHPEG_OP_RSDONE: // Repeat Star DONE (*): always succeeds, proceeds to next instr.
            case CHPEG_OP_RQDONE: // Repeat Question DONE (?): always succeeds, proceeds to next instr.
#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(2)) { // pops 2 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                // backtrack to point where match failed
                offset = stack[top];
                for (i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    ChpegNode_pop_child(tree_stack[tree_top]);
                top -= 2;

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                ++pc; // next instruction
                break;

// Repeat ?
            case CHPEG_OP_RQMAT: // Repeat Question MATch (?); no looping for ?
                stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                stack[top] = offset; // update backtrack point
                ++pc; // next instruction
                break;

#ifdef CHPEG_EXTENSIONS
//
// Trim
//
            case CHPEG_OP_TRIM: // TRIM start ('<')
                                // arg: unused

                if (CHPEG_CHECK_STACK_OVERFLOW(4)) { // pushes 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                // Save original offset and flags
                stack[++top] = tree_stack[tree_top]->offset;
                stack[++top] = tree_stack[tree_top]->flags;

                // Trim on left (set offset and mark as 'trimmed left')
                tree_stack[tree_top]->offset = offset;
                tree_stack[tree_top]->flags |= CHPEG_FLAG_TRIMMED_LEFT;

                // Save changed offset and flags on the stack
                stack[++top] = tree_stack[tree_top]->offset;
                stack[++top] = tree_stack[tree_top]->flags;

                ++pc; // next instruction
                break;

            case CHPEG_OP_TRIMS: // TRIM Success ('>'): the contents inside ('<' ... '>') matched
                                 // arg = next pc

#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(4)) { // pops 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif

                // Restore the changed flags and offset values from the stack
                tree_stack[tree_top]->flags = stack[top--];
                tree_stack[tree_top]->offset = stack[top--];

                // Trim of right (set length and mark as 'trimmed right')
                tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                tree_stack[tree_top]->flags |= CHPEG_FLAG_TRIMMED_RIGHT;

                // Discard the original flags and offset values saved on stack
                top -= 2;

                pc = arg;
                break;

            case CHPEG_OP_TRIMF: // TRIM Failed ('>'): the contents inside ('<' ... '>') did not match
                                 // arg = next pc

#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(4)) { // pops 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif

                // Discard the changed flags and offset values saved on stack
                top -= 2;

                // Restore the original flags and offset values from the stack
                tree_stack[tree_top]->flags = stack[top--];
                tree_stack[tree_top]->offset = stack[top--];

                pc = arg;
                break;
#endif

// Predicate
            case CHPEG_OP_PREDA:
            case CHPEG_OP_PREDN:
                // Predicate begin, should be followed with child instructions,
                // then PMATCH{S,F}, then PNOMAT{S,F}, depending on op (&,!)
                if (CHPEG_CHECK_STACK_OVERFLOW(4)) { // pushes 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
#if ERRORS
#if ERRORS_PRED
                //stack[++top] = pc + 1; // 1st child inst address for error
                stack[++top] = pc; // save pc for error
#endif
                if (!err_locked) {
                    stack[++top] = 1; err_locked = 1;
                }
                else {
                    stack[++top] = 0;
                }
#endif
                ++pc; // next instruction
                break;

            case CHPEG_OP_PMATCHF: // Predicate matched, match is considered failure, arg = failure address
            case CHPEG_OP_PNOMATF: // Predicate not matched, not match is considered failure, arg = failure address
#if ERRORS && ERRORS_PRED
                if (stack[top]) {
                    ChpegParser_expected(self, stack[top-2], tree_top, // offset, depth
                        tree_stack[tree_top]->def, stack[top-1]); // def, pc
#if DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                goto pred_cleanup;

            case CHPEG_OP_PMATCHS: // Predicate matched, match is considered success; next instruction skipped
                pc += 2;
                goto pred_cleanup;

            case CHPEG_OP_PNOMATS: // Predicate not matched, not match is considered success
                ++pc; // next instruction

                // pc is expected to be set when hitting pred_cleanup (and thus should not change)
pred_cleanup:
#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(4)) { // pops 4 items
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if ERRORS
                if (stack[top--]) {
                    err_locked = 0;
                }
#if ERRORS_PRED
                --top;
#endif
#endif
                // backtrack
                offset = stack[top--]; // restore saved offset (don't consume)
                for (i = tree_stack[tree_top]->num_children - stack[top--]; i > 0; --i)
                    ChpegNode_pop_child(tree_stack[tree_top]);
#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// CharClass
            case CHPEG_OP_CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < length) {
                        int mlen = str_len[arg], i;
                        const unsigned char *mstr = strings[arg];
                        for (i = 0; i < mlen; ++i) {
                            if (mstr[i] == input[offset]) {
                                ++offset;
                                pc += 2;
                                goto chrcls_done; // need to break two levels
                            }
                            if ((i < mlen - 2) && (mstr[i+1] == '-')) {
                                if ((input[offset] >= mstr[i]) && (input[offset] <= mstr[i+2])) {
                                    ++offset;
                                    pc += 2;
                                    goto chrcls_done; // need to break two levels
                                }
                                i+=2;
                            }
                        }
                    }
#if ERRORS && ERRORS_TERMINALS
                    if (!err_locked) {
                        ChpegParser_expected(self, offset, tree_top, // offset, depth
                            tree_stack[tree_top]->def, pc); // def, pc
#if DEBUG_ERRORS
                        ChpegParser_print_error(self, input);
#endif
                    }
#endif
                    ++pc; // next instruction (upon failure)
                }
chrcls_done:
                break;

// Literal
            case CHPEG_OP_LIT: // arg = str_id; match literal string; skip next instruction on match
                {
                    int len = str_len[arg];
                    if ((offset < (length - (len - 1))) && !memcmp(&input[offset], strings[arg], len)) {
                        offset += len;
                        pc += 2;
                        break;
                    }
                }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_stack[tree_top]->def, pc); // def, pc
#if DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif
                ++pc; // next instruction
                break;

// Dot
            case CHPEG_OP_DOT: // arg = fail addr; match any char; goto addr on failure
                if (offset < length) {
                    offset++;
                    ++pc; // next instruction
                    break;
                }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_stack[tree_top]->def, pc); // def, pc
#if DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                break;

// End
            case CHPEG_OP_SUCC: // overall success
                pc = -1; // we're done

#ifdef CHPEG_EXTENSIONS
                // if we haven't set length by trimming right, set length
                if ((tree_stack[tree_top]->flags & CHPEG_FLAG_TRIMMED_RIGHT) == 0)
                {
#endif
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
#ifdef CHPEG_EXTENSIONS
                }
#endif

                // clean up the parse tree, reversing the reverse node insertion in the process
                self->tree_root = ChpegNode_unwrap(self->tree_root);
#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif

                if (top != 0) {
                    retval = CHPEG_ERR_UNEXPECTED_STACK_DATA;
                }
                else if (tree_top != 0) {
                    retval = CHPEG_ERR_UNEXPECTED_TREE_STACK_DATA;
                }
                else {
                    if (consumed != NULL) {
                        *consumed = offset;
                    }
                    if (offset == length) {
                        retval = 0;
                    }
                    else {
                        retval = CHPEG_ERR_EXTRANEOUS_INPUT;
                    }
                }
                break;

            case CHPEG_OP_FAIL: // overall failure
                pc = -1; // we're done
                retval = arg < 0 ? arg : CHPEG_ERR_PARSE_FAILED;
                break;

            default:
                pc = -1; // we're done
                retval = CHPEG_ERR_INVALID_INSTRUCTION;
                break;
        }

#if CHPEG_VM_PRINT_TREE
        if (self->vm_print_tree && tree_changed) {
            fprintf(stderr, "============================================\n");
            ChpegParser_print_tree(self, input, stderr);
            fprintf(stderr, "============================================\n");
        }
#endif
        if (pc < 0) {
            goto done;
        }
    }

    retval = CHPEG_ERR_RUNAWAY;

done:

#if CHPEG_PACKRAT
    if (packrat) {
        loop_end = num_defs * window_size;
        for (i = 0; i < loop_end; ++i) {
            ChpegPNode *pnode = packrat[i];
            if (pnode && pnode != packrat_no_match) {
                assert(pnode->node->refs > 0);
                ChpegPNode_free(pnode);
            }
        }
        CHPEG_FREE(packrat);
    }
    if (packrat_no_match) {
        CHPEG_FREE(packrat_no_match);
    }
#endif

    self->parse_result = retval;
    return retval;

#undef CHPEG_CHECK_STACK_OVERFLOW
#undef CHPEG_CHECK_STACK_UNDERFLOW
#undef CHPEG_CHECK_TSTACK_OVERFLOW
#undef CHPEG_CHECK_TSTACK_UNDERFLOW

}

// } chpeg: parser.c

