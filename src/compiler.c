//
// chpeg: compiler.c {
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/util.h"
#include "chpeg/parser.h"
#include "chpeg/compiler.h"
#include "chpeg/opcodes.h"
#if CHPEG_USES_EXTENSIONS
#include "chpeg/chpeg_ext_bytecode.h"
#else
#include "chpeg/chpeg_bytecode.h"
#endif
#endif

#ifndef CHPEG_DEBUG_COMPILER
#define CHPEG_DEBUG_COMPILER 0
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
    struct _ChpegGNode *parent;
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
    self->parent = NULL;
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
    child->parent = self;
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
    int refs_allocated;
} ChpegCU;

#if CHPEG_EXTENSION_TRIM
static int ChpegCU_find_def_node(ChpegCU *cu, ChpegGNode *gnode, ChpegGNode **def_return)
{
    ChpegGNode *parent = gnode;

    for (; parent && parent != cu->root; parent = parent->parent) {
        if (parent->type == CHPEG_DEF_DEFINITION) {
            if (def_return) {
                *def_return = parent;
            }
            return 0;
        }
    }
    if (def_return) {
        *def_return = NULL;
    }
    return 1;
}
#endif

#if CHPEG_DEBUG_COMPILER
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
        fprintf(stderr, "---------------------------------------------------------------------------------\n");
        fprintf(stderr, " Begin    Len  DefID    Parse  PNext  PFail  Flags  Def. Name / Data\n");
        fprintf(stderr, "---------------------------------------------------------------------------------\n");
    }
    fprintf(stderr, "%6zu %6zu %6d | %6d %6d %6d | %s%s%s%s | %*s%s \"%s\"\n",
        node ? node->offset : -1,
        node ? node->length : -1,
        gnode->type,
        gnode->parse_state,
        gnode->parent_next_state,
        gnode->parent_fail_state,
        flags & CHPEG_FLAG_STOP ? "S" : "-",
        flags & CHPEG_FLAG_IGNORE ? "I" : "-",
        flags & CHPEG_FLAG_LEAF ? "L" : "-",
        flags & CHPEG_FLAG_PACKRAT ? "P" : "-",
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
#if CHPEG_DEBUG_COMPILER
    fprintf(stderr, "ChpegCU_setup_defs: num_defs=%d\n", cu->bc->num_defs);
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
                    case 'C':
                    case 'S':
                        flags |= CHPEG_FLAG_STOP; break;
                    case 'I':
                        flags |= CHPEG_FLAG_IGNORE; break;
                    case 'L':
                    case 'T':
                        flags |= CHPEG_FLAG_LEAF; break;
                    case 'P':
                        flags |= CHPEG_FLAG_PACKRAT; break;
                }
            }
            p->head->next = tmp->next; // eliminate OPTIONS node
            ChpegNode_free(tmp);
        }
        cu->bc->def_flags[i] = flags;
#if CHPEG_DEBUG_COMPILER
        fprintf(stderr, "Compile_setup_defs: def[%i]: name=%s flags=%d\n", i, cu->bc->def_names[i], cu->bc->def_flags[i]);
#endif
    }
}

static void ChpegCU_setup_def_addrs(ChpegCU *cu)
{
    int i = 0;
    ChpegGNode *p = NULL;
    for (i = 0, p = cu->root->head; p; p = p->next, ++i) {
        cu->bc->def_addrs[i] = p->head->next->parse_state;
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

// reserve instructions and keep track of instruction addresses where references needed
// (referred to as state: parse_state, parent_next_state, etc.)
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
#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            gp->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, gp->head);
            gp->head->parent_next_state = ChpegCU_alloc_inst(cu);
            gp->head->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
#endif
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            gp->parse_state = ChpegCU_alloc_inst(cu);                     // MARK
            ChpegCU_alloc_instructions(cu, gp->head->next);               // (CONTAINED INSTRUCTIONS)
            gp->head->next->parent_next_state = ChpegCU_alloc_inst(cu);   // MARKS
            gp->head->next->parent_fail_state = ChpegCU_alloc_inst(cu);   // MARKF
            break;
#endif
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
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_REFERENCE:
#endif
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

#if CHPEG_EXTENSION_REFS
static int ChpegCU_find_ref(ChpegCU *cu, ChpegNode *node);
#endif

static void ChpegCU_add_instructions(ChpegCU *cu, ChpegGNode *gp)
{
    int def = 0;
#if CHPEG_EXTENSION_REFS
    int ref_id = 0;
#endif
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
            def = ChpegCU_find_def(cu, gp->head->node);
            ChpegCU_add_instructions(cu, gp->head->next);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_ISUCC, def));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IFAIL, def));
            break;
        case CHPEG_DEF_CHOICE:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHOICE, 0));
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                ChpegCU_add_instructions(cu, p);
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CISUCC, gp->parent_next_state));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CIFAIL, 0));
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CFAIL, gp->parent_fail_state));
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : gp->parent_next_state;
                p->parent_fail_state = gp->parent_fail_state;
                ChpegCU_add_instructions(cu, p);
            }
            break;
