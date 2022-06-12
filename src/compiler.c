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
// CNode: compilation unit node
//

typedef struct _ChpegCNode
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
    struct _ChpegCNode *head;
    struct _ChpegCNode *next;
    struct _ChpegCNode *parent;
} ChpegCNode;

static ChpegCNode *ChpegCNode_new()
{
    ChpegCNode *self = (ChpegCNode *)CHPEG_MALLOC(sizeof(ChpegCNode));

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

static void ChpegCNode_free(ChpegCNode *self)
{
    if (self == NULL) return;

    ChpegCNode *tmp;
    for (ChpegCNode *p = self->head; p; p = tmp) {
        tmp = p->next;
        ChpegCNode_free(p);
    }
    self->head = NULL;
    CHPEG_FREE(self);
}

static ChpegCNode *ChpegCNode_push_child(ChpegCNode *self, ChpegCNode *child)
{
    child->parent = self;
    child->next = self->head;
    self->head = child;
    ++(self->num_children);
    return child;
}

// unused
#if 0
static void ChpegCNode_pop_child(ChpegCNode *self)
{
    if (self->head) {
        ChpegCNode *tmp = self->head;
        self->head = self->head->next;
        ChpegCNode_free(tmp);
        --(self->num_children);
    }
}
#endif

static ChpegCNode *ChpegCNode_reverse(ChpegCNode *self)
{
    ChpegCNode *p = self->head; self->head = NULL;
    ChpegCNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = ChpegCNode_reverse(p);
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
    ChpegCNode *root;
    int strings_allocated;
    int refs_allocated;
} ChpegCU;

static int ChpegCU_find_def(ChpegCU *cu, ChpegCNode *ident);

#if CHPEG_EXTENSION_TRIM
static ChpegCNode *ChpegCU_find_def_node(ChpegCU *cu, ChpegCNode *cnode)
{
    ChpegCNode *parent = cnode;
    for (; parent && parent != cu->root; parent = parent->parent) {
        if (parent->type == CHPEG_DEF_DEFINITION) {
            return parent;
        }
    }
    return NULL;
}

static int ChpegCU_contains_trim(ChpegCNode *cnode)
{
    assert(cnode);
    switch (cnode->type) {
        case CHPEG_DEF_TRIM:
            return 1;
            break;
        default:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                if (ChpegCU_contains_trim(p)) {
                    return 1;
                }
            }
            break;
    }
    return 0;
}

static int ChpegCU_auto_leaf(ChpegCU *cu)
{
    assert(cu->root && cu->root->head);
    ChpegCNode *cdef = cu->root->head;

    for (; cdef; cdef = cdef->next) {

        assert(cdef->head && cdef->head->next);

        if (ChpegCU_contains_trim(cdef->head->next)) {
            int def_id = cdef->val.ival;
            assert(def_id >= 0 && def_id < cu->bc->num_defs);

            cu->bc->def_flags[def_id] |= CHPEG_FLAG_LEAF;

#if CHPEG_DEBUG_COMPILER
            fprintf(stderr, "%s: def_id=%d; found TRIM, adding CHPEG_FLAG_LEAF flags; flags=%d\n",
                __func__, def_id, cu->bc->def_flags[def_id]);
#endif
        }
    }
    return 0;
}
#endif

#if CHPEG_DEBUG_COMPILER
static void ChpegCU_print(ChpegCU *cu, ChpegCNode *cnode, const unsigned char *input, int depth)
{
    char *data = NULL;
    char flags[CHPEG_FLAGS_DISPLAY_SIZE];
    const char *def_name = ChpegByteCode_def_name(cu->parser->bc, cnode->type);

    ChpegNode *node = cnode->node;
    if (node) {
        data = chpeg_esc_bytes(&input[node->offset], node->length, 40);
    }

    if (depth == 0) {
        fprintf(stderr, "---------------------------------------------------------------------------------\n");
        fprintf(stderr, " Begin    Len  DefID    Parse  PNext  PFail  Flags  Def. Name / Data\n");
        fprintf(stderr, "---------------------------------------------------------------------------------\n");
    }
    fprintf(stderr, "%6zu %6zu %6d | %6d %6d %6d | %3s | %*s%s \"%s\"\n",
        node ? node->offset : -1,
        node ? node->length : -1,
        cnode->type,
        cnode->parse_state,
        cnode->parent_next_state,
        cnode->parent_fail_state,
        chpeg_flags(flags, node->flags),
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : ""
        );
    if (data) { CHPEG_FREE(data); data = NULL; }

    for (ChpegCNode *p = cnode->head; p; p = p->next) {
        ChpegCU_print(cu, p, input, depth + 1);
    }
}
#endif

