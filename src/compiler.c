//
// chpeg: compiler.c {
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/parser.h"
#include "chpeg/compiler.h"
#include "chpeg/opcodes.h"
#include "chpeg/chpeg_bytecode.h"
#endif

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

//
// Gnode: grammar tree node (internal use during compilation)
// FIXME: probably more of a 'compilation unit node'
//

typedef struct _ChpegGNode
{
    ChpegNode *node;
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
    struct _ChpegGNode *head;
    struct _ChpegGNode *next;
} ChpegGNode;

static ChpegGNode *ChpegGNode_new()
{
    ChpegGNode *self = (ChpegGNode *)CHPEG_MALLOC(sizeof(ChpegGNode));

    self->node = NULL;
    self->type = -1;

    self->parse_state = -1;
    self->parent_next_state = -1;
    self->parent_fail_state = -1;

    memset(self->val.cval, 0, 4);
    self->value_len = 0;

    self->num_children = 0;
    self->head = NULL;
    self->next = NULL;
    return self;
}

static void ChpegGNode_free(ChpegGNode *self)
{
    if (self == NULL) return;

    ChpegGNode *tmp;
    for (ChpegGNode *p = self->head; p; p = tmp) {
        tmp = p->next;
        ChpegGNode_free(p);
    }
    self->head = NULL;
    CHPEG_FREE(self);
}

static ChpegGNode *ChpegGNode_push_child(ChpegGNode *self, ChpegGNode *child)
{
    child->next = self->head;
    self->head = child;
    ++(self->num_children);
    return child;
}

// unused
#if 0
static void ChpegGNode_pop_child(ChpegGNode *self)
{
    if (self->head) {
        ChpegGNode *tmp = self->head;
        self->head = self->head->next;
        ChpegGNode_free(tmp);
        --(self->num_children);
    }
}
#endif

