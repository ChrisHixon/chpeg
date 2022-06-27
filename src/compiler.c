//
// chpeg: compiler.c {
//

#ifdef __cplusplus
extern "C" {
#endif

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

#if CHPEG_EXTENSION_MINMAX
#define CHPEG_MINMAX_MAX ((1<<23)-1)
#endif

//
// CNode: compilation unit node
//

enum ChpegCNodeBits
{
    CHPEG_CNODE_CONSUMES            = 1<<1,
    CHPEG_CNODE_CONSUME_RESOLVED    = 1<<2,
    CHPEG_CNODE_REFERENCED          = 1<<3,
};

typedef struct _ChpegCNode
{
    ChpegNode *node;
    int type;

    int parse_state;
    int parent_next_state;
    int parent_fail_state;

    // various use value storage
    union {
        unsigned char cval[4];
        int ival;
    } val;
    union {
        unsigned char cval[4];
        int ival;
    } val2;

    int bits; // various use

    int num_children;
    struct _ChpegCNode *head;
    struct _ChpegCNode *next;
    struct _ChpegCNode *parent;
} ChpegCNode;

static ChpegCNode *ChpegCNode_new(void)
{
    ChpegCNode *self = (ChpegCNode *)CHPEG_CALLOC(1, sizeof(ChpegCNode));

    self->type = -1;

    self->parse_state = -1;
    self->parent_next_state = -1;
    self->parent_fail_state = -1;

    self->val.ival = -1;

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
    int undefined_identifiers;
} ChpegCU;

static int ChpegCU_find_def(ChpegCU *cu, ChpegCNode *ident);

#if CHPEG_EXTENSION_TRIM
static ChpegCNode *ChpegCU_find_parent_def_node(ChpegCU *cu, ChpegCNode *cnode)
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

        // mark def_id as referenced, as it is 'referenced' as the start rule
        if (def_id == 0) {
            cdef->bits |= CHPEG_CNODE_REFERENCED;
        }

        // stash the def_id in the DEFINITION cnode
        cdef->val.ival = def_id;

        ChpegCNode *cident = cdef->head; // first child: Identifier: definition name
        assert (cident->type == CHPEG_DEF_IDENTIFIER);

        if (ChpegCU_find_def(cu, cident) >= 0) {
            size_t line, col;
            chpeg_line_col(cu->input, cident->node->offset, &line, &col);
            char *tmp = chpeg_esc_bytes(cu->input + cident->node->offset, cident->node->length, 0);
            fprintf(stderr, "input:%zu:%zu: Error: '%s' is already defined.\n",
                line, col, tmp);
            CHPEG_FREE(tmp);
            err = 1;
            goto done;
        }

        // also store the def_id in the identifier node
        cident->val.ival = def_id;

        cu->bc->def_names[def_id] = (char *)CHPEG_MALLOC(1 + cident->node->length);
        ++cu->bc->num_defs;

        memcpy(cu->bc->def_names[def_id], &cu->input[cident->node->offset], cident->node->length);
        cu->bc->def_names[def_id][cident->node->length] = '\0';

        // second child: Options / a.k.a. flags
        int flags = 0;
        if (cident->next->node->def == CHPEG_DEF_OPTIONS) {
            ChpegCNode *coptions = cident->next;

            for (int i = 0; i < (int)coptions->node->length; ++i) {
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
            if (cnode->head->next->type == CHPEG_DEF_REPOP) {
                cnode->parse_state = ChpegCU_alloc_inst(cu);
                ChpegCU_alloc_instructions(cu, cnode->head);
                cnode->head->parent_next_state = ChpegCU_alloc_inst(cu);
                cnode->head->parent_fail_state = ChpegCU_alloc_inst(cu);
            }
#if CHPEG_EXTENSION_MINMAX
            else if (cnode->head->next->type == CHPEG_DEF_MINMAX) {
                cnode->parse_state = ChpegCU_alloc_inst(cu);
                ChpegCU_alloc_instructions(cu, cnode->head);
                cnode->head->parent_next_state = ChpegCU_alloc_inst(cu);
                cnode->head->parent_fail_state = ChpegCU_alloc_inst(cu);
                ChpegCU_alloc_inst(cu);
            }
#endif
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
#if CHPEG_EXTENSION_NCHRCLS
        case CHPEG_DEF_NCHARCLASS:
#endif
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

static int ChpegCU_add_instructions(ChpegCU *cu, ChpegCNode *cnode)
{
    int err = 0;

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
                if ((err = ChpegCU_add_instructions(cu, p)) != 0) {
                    goto done;
                }
            }
            break;
        case CHPEG_DEF_DEFINITION:
            def_id = cnode->val.ival;
            assert(def_id >= 0 && def_id < cu->bc->num_defs);
            if ((err = ChpegCU_add_instructions(cu, cnode->head->next)) != 0) {
                goto done;
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_ISUCC, def_id));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IFAIL, def_id));
            break;
        case CHPEG_DEF_CHOICE:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHOICE, 0));
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                if ((err = ChpegCU_add_instructions(cu, p)) != 0) {
                    goto done;
                }
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CISUCC, cnode->parent_next_state));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CIFAIL, 0));
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CFAIL, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_SEQUENCE:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : cnode->parent_next_state;
                p->parent_fail_state = cnode->parent_fail_state;
                if ((err = ChpegCU_add_instructions(cu, p)) != 0) {
                    goto done;
                }

            }
            break;