static int ChpegCU_setup_defs(ChpegCU *cu)
{
    int err = 0;

    int num_defs = cu->root->num_children; // number of defs we supposedly have

#if CHPEG_DEBUG_COMPILER
    fprintf(stderr, "%s: num_defs=%d\n", __func__, num_defs);
#endif

    // cu->bc should be kept in a state we can clean up if we encounter an error
    cu->bc->num_defs = 0; // increment as valid defs are added
    cu->bc->def_names = (char **)CHPEG_MALLOC(num_defs * sizeof(char *));
    cu->bc->def_flags = (int *)CHPEG_MALLOC(num_defs * sizeof(int));
    cu->bc->def_addrs = (int *)CHPEG_MALLOC(num_defs * sizeof(int));

    ChpegCNode *cdef = cu->root->head;
    for (int def_id = 0; cdef; cdef = cdef->next, ++def_id) {
        assert (cdef->type == CHPEG_DEF_DEFINITION);

        // stash the def_id in the DEFINITION cnode
        cdef->val.ival = def_id;

        ChpegCNode *cident = cdef->head; // first child: Identifier: definition name
        assert (cident->type == CHPEG_DEF_IDENTIFIER);

        if (ChpegCU_find_def(cu, cident) >= 0) {
            char *tmp = chpeg_esc_bytes(cu->input + cident->node->offset, cident->node->length, 0);
            fprintf(stderr, "Definition '%s' is already defined.\n", tmp);
            CHPEG_FREE(tmp);
            err = 1;
            goto done;
        }

        cu->bc->def_names[def_id] = (char *)CHPEG_MALLOC(1 + cident->node->length);
        ++cu->bc->num_defs;

        memcpy(cu->bc->def_names[def_id], &cu->input[cident->node->offset], cident->node->length);
        cu->bc->def_names[def_id][cident->node->length] = '\0';

        // second child: Options / a.k.a. flags
        int flags = 0;
        if (cident->next->node->def == CHPEG_DEF_OPTIONS) {
            ChpegCNode *coptions = cident->next;

            for (int i = 0; i < coptions->node->length; ++i) {
                switch(cu->input[coptions->node->offset + i]) {
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
                    case 'R':
                        flags |= CHPEG_FLAG_REFSCOPE; break;
                }
            }
            // eliminate Options node
            cident->node->next = coptions->node->next;
            cident->next = coptions->next;
            ChpegNode_free(coptions->node);
            ChpegCNode_free(coptions);
        }

        cu->bc->def_flags[def_id] = flags;

#if CHPEG_DEBUG_COMPILER
        fprintf(stderr, "%s: def[%i]: name=%s flags=%d\n", __func__,
            def_id, cu->bc->def_names[def_id], cu->bc->def_flags[def_id]);
#endif

    }

done:
    return err;
}

static void ChpegCU_setup_def_addrs(ChpegCU *cu)
{
    int i = 0;
    ChpegCNode *p = NULL;
    for (i = 0, p = cu->root->head; p; p = p->next, ++i) {
        cu->bc->def_addrs[i] = p->head->next->parse_state;
    }
}

static int ChpegCU_find_def(ChpegCU *cu, ChpegCNode *cident)
{
    for (int i = 0; i < cu->bc->num_defs; ++i) {
        if (cident->node->length == strlen(cu->bc->def_names[i]) &&
            memcmp(cu->input + cident->node->offset,
                cu->bc->def_names[i], cident->node->length) == 0)
        {
            return i;
        }
    }
    return -1;
}