#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            {
                ChpegGNode *def_node;
                def = -1;
                if (0 == ChpegCU_find_def_node(cu, gp, &def_node)) {
                    def = ChpegCU_find_def(cu, def_node->head->node);
                }
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIM, def)); // arg def is informational only
            ChpegCU_add_instructions(cu, gp->head);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMS, gp->parent_next_state));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMF, gp->parent_fail_state));
            break;
#endif
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            ref_id = ChpegCU_find_ref(cu, gp->node);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARK, ref_id));
            ChpegCU_add_instructions(cu, gp->head->next);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARKS, gp->parent_next_state));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARKF, gp->parent_fail_state));
            break;
        case CHPEG_DEF_REFERENCE:
            ref_id = ChpegCU_find_ref(cu, gp->node);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_REF, ref_id));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state));
            break;
#endif
        case CHPEG_DEF_REPEAT:
            {
                unsigned char op = cu->input[gp->head->next->node->offset];
                switch (op) {
                    case '+':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPBEG, 0));
                        ChpegCU_add_instructions(cu, gp->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPMAT, gp->head->parse_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPDONE, gp->parent_fail_state));
                        break;
                    case '*':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSBEG, 0));
                        ChpegCU_add_instructions(cu, gp->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSMAT, gp->head->parse_state));
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
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDA, 0));
                        ChpegCU_add_instructions(cu, gp->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHS, gp->parent_fail_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATF, gp->parent_fail_state));
                        break;
                    case '!':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDN, 0));
                        ChpegCU_add_instructions(cu, gp->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHF, gp->parent_fail_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATS, gp->parent_fail_state));
                        break;
                }
            }
            break;
        case CHPEG_DEF_DOT:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_DOT, gp->parent_fail_state));
            break;
        case CHPEG_DEF_IDENTIFIER:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, ChpegCU_find_def(cu, gp->node)));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state));
            break;
        case CHPEG_DEF_CHARCLASS:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHRCLS, gp->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state));
            break;
        case CHPEG_DEF_LITERAL:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_LIT, gp->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state));
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
#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(str, len, 20);
                fprintf(stderr, "alloc_strings: LITERAL/CHARCLASS %s %d\n", tmp, gp->val.ival);
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
#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                fprintf(stderr, "alloc_strings: CHARRANGE %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_DEF_PLAINCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset];
                gp->value_len = 1;
#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                fprintf(stderr, "alloc_strings: PLAINCHAR %s\n", tmp);
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
#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                fprintf(stderr, "alloc_strings: ESCCHAR %s\n", tmp);
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
#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(gp->val.cval, gp->value_len, 10);
                fprintf(stderr, "alloc_strings: OCTCHAR %s\n", tmp);
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

#if CHPEG_EXTENSION_REFS

// Return the reference id given a Mark or Reference node;
// node's first child should be an Identifier containing the reference name;
// Returns int reference id, or:
// -1, if not found
// -2, if node's first child (head) is NULL or not an identifier
static int ChpegCU_find_ref(ChpegCU *cu, ChpegNode *node)
{
    ChpegNode *ident = node->head;

    if (!ident) return -2;
    if (ident->def != CHPEG_DEF_IDENTIFIER) return -2;

    for (int i = 0; i < cu->bc->num_refs; ++i) {
        if (ident->length == strlen(cu->bc->refs[i]) &&
            0 == memcmp(cu->input + ident->offset, cu->bc->refs[i], ident->length))
        {
            return i;
        }
    }
    return -1;
}

// Return the reference id given a Mark or Reference node; allocates reference if necessary.
// node's first child (head) should be an Identifier containing the reference name;
// Returns int reference id; or:
// -2, if first child is not found or not an identifier
// -3, if unable to allocate reference string
static int ChpegCU_alloc_ref(ChpegCU *cu, ChpegNode *node)
{
    int ref = ChpegCU_find_ref(cu, node);
    if (ref != -1) { // ref already exists or error
        return ref;
    }

    // refs array initial allocation
    if (!cu->bc->refs) {
        cu->bc->num_refs = 0;
        cu->refs_allocated = 16;
        cu->bc->refs = (char **)CHPEG_MALLOC(cu->refs_allocated * sizeof(unsigned char *));
    }
    // grow refs array if necessary
    if (cu->bc->num_refs + 1 >= cu->refs_allocated) {
        cu->refs_allocated *= 2;
        cu->bc->refs = (char **)CHPEG_REALLOC(cu->bc->refs, cu->refs_allocated * sizeof(char *));
    }

    ChpegNode *ident = node->head;
    int ref_id = cu->bc->num_refs++;

    // allocate reference string
    char *str = (char *)CHPEG_MALLOC(ident->length + 1);
    if (!str) return -3;

    // copy string data and terminate
    memcpy(str, cu->input + ident->offset, ident->length);
    str[ident->length] = '\0';
    cu->bc->refs[ref_id] = str;

    return ref_id;
}