static ChpegGNode *ChpegGNode_reverse(ChpegGNode *self)
{
    ChpegGNode *p = self->head; self->head = NULL;
    ChpegGNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = ChpegGNode_reverse(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

//
// Compiler
//

typedef struct _ChpegCU
{
    ChpegParser *parser;
    const unsigned char *input;
    ChpegByteCode *bc;
    ChpegGNode *root;
    int strings_allocated;
} ChpegCU;

#if DEBUG_COMPILER
static void ChpegCU_print(ChpegCU *cu, ChpegGNode *gnode, const unsigned char *input, int depth)
{
    int flags = 0;
    char *data = NULL;
    const char *def_name = ChpegByteCode_def_name(cu->parser->bc, gnode->type);

    ChpegNode *node = gnode->node;
    if (node) {
        data = chpeg_esc_bytes(&input[node->offset], node->length, 40);
        flags = node->flags;
    }

    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
        printf(" Begin    Len  DefID    Parse  PNext  PFail  Flags  Def. Name / Data\n");
        printf("---------------------------------------------------------------------------------\n");
    }
    printf("%6zu %6zu %6d | %6d %6d %6d | %s%s%s | %*s%s \"%s\"\n",
        node ? node->offset : -1,
        node ? node->length : -1,
        gnode->type,
        gnode->parse_state,
        gnode->parent_next_state,
        gnode->parent_fail_state,
        flags & CHPEG_FLAG_STOP ? "S" : " ",
        flags & CHPEG_FLAG_IGNORE ? "I" : " ",
        flags & CHPEG_FLAG_LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : ""
        );
    if (data) { CHPEG_FREE(data); data = NULL; }

    for (ChpegGNode *p = gnode->head; p; p = p->next) {
        ChpegCU_print(cu, p, input, depth + 1);
    }
}
#endif

static void ChpegCU_setup_defs(ChpegCU *cu)
{
    ChpegNode *p = NULL;
    int i = 0, j = 0;

    cu->bc->num_defs = cu->parser->tree_root->num_children;
#if DEBUG_COMPILER
    printf("ChpegCU_setup_defs: num_defs=%d\n", cu->bc->num_defs);
#endif

    cu->bc->def_names = (char **)CHPEG_MALLOC(cu->bc->num_defs * sizeof(char *));
    cu->bc->def_flags = (int *)CHPEG_MALLOC(cu->bc->num_defs * sizeof(int));
    cu->bc->def_addrs = (int *)CHPEG_MALLOC(cu->bc->num_defs * sizeof(int));

    for (p = cu->parser->tree_root->head, i = 0; p; p = p->next, ++i) {
        if (CHPEG_DEF_DEFINITION != p->def) { continue; }

        ChpegNode *tmp = p->head; // Identifier, definition name
        if (NULL == tmp || CHPEG_DEF_IDENTIFIER != tmp->def) { continue; }
        cu->bc->def_names[i] = (char *)CHPEG_MALLOC(1 + tmp->length);
        memcpy(cu->bc->def_names[i], &cu->input[tmp->offset], tmp->length);
        cu->bc->def_names[i][tmp->length] = '\0';

        int flags = 0;
        tmp = tmp->next; // Options, flags
        if (NULL != tmp && CHPEG_DEF_OPTIONS == tmp->def) {
            for (j = 0; j < tmp->length; ++j) {
                switch(cu->input[tmp->offset + j]) {
                    case 'S': flags |= CHPEG_FLAG_STOP; break;
                    case 'I': flags |= CHPEG_FLAG_IGNORE; break;
                    case 'L': flags |= CHPEG_FLAG_LEAF; break;
                }
            }
            p->head->next = tmp->next; // eliminate OPTIONS node
            ChpegNode_free(tmp);
        }
        cu->bc->def_flags[i] = flags;
#if DEBUG_COMPILER
        printf("Compile_setup_defs: def[%i]: name=%s flags=%d\n", i, cu->bc->def_names[i], cu->bc->def_flags[i]);
#endif
    }
}

static void ChpegCU_setup_def_addrs(ChpegCU *cu)
{
    int i = 0;
    ChpegGNode *p = NULL;
    for (i = 0, p = cu->root->head; p; p = p->next, ++i) {
        cu->bc->def_addrs[i] = p->head->next->parse_state - 1;
    }
}

static int ChpegCU_find_def(ChpegCU *cu, ChpegNode *ident)
{
    for (int i = 0; i < cu->bc->num_defs; ++i) {
        if (ident->length == strlen(cu->bc->def_names[i]) &&
            0 == memcmp(cu->input + ident->offset, cu->bc->def_names[i], ident->length))
        {
            return i;
        }
    }
    return -1;
}

static void ChpegCU_build_tree(ChpegCU *cu, ChpegNode *np, ChpegGNode *gp)
{
    if (NULL == np) { np = cu->parser->tree_root; }
    if (NULL == gp) { gp = cu->root; }
    gp->node = np;
    gp->type = np->def;
    for (ChpegNode *p = np->head; p; p = p->next) {
        ChpegGNode *g = ChpegGNode_new();
        ChpegCU_build_tree(cu, p, g);
        ChpegGNode_push_child(gp, g);
    }
}

// TODO: count in cu not bc
static inline int ChpegCU_alloc_inst(ChpegCU *cu)
{
    return cu->bc->num_instructions++;
}

static void ChpegCU_alloc_instructions(ChpegCU *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_DEF_GRAMMAR:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
            }
            break;
        case CHPEG_DEF_DEFINITION:
            ChpegCU_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = ChpegCU_alloc_inst(cu);
            gp->head->next->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_CHOICE:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
                p->parent_next_state = ChpegCU_alloc_inst(cu);
                p->parent_fail_state = ChpegCU_alloc_inst(cu);
            }
            ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
            }
            gp->parse_state = gp->head->parse_state;
            break;
        case CHPEG_DEF_REPEAT:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, gp->head);
            gp->head->parent_next_state = ChpegCU_alloc_inst(cu);
            gp->head->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_PREDICATE:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = ChpegCU_alloc_inst(cu);
            gp->head->next->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_DOT:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_IDENTIFIER:
        case CHPEG_DEF_CHARCLASS:
        case CHPEG_DEF_LITERAL:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            break;
    }
}

// TODO: move to ChpegByteCode
static inline void ChpegCU_add_inst(ChpegCU *cu, int inst)
{
    cu->bc->instructions[cu->bc->num_instructions++] = inst;
}