#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            {
                ChpegCNode *def_node = ChpegCU_find_parent_def_node(cu, cnode);
                assert(def_node);
                def_id = def_node->val.ival;
                assert(def_id >= 0 && def_id < cu->bc->num_defs);
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIM, def_id)); // def_id is informational only
            if ((err = ChpegCU_add_instructions(cu, cnode->head)) != 0) {
                goto done;
            }
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMS, cnode->parent_next_state));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_TRIMF, cnode->parent_fail_state));
            break;
#endif
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            ref_id = ChpegCU_find_ref(cu, cnode);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_MARK, ref_id));
            if ((err = ChpegCU_add_instructions(cu, cnode->head->next)) != 0) {
                goto done;
            }
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
            if (cnode->head->next->type == CHPEG_DEF_REPOP) {
                unsigned char op = cu->input[cnode->head->next->node->offset];
                switch (op) {
                    case '+':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPBEG, 0));
                        if ((err = ChpegCU_add_instructions(cu, cnode->head)) != 0) {
                            goto done;
                        }
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPMAT, cnode->head->parse_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RPDONE, cnode->parent_fail_state));
                        break;
                    case '*':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSBEG, 0));
                        if ((err = ChpegCU_add_instructions(cu, cnode->head)) != 0) {
                            goto done;
                        }
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSMAT, cnode->head->parse_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RSDONE, 0));
                        break;
                    case '?':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQBEG, 0));
                        if ((err = ChpegCU_add_instructions(cu, cnode->head)) != 0) {
                            goto done;
                        }
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQMAT, 0));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RQDONE, 0));
                        break;
                }
            }
#if CHPEG_EXTENSION_MINMAX
            else if (cnode->head->next->type == CHPEG_DEF_MINMAX) {
#if CHPEG_DEBUG_COMPILER
                fprintf(stderr, "%s: MINMAX: offset=%zu min=%d, max=%d\n", __func__,
                    cnode->head->next->head->node->offset,
                    cnode->val.ival, cnode->val2.ival);
#endif
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RMMBEG, cnode->val.ival));
                if ((err = ChpegCU_add_instructions(cu, cnode->head)) != 0) {
                    goto done;
                }
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RMMMAT, cnode->val2.ival));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_RMMDONE, cnode->val2.ival));
                ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
                assert(cu->bc->num_instructions == cnode->parent_next_state);
                break;
            }
#endif
            break;
        case CHPEG_DEF_PREDICATE:
            {
                unsigned char op = cu->input[cnode->head->node->offset];
                switch (op) {
                    case '&':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDA, 0));
                        if ((err = ChpegCU_add_instructions(cu, cnode->head->next)) != 0) {
                            goto done;
                        }
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHS, cnode->parent_fail_state));
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATF, cnode->parent_fail_state));
                        break;
                    case '!':
                        ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_PREDN, 0));
                        if ((err = ChpegCU_add_instructions(cu, cnode->head->next)) != 0) {
                            goto done;
                        }
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
            def_id = cnode->val.ival;
            assert(def_id >= 0 && def_id < cu->bc->num_defs);
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, def_id));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
        case CHPEG_DEF_CHARCLASS:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_CHRCLS, cnode->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