static int ChpegCU_alloc_refs(ChpegCU *cu, ChpegGNode *gp)
{
    int err = 0;
    switch (gp->type) {
        case CHPEG_DEF_MARK:
        case CHPEG_DEF_REFERENCE:
            {
                int ref_id = ChpegCU_alloc_ref(cu, gp->node);
#if CHPEG_DEBUG_COMPILER
                const char *def_name = ChpegByteCode_def_name(chpeg_default_bytecode(), gp->type);
                fprintf(stderr, "alloc_refs: (via %s) refs[%d]=\"%s\"\n", def_name,
                    ref_id, ref_id >= 0 ? cu->bc->refs[ref_id] : "<ERROR>");
#endif
                err = (ref_id < 0) ? ref_id : 0;
                goto done;
            }
        default:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                if ((err = ChpegCU_alloc_refs(cu, p)) != 0) {
                    goto done;
                }
            }
            break;
    }
done:
    return err;
}

#endif

void chpeg_sanity_check()
{
    const ChpegByteCode *bc = chpeg_default_bytecode();

    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_GRAMMAR), "Grammar") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_DEFINITION), "Definition") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_CHOICE), "Choice") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_SEQUENCE), "Sequence") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_PREDICATE), "Predicate") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_REPEAT), "Repeat") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_PRIMARY), "Primary") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_OPTIONS), "Options") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_IDENTIFIER), "Identifier") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_LITERAL), "Literal") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_CHARCLASS), "CharClass") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_CHARRANGE), "CharRange") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_CHAR), "Char") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_ESCCHAR), "EscChar") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_OCTCHAR), "OctChar") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_PLAINCHAR), "PlainChar") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_PREDOP), "PredOp") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_REPOP), "RepOp") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_DOT), "Dot") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_S), "S") == 0);

#ifdef CHPEG_EXTENSION_TRIM
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_TRIM), "Trim") == 0);
#endif

#ifdef CHPEG_EXTENSION_REFS
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_MARK), "Mark") == 0);
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_REFERENCE), "Reference") == 0);
#endif

}


// int chpeg_compile(
//    const unsigned char *input,       // grammar input
//    size_t length,                    // input data length
//    ChpegByteCode **bytecode_return,  // bytecode returned here
//    int verbose                       // bitfield; see below
//    );
// returns: ChpegParser_parse status/error code (see ChpegErrorCodes)
//
// Parse and compile grammar input to bytecode.
//
// This is more of a convenience function, thus the lowercase chpeg_ prefix.
// I plan to create a more OO-like API to the compiler to allow more flexibility.
// Error handling and warnings are mostly not present. Many error/warning conditions silently ignored.
//
// verbose is a bit field: 1: info | 2: print parse tree | 1<<15: disable errors
//
// I need a better way to deal with errors. Should be dealt with when creating a better API.
// For now I'm just using CHPEG_ERR_COMPILE, added into the parser error codes (ChpegErrorCodes)
int chpeg_compile(const unsigned char *input, size_t length,
    ChpegByteCode **bytecode_return, int verbose)
{
    if (verbose & 3) {
#if CHPEG_USES_EXTENSIONS
        fprintf(stderr, "Using extensions:");

#if CHPEG_EXTENSION_TRIM
        fprintf(stderr, " TRIM");
#endif
#if CHPEG_EXTENSION_REFS
        fprintf(stderr, " REFS");
#endif
        fprintf(stderr, "\n");

#else
        fprintf(stderr, "Not using extensions.\n");
#endif
    }
    chpeg_sanity_check();

    int err = 0;
    ChpegCU cu;

    cu.parser = ChpegParser_new(chpeg_default_bytecode());
    cu.input = input;
    cu.bc = NULL;
    cu.root = NULL;
    cu.strings_allocated = 0;

    size_t consumed = 0;
    int parse_result = ChpegParser_parse(cu.parser, input, length, &consumed);
    err = parse_result;

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

#if CHPEG_EXTENSION_REFS
    if ((err = ChpegCU_alloc_refs(&cu, cu.root)) != 0) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }
#endif

    ChpegCU_alloc_instructions(&cu, cu.root);
#if CHPEG_DEBUG_COMPILER
    fprintf(stderr, "instructions alloc'd: %d\n", cu.bc->num_instructions);
#endif
    cu.bc->instructions = (int *)CHPEG_CALLOC(cu.bc->num_instructions, sizeof(int));

    cu.bc->num_instructions = 0;
    ChpegCU_add_instructions(&cu, cu.root);
#if CHPEG_DEBUG_COMPILER
    fprintf(stderr, "instructions after add: %d\n", cu.bc->num_instructions);
#endif

    ChpegCU_setup_def_addrs(&cu);

#if CHPEG_DEBUG_COMPILER
    ChpegCU_print(&cu, cu.root, input, 0);
#endif

done:
    if (cu.parser) { ChpegParser_free(cu.parser); }
    if (cu.root) { ChpegGNode_free(cu.root); }
    if (bytecode_return) {
        *bytecode_return = cu.bc;
    }
    return err;
}

const ChpegByteCode *chpeg_default_bytecode()
{
    return &chpeg_bytecode;
}

// } chpeg: compiler.c