static void ChpegCU_build_tree(ChpegCU *cu, ChpegNode *node, ChpegCNode *cnode)
{
    if (NULL == node) { node = cu->parser->tree_root; }
    if (NULL == cnode) { cnode = cu->root; }
    cnode->node = node;
    cnode->type = node->def;
    for (ChpegNode *p = node->head; p; p = p->next) {
        ChpegCNode *cnew = ChpegCNode_new();
        ChpegCU_build_tree(cu, p, cnew);
        ChpegCNode_push_child(cnode, cnew);
    }
}

static inline int ChpegCU_alloc_inst(ChpegCU *cu)
{
    return cu->bc->num_instructions++;
}

// reserve instructions and keep track of instruction addresses where references needed
// (referred to as state: parse_state, parent_next_state, etc.)
static void ChpegCU_alloc_instructions(ChpegCU *cu, ChpegCNode *cnode)
{
    switch (cnode->type) {
        case CHPEG_DEF_GRAMMAR:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
            }
            break;
        case CHPEG_DEF_DEFINITION:
            ChpegCU_alloc_instructions(cu, cnode->head->next);
            cnode->head->next->parent_next_state = ChpegCU_alloc_inst(cu);
            cnode->head->next->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_CHOICE:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
                p->parent_next_state = ChpegCU_alloc_inst(cu);
                p->parent_fail_state = ChpegCU_alloc_inst(cu);
            }
            ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                ChpegCU_alloc_instructions(cu, p);
            }
            cnode->parse_state = cnode->head->parse_state;
            break;
#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, cnode->head);
            cnode->head->parent_next_state = ChpegCU_alloc_inst(cu);
            cnode->head->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
#endif
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            cnode->parse_state = ChpegCU_alloc_inst(cu);                   // MARK
            ChpegCU_alloc_instructions(cu, cnode->head->next);             // ...
            cnode->head->next->parent_next_state = ChpegCU_alloc_inst(cu); // MARKS
            cnode->head->next->parent_fail_state = ChpegCU_alloc_inst(cu); // MARKF
            break;
#endif
        case CHPEG_DEF_REPEAT:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, cnode->head);
            cnode->head->parent_next_state = ChpegCU_alloc_inst(cu);
            cnode->head->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_PREDICATE:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_instructions(cu, cnode->head->next);
            cnode->head->next->parent_next_state = ChpegCU_alloc_inst(cu);
            cnode->head->next->parent_fail_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_DOT:
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            break;
        case CHPEG_DEF_IDENTIFIER:
        case CHPEG_DEF_CHARCLASS:
        case CHPEG_DEF_LITERAL:
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_REFERENCE:
#endif
            cnode->parse_state = ChpegCU_alloc_inst(cu);
            ChpegCU_alloc_inst(cu);
            break;
    }
}

#if CHPEG_EXTENSION_REFS
static int ChpegCU_find_ref(ChpegCU *cu, ChpegCNode *cnode);
#endif

static inline void ChpegCU_add_inst(ChpegCU *cu, int inst)
{
    cu->bc->instructions[cu->bc->num_instructions++] = inst;
}

static void ChpegCU_add_instructions(ChpegCU *cu, ChpegCNode *cnode)
{
    int def_id = 0;
#if CHPEG_EXTENSION_REFS
    int ref_id = 0;
#endif
    switch (cnode->type) {
        case CHPEG_DEF_GRAMMAR:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, 0));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_FAIL, 0));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_SUCC, 0));
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                ChpegCU_add_instructions(cu, p);
            }
            break;
        case CHPEG_DEF_DEFINITION:
            def_id = ChpegCU_find_def(cu, cnode->head);
            ChpegCU_add_instructions(cu, cnode->head->next);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_ISUCC, def_id));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IFAIL, def_id));
            break;
        case CHPEG_DEF_CHOICE:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHOICE, 0));
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                ChpegCU_add_instructions(cu, p);
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CISUCC, cnode->parent_next_state));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CIFAIL, 0));
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CFAIL, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : cnode->parent_next_state;
                p->parent_fail_state = cnode->parent_fail_state;
                ChpegCU_add_instructions(cu, p);
            }
            break;