static void ChpegCU_add_instructions(ChpegCU *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_DEF_GRAMMAR:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, 0));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_FAIL, 0));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_SUCC, 0));
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_add_instructions(cu, p);
            }
            break;
        case CHPEG_DEF_DEFINITION:
            ChpegCU_add_instructions(cu, gp->head->next);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_ISUCC, ChpegCU_find_def(cu, gp->head->node)));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IFAIL, 0));
            break;
        case CHPEG_DEF_CHOICE:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHOICE, 0));
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_add_instructions(cu, p);
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CISUCC, gp->parent_next_state - 1));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CIFAIL, 0));
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CFAIL, gp->parent_fail_state - 1));
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : gp->parent_next_state;
                p->parent_fail_state = gp->parent_fail_state;
                ChpegCU_add_instructions(cu, p);
            }
            break;
        case CHPEG_DEF_REPEAT:
            {
                unsigned char op = cu->input[gp->head->next->node->offset];
                switch (op) {
                    case '+':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPBEG, 0));
                        ChpegCU_add_instructions(cu, gp->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPMAT, gp->head->parse_state - 1));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPDONE, gp->parent_fail_state - 1));
                        break;
                    case '*':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSBEG, 0));
                        ChpegCU_add_instructions(cu, gp->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSMAT, gp->head->parse_state - 1));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSDONE, 0));
                        break;
                    case '?':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQBEG, 0));
                        ChpegCU_add_instructions(cu, gp->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQMAT, 0));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQDONE, 0));
                        break;
                }
            }
            break;
        case CHPEG_DEF_PREDICATE:
            {
                unsigned char op = cu->input[gp->head->node->offset];
                switch (op) {
                    case '&':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PRED, 0));
                        ChpegCU_add_instructions(cu, gp->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHS, gp->parent_fail_state - 1));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATF, gp->parent_fail_state - 1));
                        break;
                    case '!':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PRED, 0));
                        ChpegCU_add_instructions(cu, gp->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHF, gp->parent_fail_state - 1));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATS, gp->parent_fail_state - 1));
                        break;
                }
            }
            break;
        case CHPEG_DEF_DOT:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_DOT, gp->parent_fail_state - 1));
            break;
        case CHPEG_DEF_IDENTIFIER:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, ChpegCU_find_def(cu, gp->node)));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
        case CHPEG_DEF_CHARCLASS:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHRCLS, gp->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
        case CHPEG_DEF_LITERAL:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_LIT, gp->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
    }
}

static int ChpegCU_alloc_string(ChpegCU *cu, const unsigned char *str, int len)
{
    for (int i = 0; i < cu->bc->num_strings; ++i) {
        if (len == cu->bc->str_len[i] && 0 == memcmp(cu->bc->strings[i], str, len)) {
            return i;
        }
    }
    if (NULL == cu->bc->strings) {
        cu->bc->num_strings = 0;
        cu->strings_allocated = 16;
        cu->bc->strings = (unsigned char **)CHPEG_MALLOC(cu->strings_allocated * sizeof(unsigned char *));
        cu->bc->str_len = (int *)CHPEG_MALLOC(cu->strings_allocated * sizeof(int));
    }
    if (cu->bc->num_strings == cu->strings_allocated) {
        cu->strings_allocated *= 2;
        cu->bc->strings = (unsigned char **)CHPEG_REALLOC(cu->bc->strings, cu->strings_allocated * sizeof(unsigned char *));
        cu->bc->str_len = (int *)CHPEG_REALLOC(cu->bc->str_len, cu->strings_allocated * sizeof(int));
    }
    int idx = cu->bc->num_strings++;
    cu->bc->strings[idx] = (unsigned char *)CHPEG_MALLOC(len + 1);
    memcpy(cu->bc->strings[idx], str, len);
    cu->bc->strings[idx][len] = '\0'; // safety
    cu->bc->str_len[idx] = len;
    return idx;
}

static void ChpegCU_alloc_strings(ChpegCU *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_DEF_LITERAL:
        case CHPEG_DEF_CHARCLASS:
            {
                int len = 0, offset = 0;
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    ChpegCU_alloc_strings(cu, p);
                    len += p->value_len;
                }
                unsigned char *str = (unsigned char *)CHPEG_MALLOC(len);
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    memcpy(str+offset, p->val.cval, p->value_len);
                    offset += p->value_len;
                }
                gp->val.ival = ChpegCU_alloc_string(cu, str, len);