#if CHPEG_EXTENSION_NCHRCLS
        case CHPEG_DEF_NCHARCLASS:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_NCHRCLS, cnode->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
#endif
        case CHPEG_DEF_LITERAL:
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_LIT, cnode->val.ival));
            ChpegCU_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, cnode->parent_fail_state));
            break;
    }

done:
    return err;
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
#if CHPEG_EXTENSION_NCHRCLS
        case CHPEG_DEF_NCHARCLASS:
#endif
            {
                int len = 0, offset = 0;
                for (ChpegCNode *p = cnode->head; p; p = p->next) {
                    ChpegCU_alloc_strings(cu, p);
                    len += p->val.cval[3];
                }
                unsigned char *str = (unsigned char *)CHPEG_MALLOC(len);
                for (ChpegCNode *p = cnode->head; p; p = p->next) {
                    memcpy(str+offset, p->val.cval, p->val.cval[3]);
                    offset += p->val.cval[3];
                }
                cnode->val.ival = ChpegCU_alloc_string(cu, str, len);

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(str, len, 20);
                fprintf(stderr, "%s: LITERAL/CHARCLASS/NCHARCLASS str_id=%d, value=\"%s\"\n",
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
                cnode->val.cval[3] = 3;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->val.cval[3], 0);
                fprintf(stderr, "%s: CHARRANGE [%s]\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;

        case CHPEG_DEF_PLAINCHAR:
            {
                cnode->val.cval[0] = cu->input[cnode->node->offset];
                cnode->val.cval[3] = 1;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->val.cval[3], 0);
                fprintf(stderr, "%s: PLAINCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;

        case CHPEG_DEF_ESCCHAR:
            {
                cnode->val.cval[0] = cu->input[cnode->node->offset + 1];
                cnode->val.cval[3] = 1;
                switch (cnode->val.cval[0]) {
                    case 'n': cnode->val.cval[0] = '\n'; break;
                    case 't': cnode->val.cval[0] = '\t'; break;
                    case 'r': cnode->val.cval[0] = '\r'; break;
#if CHPEG_EXTENSION_ESCAPES
                    case 'f': cnode->val.cval[0] = '\f'; break;
                    case 'v': cnode->val.cval[0] = '\v'; break;
                    case 'a': cnode->val.cval[0] = '\a'; break;
                    case 'b': cnode->val.cval[0] = '\b'; break;
#endif
                    case '\'':
                    case '"':
                    case '[':
                    case ']':
                    case '\\':
                              break;
                    default:
                              chpeg_abort("unhandled ESCCHAR case: '%c'\n",
                                  cnode->val.cval[0]);
                              break;
                }

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->val.cval[3], 0);
                fprintf(stderr, "%s: ESCCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;

        case CHPEG_DEF_OCTCHAR:
            {
                int len = cnode->node->length - 1;
                unsigned int val = 0;
                const unsigned char *ip = cu->input + cnode->node->offset + 1;
                assert(len >= 1 && len <= 3);
                for (int i = 0; i < len; ++i) {
                    val = (val << 3) | (ip[i] - '0');
                }
                assert (val < 256);
                cnode->val.cval[0] = val;
                cnode->val.cval[3] = 1;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->val.cval[3], 0);
                fprintf(stderr, "%s: OCTCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;

#if CHPEG_EXTENSION_HEX
        case CHPEG_DEF_HEXCHAR:
            {
                int len = cnode->node->length - 2;
                unsigned char val = 0;
                const unsigned char *ip = cu->input + cnode->node->offset + 2;
                assert(len >= 1 && len <= 2);
                for (int i = 0; i < len; ++i) {
                    switch(ip[i]) {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            val = (val << 4) | (ip[i] - '0');
                            break;
                        case 'a': case 'b': case 'c':
                        case 'd': case 'e': case 'f':
                            val = (val << 4) | (10 + ip[i] - 'a');
                            break;
                        case 'A': case 'B': case 'C':
                        case 'D': case 'E': case 'F':
                            val = (val << 4) | (10 + ip[i] - 'A');
                            break;
                        default:
                            chpeg_abort("unhandled HEXCHAR case: '%c'\n", ip[i]);
                            break;
                    }
                }
                cnode->val.cval[0] = val;
                cnode->val.cval[3] = 1;

#if CHPEG_DEBUG_COMPILER
                char *tmp = chpeg_esc_bytes(cnode->val.cval, cnode->val.cval[3], 0);
                fprintf(stderr, "%s: HEXCHAR '%s'\n", __func__, tmp);
                CHPEG_FREE(tmp);
#endif

            }
            break;
#endif

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

static ChpegCNode *ChpegCU_find_def_node(ChpegCU *cu, int def_id)
{
    for (ChpegCNode *cdef = cu->root->head; cdef; cdef = cdef->next) {
        if (cdef->val.ival == def_id) {
            return cdef;
        }
    }
    return NULL;
}

#if CHPEG_EXTENSION_MINMAX
// helper function to convert a MinMaxVal to int, and print an error message upon error
static inline int minmax_value(ChpegCU *cu, ChpegCNode *cnode, int *val_out)
{
    int err = 0;
    unsigned int val = 0;

    assert(cnode->type == CHPEG_DEF_MINMAXVAL);
    if ( (err = chpeg_bytes2uint(cu->input + cnode->node->offset,
        cnode->node->length, CHPEG_MINMAX_MAX, &val)) != 0)
    {
        size_t line, col;
        chpeg_line_col(cu->input, cnode->node->offset, &line, &col);
        assert(err == 4);
        fprintf(stderr, "input:%zu:%zu: Error: invalid value in { min, max }.\n", line, col);
    }
    assert(val_out);
    *val_out = val;
    return err;
}
#endif

// Set up values from the parsed input; detect some errors in the process.
// - mark identifiers with the definition id they refer to
// - detects undefined identifiers
// - mark definitions as referenced
// - set min/max values for repeat {min, max} extension
static int ChpegCU_setup_values(ChpegCU *cu, ChpegCNode *cnode)
{
    int err = 0;

    if (!cnode) { cnode = cu->root; }

    switch (cnode->type) {
        case CHPEG_DEF_DEFINITION:
            // This is an override of default case that recursively explores the AST...
            // We want to explore the rule definition (cnode->head->next), but not
            // the definition name Identifier (cnode->head).
            if ( (err = ChpegCU_setup_values(cu, cnode->head->next)) != 0) {
                goto done;
            }
            break;
        case CHPEG_DEF_IDENTIFIER:
            {
                int def_id = ChpegCU_find_def(cu, cnode);
                if (def_id < 0) {
                    char *tmp = chpeg_esc_bytes(cu->input + cnode->node->offset,
                        cnode->node->length, 0);
                    size_t line, col;
                    chpeg_line_col(cu->input, cnode->node->offset, &line, &col);
                    fprintf(stderr, "input:%zu:%zu: Error: Identifier '%s' is not defined.\n",
                        line, col, tmp);
                    CHPEG_FREE(tmp);
                    cu->undefined_identifiers++;
                    break;
                }

                // store the def_id in the identifier node
                assert(def_id < cu->bc->num_defs);
                cnode->val.ival = def_id;

                // mark the definition as referenced
                ChpegCNode *cdef = ChpegCU_find_def_node(cu, def_id);
                assert(cdef);
                cdef->bits |= CHPEG_CNODE_REFERENCED;
            }
            break;
#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            // cnode->head IDENTIFIER refers to a reference, not a definition
            // explore MARK content in cnode->head->next
            if ( (err = ChpegCU_setup_values(cu, cnode->head->next)) != 0) {
                goto done;
            }
            break;
        case CHPEG_DEF_REFERENCE:
            // this IDENTIFIER refers to a reference, not a definition
            break;
#endif

        case CHPEG_DEF_REPEAT:
            if ( (err = ChpegCU_setup_values(cu, cnode->head)) != 0) {
                goto done;
            }
#if CHPEG_EXTENSION_MINMAX
            if (cnode->head->next->type == CHPEG_DEF_MINMAX) {
#if CHPEG_DEBUG_COMPILER
                fprintf(stderr, "%s: MINMAX nc=%d\n", __func__, cnode->head->next->num_children);
#endif
                int min = 0, max = 0;
                ChpegCNode *cminmax = cnode->head->next;

                // { Num }: min = max = Num
                if (cminmax->num_children == 1) {
                    if ( (err = minmax_value(cu, cminmax->head, &min)) != 0) {
                        goto done;
                    }
                    max = min;
                }
                else if (cminmax->num_children == 2) {
                    // { , Num }: min = 0; max = Num
                    if (cminmax->head->type == CHPEG_DEF_COMMA)
                    {
                        min = 0;
                        if ( (err = minmax_value(cu, cminmax->head->next, &max)) != 0) {
                            goto done;
                        }
                    }
                    // { Num , }: min = Num; max = -1
                    else if (cminmax->head->next->type == CHPEG_DEF_COMMA)
                    {
                        max = -1;
                        if ( (err = minmax_value(cu, cminmax->head, &min)) != 0) {
                            goto done;
                        }
                    }
                }
                // { Min , Max }
                else if (cminmax->num_children == 3) {
                    assert(cminmax->head->next->type == CHPEG_DEF_COMMA);
                    if ( (err = minmax_value(cu, cminmax->head, &min)) != 0) {
                        goto done;
                    }
                    if ( (err = minmax_value(cu, cminmax->head->next->next, &max)) != 0) {
                        goto done;
                    }
                    if (max < min) {
                        size_t line, col;
                        chpeg_line_col(cu->input, cminmax->head->next->next->node->offset,
                            &line, &col);
                        fprintf(stderr, "input:%zu:%zu: Error: max < min in { min, max }.\n",
                            line, col);
                        err = CHPEG_ERR_COMPILE;
                        goto done;
                    }
                }
                else {
                    chpeg_abort("%s\n", "unhandled MINMAX case");
                }

                // store the min/max values in the REPEAT node
                cnode->val.ival = min;
                cnode->val2.ival = max;
            }
            break;
#endif

        default:
            for (ChpegCNode *p = cnode->head; p; p = p->next) {
                if ( (err = ChpegCU_setup_values(cu, p)) != 0) {
                    goto done;
                }
            }
            break;
    }

done:
    return err;
}

static int ChpegCU_warn_unreferenced(ChpegCU *cu)
{
    for (ChpegCNode *cdef = cu->root->head; cdef; cdef = cdef->next) {
        assert(cdef->type == CHPEG_DEF_DEFINITION);
        if (!(cdef->bits & CHPEG_CNODE_REFERENCED)) {
            size_t line, col;
            chpeg_line_col(cu->input, cdef->node->offset, &line, &col);
            fprintf(stderr, "input:%zu:%zu: Warning: Definition '%s' is not referenced.\n",
                line, col, ChpegByteCode_def_name(cu->bc, cdef->val.ival));
        }
    }
    return 0;
}


// TODO: clean up left recursion stuff, refactor, deal with errors in a better way.
// We could easily show the whole ident path that led to left recursion using the stack.
#define CHPEG_LR_STACK_SIZE 4096
typedef struct _ChpegLR {
    ChpegByteCode *bc; // for debugging print
    ChpegCNode *cnodes[CHPEG_LR_STACK_SIZE];
    int consumes[CHPEG_LR_STACK_SIZE];
    int top;
    int errors;
} ChpegLR;

#if CHPEG_DEBUG_LR
static void ChpegLR_show(ChpegLR *lr)
{
    fprintf(stderr, "LR: ");
    for (int i = 1; i <= lr->top; ++i) {
        if (lr->cnodes[i]->type == CHPEG_DEF_IDENTIFIER) {
            fprintf(stderr, "| %s:%s %d ",
                ChpegByteCode_def_name(chpeg_default_bytecode(), lr->cnodes[i]->type),
                ChpegByteCode_def_name(lr->bc, lr->cnodes[i]->val.ival),
                lr->consumes[i]);
        }
        else {
            fprintf(stderr, "| %s %d ",
                ChpegByteCode_def_name(chpeg_default_bytecode(), lr->cnodes[i]->type),
                lr->consumes[i]);
        }
    }
    fprintf(stderr, "\n");
}
#endif

static void ChpegLR_push(ChpegLR *lr, ChpegCNode *cnode, int consumes)
{
    ++lr->top;
    assert(lr->top >= 0 && lr->top < CHPEG_LR_STACK_SIZE);
    lr->cnodes[lr->top] = cnode;
    lr->consumes[lr->top] = consumes;
#if CHPEG_DEBUG_LR >= 3
    ChpegLR_show(lr);
#endif
}

static void ChpegLR_pop(ChpegLR *lr)
{
    --lr->top;
    assert(lr->top >= 0 && lr->top < CHPEG_LR_STACK_SIZE);
#if CHPEG_DEBUG_LR >= 3
    ChpegLR_show(lr);
#endif
}

// Does cnode consume, in all cases? Detects left recursion and infinite loops.
// Note: we need to visit all child nodes even if we know whether or not something consumes.
// returns: 1 if consumes, 0 if doesn't consume
static int ChpegCU_consumes(ChpegCU *cu, ChpegCNode *cnode, ChpegLR *lr)
{
#if CHPEG_DEBUG_LR >= 2
    fprintf(stderr, "%s (BEGIN): %s (resolved=%d)\n", __func__, ChpegByteCode_def_name(
            chpeg_default_bytecode(), cnode->type),
            !!(cnode->bits & CHPEG_CNODE_CONSUME_RESOLVED));
#endif

    // Have we already resolved whether or not this consumes?
    // If so, return result. This is necessary on complex grammars, or we'll spend
    // all day trying every possible choice combo/path.
    if (cnode->bits & CHPEG_CNODE_CONSUME_RESOLVED) {
        return !!(cnode->bits & CHPEG_CNODE_CONSUMES);
    }

    int consumes = 0, def_id = 0;

    switch (cnode->type) {
        case CHPEG_DEF_IDENTIFIER:
            {
                def_id = cnode->val.ival;
                assert(def_id >= 0 && def_id < cu->bc->num_defs);

                // check if RESOLVED already, via the def node
                ChpegCNode *cdef = ChpegCU_find_def_node(cu, def_id);
                assert(cdef);

                if (cdef->bits & CHPEG_CNODE_CONSUME_RESOLVED) {
#if CHPEG_DEBUG_LR >= 1
                    fprintf(stderr, "IDENTIFIER %s(%d) RESOLVED\n",
                        ChpegByteCode_def_name(cu->bc, def_id), def_id);
#endif
                    return !!(cdef->bits & CHPEG_CNODE_CONSUMES);
                }

#if CHPEG_DEBUG_LR >= 1
                fprintf(stderr, "IDENTIFIER %s(%d)\n",
                    ChpegByteCode_def_name(cu->bc, def_id), def_id);
#if CHPEG_DEBUG_LR >= 3
                ChpegLR_show(lr);
#endif
#endif

                // Look back into the stack and see if this ident exists.
                // If so, and there was no consumption between that point and now,
                // we have left recursion.
                int consumed = 0;
                // scan backwards
                for (int i = lr->top; i > 0; --i) {
                    assert(lr->cnodes[i]);
                    // found matching IDENT
                    if (lr->cnodes[i]->type == CHPEG_DEF_IDENTIFIER &&
                        lr->cnodes[i]->val.ival == def_id)
                    {
                        // scan forward looking for consuming
                        for (int j = i; j <= lr->top; ++j) {
                            if (lr->consumes[j] > 0) {
                                consumed = 1; break;
                            }
                        }
                        if (consumed == 0) {
                            size_t line, col;
                            chpeg_line_col(cu->input, cnode->node->offset, &line, &col);
                            fprintf(stderr, "input:%zu:%zu: Error: Left recursion detected in '%s'.\n",
                                line, col, ChpegByteCode_def_name(cu->bc, def_id));
                            lr->errors++;
#if CHPEG_DEBUG_LR >= 1
                            ChpegLR_show(lr);
#endif

                            consumes = 0;
                        }
                        else {
#if CHPEG_DEBUG_LR >= 1
                            // not an error
                            fprintf(stderr, "recursion with consume detected in %s at offset %zu\n",
                                ChpegByteCode_def_name(cu->bc, def_id), cnode->node->offset);
#endif
                            consumes = 1;
                        }
                        goto done;
                    }
                }


                ChpegLR_push(lr, cnode, 0);
                consumes = ChpegCU_consumes(cu, cdef->head->next, lr);
                ChpegLR_pop(lr);

                // record RESOLVED state in the definition node
                assert(!(cdef->bits & CHPEG_CNODE_CONSUME_RESOLVED));
                cdef->bits |= CHPEG_CNODE_CONSUME_RESOLVED;
                cdef->bits &= ~CHPEG_CNODE_CONSUMES;
                cdef->bits |= (consumes ? CHPEG_CNODE_CONSUMES : 0);

#if CHPEG_DEBUG_LR >= 1
                    fprintf(stderr, "DEF %s(%d) MARKED RESOLVED\n",
                        ChpegByteCode_def_name(cu->bc, def_id), def_id);
#endif

            }
            break;

        case CHPEG_DEF_CHOICE:
            // if any choice branches do not consume, choice does not consume...
            ChpegLR_push(lr, cnode, 0);
            consumes = 1;
            for (ChpegCNode * child = cnode->head; child; child = child->next) {
                if (!ChpegCU_consumes(cu, child, lr)) {
                    consumes = 0;
                }
                lr->consumes[lr->top] = 0; // reset consume tracking for each branch
            }
            ChpegLR_pop(lr);
            break;

        case CHPEG_DEF_SEQUENCE:
            // if any parts of sequence consume, sequence consumes...
            consumes = 0;
            for (ChpegCNode * child = cnode->head; child; child = child->next) {
                if ( (ChpegCU_consumes(cu, child, lr)) != 0) {
                    consumes = 1;
                    lr->consumes[lr->top] = 1;
                }
            }
            break;

#if CHPEG_EXTENSION_TRIM
        case CHPEG_DEF_TRIM:
            // is push/pop necessary in this case?
            ChpegLR_push(lr, cnode, 0);
            consumes = ChpegCU_consumes(cu, cnode->head, lr);
            ChpegLR_pop(lr);
            break;
#endif

#if CHPEG_EXTENSION_REFS
        case CHPEG_DEF_MARK:
            // is push/pop necessary in this case?
            ChpegLR_push(lr, cnode, 0);
            consumes = ChpegCU_consumes(cu, cnode->head->next, lr);
            ChpegLR_pop(lr);
            break;

        case CHPEG_DEF_REFERENCE:
            consumes = 1;
            break;
#endif

        case CHPEG_DEF_REPEAT:
            ChpegLR_push(lr, cnode, 0);
            consumes = ChpegCU_consumes(cu, cnode->head, lr);
            ChpegLR_pop(lr);

            if (cnode->head->next->type == CHPEG_DEF_REPOP) {
                unsigned char op = cu->input[cnode->head->next->node->offset];
                // infinite loop if no consumption and repeat has no maximum
                // (ops * and +, but not ?)
                if (!consumes && op != '?') {
                    size_t line, col;
                    chpeg_line_col(cu->input, cnode->node->offset, &line, &col);
                    fprintf(stderr, "input:%zu:%zu: Error: Infinite loop detected.\n",
                        line, col);
                    lr->errors++;
                    consumes = 0;
                    break;
                }
                // this REPEAT consumes only if minimum > 0, which is only '+' operator
                consumes = (op == '+');
            }

#if CHPEG_EXTENSION_MINMAX
            else if (cnode->head->next->type == CHPEG_DEF_MINMAX) {
                // infinite loop if no consumption and repeat has no maximum
                // (val2.ival == -1 means 'no maximum')
                if (!consumes && cnode->val2.ival < 0) {
                    size_t line, col;
                    chpeg_line_col(cu->input, cnode->node->offset, &line, &col);
                    fprintf(stderr, "input:%zu:%zu: Error: Infinite loop detected.\n",
                        line, col);
                    lr->errors++;
                    consumes = 0;
                    break;
                }
                // this REPEAT consumes only if minimum > 0
                consumes = (cnode->val.ival > 0);
            }
#endif
            else {
                chpeg_abort("%s\n", "unhandled REPEAT case");
            }
            break;

        case CHPEG_DEF_PREDICATE:
            ChpegLR_push(lr, cnode, 0);
            ChpegCU_consumes(cu, cnode->head->next, lr);
            ChpegLR_pop(lr);
            consumes = 0;
            break;

        case CHPEG_DEF_DOT:
        case CHPEG_DEF_CHARCLASS:
#if CHPEG_EXTENSION_NCHRCLS
        case CHPEG_DEF_NCHARCLASS:
#endif
            consumes = 1;
            break;

        case CHPEG_DEF_LITERAL:
            // we haven't processed strings yet... but an empty string has no child nodes
            consumes = (cnode->node->num_children > 0);
            break;

        default:
            chpeg_abort("unhandled cnode->type case: %d\n", cnode->type);
            break;
    }

done:
    assert(!(cnode->bits & CHPEG_CNODE_CONSUME_RESOLVED));
    cnode->bits |= CHPEG_CNODE_CONSUME_RESOLVED;
    cnode->bits &= ~CHPEG_CNODE_CONSUMES;
    cnode->bits |= (consumes ? CHPEG_CNODE_CONSUMES : 0);

#if CHPEG_DEBUG_LR >= 2
    fprintf(stderr, "%s (END) %s (resolved=%d) consumes=%d\n", __func__, ChpegByteCode_def_name(
            chpeg_default_bytecode(), cnode->type),
            !!(cnode->bits & CHPEG_CNODE_CONSUME_RESOLVED), consumes);
#endif
    return consumes;
}

static int ChpegCU_detect_left_recursion(ChpegCU *cu)
{
    ChpegLR lr;
    memset(&lr, 0, sizeof(ChpegLR));

    lr.bc = cu->bc;

    for (ChpegCNode *cdef = cu->root->head; cdef; cdef = cdef->next) {
        assert(lr.top == 0);
#if CHPEG_DEBUG_LR >= 1
        fprintf(stderr, "GRAMMAR DEF %s(%d)\n",
            ChpegByteCode_def_name(cu->bc, cdef->val.ival), cdef->val.ival);
#endif
        ChpegCU_consumes(cu, cdef->head, &lr); // send it identifier (first child)
    }

    return lr.errors;
}

#ifndef NDEBUG
// Compiler sanity check: assert def names looked up via CHPEG_DEF_* macros
// match what is expected. This might help detect mismatches of included
// bytecode header vs. linked bytecode.
static void chpeg_sanity_check(void)
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

#if CHPEG_EXTENSION_HEX
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_HEXCHAR), "HexChar") == 0);
#endif

#if CHPEG_EXTENSION_NCHRCLS
    assert(strcmp(ChpegByteCode_def_name(bc, CHPEG_DEF_NCHARCLASS), "NCharClass") == 0);
#endif

}
#endif


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
CHPEG_DEF int chpeg_compile(const unsigned char *input, size_t length,
    ChpegByteCode **bytecode_return, int verbose)
{
#ifndef NDEBUG
    chpeg_sanity_check();
#endif

    int err = 0;
    ChpegCU cu;
    memset(&cu, 0, sizeof(ChpegCU));
    cu.parser = ChpegParser_new(chpeg_default_bytecode());
    cu.input = input;

    // with current chpeg grammar(s) and compiler implementation, need to use
    // original unwrap simplification method to get the AST the compiler
    // expects.
    cu.parser->simplification = 1;

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

    if (! (cu.bc = ChpegByteCode_new()) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    if (! (cu.root = ChpegCNode_new()) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    ChpegCU_build_tree(&cu, NULL, NULL);
    ChpegCNode_reverse(cu.root);

    if ( (err = ChpegCU_setup_defs(&cu)) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    if ( (err = ChpegCU_setup_values(&cu, NULL)) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    if ( (err = ChpegCU_warn_unreferenced(&cu)) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    if (cu.undefined_identifiers) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    if ( (err = ChpegCU_detect_left_recursion(&cu)) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }

    ChpegCU_alloc_strings(&cu, NULL);

#if CHPEG_EXTENSION_REFS
    if ( (err = ChpegCU_alloc_refs(&cu, NULL)) ) {
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
    if ( (err = ChpegCU_add_instructions(&cu, cu.root)) ) {
        err = CHPEG_ERR_COMPILE;
        goto done;
    }


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

CHPEG_DEF const ChpegByteCode *chpeg_default_bytecode(void)
{
#if CHPEG_USES_EXTENSIONS
    return &chpeg_ext_bytecode;
#else
    return &chpeg_bytecode;
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: compiler.c