#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            {
                ChpegCNode *def_node = ChpegCU_find_def_node(cu, cnode);
                def_id = def_node ? def_node->val.ival : 0;
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIM, def_id)); // def_id is informational only
            ChpegCU_add_instructions(cu, cnode->head);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMS, cnode->parent_next_state));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMF, cnode->parent_fail_state));
            break;
#endif
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            ref_id = ChpegCU_find_ref(cu, cnode);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARK, ref_id));
            ChpegCU_add_instructions(cu, cnode->head->next);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARKS, cnode->parent_next_state));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARKF, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_REFERENCE:
            ref_id = ChpegCU_find_ref(cu, cnode);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_REF, ref_id));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
#endif
        case CHPEG_DEF_REPEAT:
            {
                unsigned char op = cu->input[cnode->head->next->node->offset];
                switch (op) {
                    case '+':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPBEG, 0));
                        ChpegCU_add_instructions(cu, cnode->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPMAT, cnode->head->parse_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPDONE, cnode->parent_fail_state));
                        break;
                    case '*':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSBEG, 0));
                        ChpegCU_add_instructions(cu, cnode->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSMAT, cnode->head->parse_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSDONE, 0));
                        break;
                    case '?':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQBEG, 0));
                        ChpegCU_add_instructions(cu, cnode->head);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQMAT, 0));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQDONE, 0));
                        break;
                }
            }
            break;
        case CHPEG_DEF_PREDICATE:
            {
                unsigned char op = cu->input[cnode->head->node->offset];
                switch (op) {
                    case '&':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDA, 0));
                        ChpegCU_add_instructions(cu, cnode->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHS, cnode->parent_fail_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATF, cnode->parent_fail_state));
                        break;
                    case '!':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDN, 0));
                        ChpegCU_add_instructions(cu, cnode->head->next);
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHF, cnode->parent_fail_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATS, cnode->parent_fail_state));
                        break;
                }
            }
            break;
        case CHPEG_DEF_DOT:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_DOT, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_IDENTIFIER:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, ChpegCU_find_def(cu, cnode)));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_CHARCLASS:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHRCLS, cnode->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_LITERAL:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_LIT, cnode->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
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