#if DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(str, len, 20);
                printf("PEG LITERAL/CHARCLASS %s %d\n", tmp, gp->val.ival);
                CHPEG_FREE(tmp);
#endif
                CHPEG_FREE(str);
            }
            break;
        case CHPEG_DEF_CHARRANGE:
            {
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    ChpegCU_alloc_strings(cu, p);
                }
                gp->val.cval[0] = gp->head->val.cval[0];
                gp->val.cval[1] = '-';
                gp->val.cval[2] = gp->head->next->val.cval[0];
                gp->value_len = 3;
#if DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_CHARRANGE %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_DEF_PLAINCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset];
                gp->value_len = 1;
#if DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_PLAINCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_DEF_ESCCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset + 1];
                gp->value_len = 1;
                switch (gp->val.cval[0]) {
                    case 'n': gp->val.cval[0] = '\n'; break;
                    case 'r': gp->val.cval[0] = '\r'; break;
                    case 't': gp->val.cval[0] = '\t'; break;
                }
#if DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_ESCCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_DEF_OCTCHAR:
            {
                int val = 0; int len = gp->node->length - 1;
                const unsigned char *ip = cu->input + gp->node->offset + 1;
                for (int i = 0; i < len; ++i) {
                    val = (val << 3) | (ip[i] - '0');
                }
                gp->val.cval[0] = val & 255;
                gp->value_len = 1;
#if DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_OCTCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        default:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_alloc_strings(cu, p);
            }
            break;
    }
}

// This is more of a convenience function, thus the lowercase chpeg_ prefix.
// I plan to create a more OO-like API to the compiler to allow more flexibility.
// verbose is a bit field: 1: info | 2: print parse tree | 1<<15: disable errors
int chpeg_compile(const unsigned char *input, size_t length,
    ChpegByteCode **bytecode_return, int verbose)
{
    ChpegCU cu;

    cu.parser = ChpegParser_new(chpeg_default_bytecode());
    cu.input = input;
    cu.bc = NULL;
    cu.root = NULL;
    cu.strings_allocated = 0;

    size_t consumed = 0;
    int parse_result = ChpegParser_parse(cu.parser, input, length, &consumed);

    if (parse_result == 0) {
        if (verbose & 1) {
            fprintf(stderr, "chpeg_compile: Parse successful.\n");
        }
        if (verbose & 2) {
            ChpegParser_print_tree(cu.parser, input, stderr);
        }
    }
    else {
        if ((verbose & (1<<15)) == 0) {
            if (parse_result == CHPEG_ERR_EXTRANEOUS_INPUT) {
                fprintf(stderr, "chpeg_compile: Extraneous input: "
                    "parse consumed %lu bytes out of %lu\n", consumed, length);
            }
            else {
                fprintf(stderr, "chpeg_compile: Parse failed with result: %d\n",
                    parse_result);
            }
            ChpegParser_print_error(cu.parser, input);
        }
        goto done;
    }

    cu.bc = ChpegByteCode_new();
    ChpegCU_setup_defs(&cu);

    cu.root = ChpegGNode_new();

    ChpegCU_build_tree(&cu, NULL, NULL);
    ChpegGNode_reverse(cu.root);

    ChpegCU_alloc_strings(&cu, cu.root);

    ChpegCU_alloc_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions alloc'd: %d\n", cu.bc->num_instructions);
#endif
    cu.bc->instructions = (int *)CHPEG_CALLOC(cu.bc->num_instructions, sizeof(int));

    cu.bc->num_instructions = 0;
    ChpegCU_add_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions after add: %d\n", cu.bc->num_instructions);
#endif

    ChpegCU_setup_def_addrs(&cu);

#if DEBUG_COMPILER
    ChpegCU_print(&cu, cu.root, input, 0);
#endif

done:
    if (cu.parser) { ChpegParser_free(cu.parser); }
    if (cu.root) { ChpegGNode_free(cu.root); }
    if (bytecode_return) {
        *bytecode_return = cu.bc;
    }
    return parse_result;
}

const ChpegByteCode *chpeg_default_bytecode()
{
    return &chpeg_bytecode;
}

// } chpeg: compiler.c