static void ChpegCU_alloc_strings(ChpegCU *cu, ChpegCNode *cnode)
{
    if (!cnode) { cnode = cu->root; }

    switch (cnode->type) {
        case CHPEG_DEF_LITERAL:
        case CHPEG_DEF_CHARCLASS:
            {
                int len = 0, offset = 0;
                for (ChpegCNode *p = cnode->head; p; p = p->next) {
                    ChpegCU_alloc_strings(cu, p);
                    len += p->value_len;
                }
                unsigned char *str = (unsigned char *)CHPEG_MALLOC(len);
                for (ChpegCNode *p = cnode->head; p; p = p->next) {
                    memcpy(str+offset, p->val.cval, p->value_len);
                    offset += p->value_len;
                }
                cnode->val.ival = ChpegCU_alloc_string(cu, str, len);

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(str, len, 20);
                fprintf(stderr, "%s: LITERAL/CHARCLASS str_id=%d, value=\"%s\"\n",
                    __func__, cnode->val.ival, tmp);
                CHPEG_FREE(tmp);
#endif

                CHPEG_FREE(str);
            }
            break;
        case CHPEG_DEF_CHARRANGE:
            {
                for (ChpegCNode *p = cnode->head; p; p = p->next) {
                    ChpegCU_alloc_strings(cu, p);
                }
                cnode->val.cval[0] = cnode->head->val.cval[0];
                cnode->val.cval[1] = '-';
                cnode->val.cval[2] = cnode->head->next->val.cval[0];
                cnode->value_len = 3;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->value_len, 10);
                fprintf(stderr, "%s: CHARRANGE [%s]\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;
        case CHPEG_DEF_PLAINCHAR:
            {
                cnode->val.cval[0] = cu->input[cnode->node->offset];
                cnode->value_len = 1;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->value_len, 10);
                fprintf(stderr, "%s: PLAINCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;
        case CHPEG_DEF_ESCCHAR:
            {
                cnode->val.cval[0] = cu->input[cnode->node->offset + 1];
                cnode->value_len = 1;
                switch (cnode->val.cval[0]) {
                    case 'n': cnode->val.cval[0] = '\n'; break;
                    case 'r': cnode->val.cval[0] = '\r'; break;
                    case 't': cnode->val.cval[0] = '\t'; break;
                }

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->value_len, 10);
                fprintf(stderr, "%s: ESCCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;
        case CHPEG_DEF_OCTCHAR:
            {
                int val = 0; int len = cnode->node->length - 1;
                const unsigned char *ip = cu->input + cnode->node->offset + 1;
                for (int i = 0; i < len; ++i) {
                    val = (val << 3) | (ip[i] - '0');
                }
                cnode->val.cval[0] = val & 255;
                cnode->value_len = 1;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->value_len, 10);
                fprintf(stderr, "%s: OCTCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;
        default:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
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
static int ChpegCU_find_ref(ChpegCU *cu, ChpegCNode *cnode)
{
    ChpegCNode *cident = cnode->head;

    if (!cident) { return -2; }
    if (cident->type != CHPEG_DEF_IDENTIFIER) { return -2; }

    for (int i = 0; i < cu->bc->num_refs; ++i) {
        if (cident->node->length == strlen(cu->bc->refs[i]) &&
            memcmp(cu->input + cident->node->offset,
                cu->bc->refs[i], cident->node->length) == 0)
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
static int ChpegCU_alloc_ref(ChpegCU *cu, ChpegCNode *cnode)
{
    int ref = ChpegCU_find_ref(cu, cnode);
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

    assert (cnode->head && cnode->head->node);
    ChpegNode *ident = cnode->head->node;
    assert (ident->def == CHPEG_DEF_IDENTIFIER);

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

static int ChpegCU_alloc_refs(ChpegCU *cu, ChpegCNode *cnode)
{
    if (!cnode) { cnode = cu->root; }

    int err = 0;
    switch (cnode->type) {
        case CHPEG_DEF_MARK:
        case CHPEG_DEF_REFERENCE:
            {
                int ref_id = ChpegCU_alloc_ref(cu, cnode);

#if CHPEG_DEBUG_COMPILER
                const char *def_name = ChpegByteCode_def_name(chpeg_default_bytecode(), cnode->type);
                fprintf(stderr, "%s: (via %s) refs[%d]=\"%s\"\n", __func__, def_name,
                    ref_id, ref_id >= 0 ? cu->bc->refs[ref_id] : "<ERROR>");
#endif

                err = (ref_id < 0) ? ref_id : 0;
                goto done;
            }
        default:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
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

// Compiler sanity check: assert def names looked up via CHPEG_DEF_* macros
// match what is expected. This might help detect mismatches of included
// bytecode header vs. linked bytecode.
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

#if CHPEG_EXTENSION_TRIM
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_TRIM), "Trim") == 0);
#endif

#if CHPEG_EXTENSION_REFS
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

    cu.root = ChpegCNode_new();

    ChpegCU_build_tree(&cu, NULL, NULL);
    ChpegCNode_reverse(cu.root);

    if ((err = ChpegCU_setup_defs(&cu)) != 0) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    ChpegCU_alloc_strings(&cu, NULL);

#if CHPEG_EXTENSION_REFS
    if ((err = ChpegCU_alloc_refs(&cu, NULL)) != 0) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }
#endif

#if CHPEG_EXTENSION_TRIM
    ChpegCU_auto_leaf(&cu);
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
    if (err || !bytecode_return) {
        ChpegByteCode_free(cu.bc);
        cu.bc = NULL;
    }
    if (cu.parser) { ChpegParser_free(cu.parser); }
    if (cu.root) { ChpegCNode_free(cu.root); }

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

