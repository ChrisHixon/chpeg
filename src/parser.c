//
// chpeg: parser.c {
//

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/util.h"
#include "chpeg/parser.h"
#endif

//
// ChpegNode
//

void ChpegNode_print(ChpegNode *self, ChpegParser *parser, const unsigned char *input, int depth, FILE *fp)
{
    if (!self) { return; }

    char flags[CHPEG_FLAGS_DISPLAY_SIZE];
    char *data = chpeg_esc_bytes(&input[self->offset], self->length, 40);
    const char *def_name = ChpegByteCode_def_name(parser->bc, self->def);

    if (depth == 0) {
        fprintf(fp,
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REF_COUNT
            "     "
#endif
            " OFFSET   LEN     ID NC FLG IDENT \"DATA\"\n");
    }

    fprintf(fp,
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REF_COUNT
        "%4d "
#endif
        "%6zu %6zu %6d %2d %3s %*s%s \"%s\"\n",
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REF_COUNT
        self->ref_count,
#endif
        self->offset,
        self->length,
        self->def,
        self->num_children,
        chpeg_flags(flags, self->flags),
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
    ChpegNode *self = (ChpegNode *)CHPEG_CALLOC(1, sizeof(ChpegNode));
    self->def = def;

#if CHPEG_EXTENSION_TRIM
    self->match_offset = offset;
    self->match_length = length;
#else
    self->offset = offset;
    self->length = length;
#endif

    self->flags = flags;
#if CHPEG_NODE_REF_COUNT
    self->ref_count = 1;
#endif
    return self;
}

#if CHPEG_EXTENSION_REFS
static inline ChpegReferenceInfo *ChpegNode_alloc_refs(ChpegNode *self, int num_refs)
{
    if (num_refs) {
        self->refs = (ChpegReferenceInfo *)CHPEG_CALLOC(num_refs, sizeof(ChpegReferenceInfo));
        return self->refs;
    }
    return NULL;
}
#endif


#if 0
ChpegNode *ChpegNode_copy(ChpegNode *other)
{
    ChpegNode *self = (ChpegNode *)CHPEG_MALLOC(sizeof(ChpegNode));
    memcpy(self, other, sizeof(ChpegNode));
#if CHPEG_NODE_REF_COUNT
    self->ref_count = 1;
#endif
    self->next = NULL;
    return self;
}
#endif

void ChpegUndoAction_free(ChpegUndoAction *self)
{
#if CHPEG_NODE_REF_COUNT
        assert(self->ref_count > 0);
        if (--self->ref_count == 0) {
#endif
            if (self->data) {
                CHPEG_FREE(self->data);
            }
            CHPEG_FREE(self);
#if CHPEG_NODE_REF_COUNT
        }
#endif
}

// Actually free a node, ignoring reference count.
// Don't call this directly.
// Instead, call one of:
//   void ChpegNode_free(ChpegNode *self)
//   void ChpegNode_free_nr(ChpegNode *self)
void ChpegNode__free__(ChpegNode *self)
{
    if (self) {
#if CHPEG_UNDO
        ChpegUndoAction *action, *tmp;
        for (action = self->undo_action; action; action = tmp) {
            tmp = action->next;
            if (action->data) {
                CHPEG_FREE(action->data);
            }
            CHPEG_FREE(action);
        }
#endif
#if CHPEG_EXTENSION_REFS
        if (self->refs) {
            CHPEG_FREE(self->refs);
            self->refs = NULL;
        }
#endif
        CHPEG_FREE(self);
    }
}

// clear (free) all children recursively
void ChpegNode_clear(ChpegNode *self)
{
    if (self) {
        ChpegNode *tmp;
        for (ChpegNode *p = self->head; p; p = tmp) {
            tmp = p->next;
            ChpegNode_free(p);
        }
        self->head = NULL;
        self->num_children = 0;
    }
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

#if CHPEG_UNDO
        ChpegUndoAction *action, *atmp;
        for (action = self->undo_action; action; action = atmp) {
            atmp = action->next;
            ChpegUndoAction_free(action);
        }
        self->undo_action = NULL;
#endif

#if CHPEG_NODE_REF_COUNT
        assert(self->ref_count > 0);
        if (--self->ref_count == 0) {
#endif
            ChpegNode__free__(self);
#if CHPEG_NODE_REF_COUNT
        }
#endif
    }
}

// free one node, non-recursively
void ChpegNode_free_nr(ChpegNode *self)
{
    if (self) {
#if CHPEG_UNDO
        ChpegUndoAction *action, *atmp;
        for (action = self->undo_action; action; action = atmp) {
            atmp = action->next;
            ChpegUndoAction_free(action);
        }
        self->undo_action = NULL;
#endif

#if CHPEG_NODE_REF_COUNT
        assert(self->ref_count > 0);
        if (--self->ref_count == 0) {
#endif
            ChpegNode__free__(self);
#if CHPEG_NODE_REF_COUNT
        }
#endif
    }
}

ChpegNode *ChpegNode_push_child(ChpegNode *self, ChpegNode *node)
{
    node->parent = self;
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

#if CHPEG_UNDO
void ChpegNode_add_undo(ChpegNode *self, ChpegUndoFunction func,
    void *data, size_t data_size)
{
    ChpegUndoAction *action = (ChpegUndoAction *)CHPEG_CALLOC(1, sizeof(ChpegUndoAction));
    assert(func);
#if CHPEG_NODE_REF_COUNT
    action->ref_count = 1;
#endif
    action->func = func;
    action->data = data;
    action->data_size = data_size;
    action->next = self->undo_action;
    self->undo_action = action;
    self->num_undo++;
}

// recursively perform undo actions in nodes, deepest first, then free nodes
void ChpegNode_undo_free(ChpegNode *self, int depth)
{
    if (self) {
        ChpegNode *tmp;
        for (ChpegNode *p = self->head; p; p = tmp) {
            tmp = p->next;
            ChpegNode_undo_free(p, depth + 1);
        }
        self->head = NULL;

#if 0
        fprintf(stderr, "ChpegNode_undo_free: UNDO depth=%d <d:%d o:%zu l:%zu f:%d>\n",
            depth, self->def, self->offset, self->length, self->flags);
#endif

        ChpegUndoAction *action;
        for (action = self->undo_action; action; action = action->next) {
            assert(action->func);
            action->func(self, action->data);
        }
        ChpegNode_free_nr(self);
    }
}

// drop-in replacement for ChpegNode_pop_child that calls undo actions
// (use only when something fails and we do backtrack)
void ChpegNode_pop_child_undo(ChpegNode *self)
{
    if (self->head) {
        ChpegNode *tmp = self->head;
        self->head = self->head->next;
        ChpegNode_undo_free(tmp, 0);
        --(self->num_children);
    }
}

// pop the undo stack and run the undo action; free action afterwards
void ChpegNode_pop_undo(ChpegNode *self)
{
    if (self->undo_action) {
        ChpegUndoAction *action = self->undo_action;
        self->undo_action = self->undo_action->next;
        assert(action->func);
        action->func(self, action->data);
        ChpegUndoAction_free(action);
    }
}
#endif

// replace first child with contents of first child
void ChpegNode_unpack_child(ChpegNode *self)
{
    ChpegNode *first_child = self->head; // first child
    // find last child of first child
    ChpegNode *last = first_child->head;
    for (;;) {
        last->parent = self;
        ++self->num_children;
        if (!last->next) break;
        last = last->next;
    }
    last->next = self->head->next; // link last item to second child
    self->head = first_child->head; // change first child (head) link
    ChpegNode_free_nr(first_child); // free the former first child
    --self->num_children;
}

// scans the child nodes to determine if we can simplify this node
int ChpegNode_can_simplify(ChpegNode *self)
{
    // STOP flag means no simplification (force a node capture)
    if (self->flags & CHPEG_FLAG_STOP) {
        return 0;
    }

    ChpegNode *p = NULL;

#if CHPEG_EXTENSION_TRIM
    size_t pos = self->match_offset + self->match_length; // start at end of this node
#else
    size_t pos = self->offset + self->length; // start at end of this node
#endif

    // scanning goes backwards since child order is reversed in tree as build while parsing

    int cnt = 0, text = 0;
    for (p = self->head; p;) {
#if CHPEG_DEBUG_CAN_SIMPLIFY
        fprintf(stderr, "node: <d:%d o:%zu l:%zu f:%d>, pos=%zu, cnt=%d, text=%d\n",
            p->def,
#if CHPEG_EXTENSION_TRIM
            p->match_offset, p->match_length,
#else
            p->offset, p->length,
#endif
            p->flags, pos, cnt, text);
#endif

#if CHPEG_EXTENSION_TRIM
        assert(p->match_offset + p->match_length <= pos);
        if (p->match_offset + p->match_length < pos)
#else
        assert(p->offset + p->length <= pos);
        if (p->offset + p->length < pos)
#endif
        {
            ++text;                             // a gap is considered a text node
#if CHPEG_DEBUG_CAN_SIMPLIFY
            fprintf(stderr, "can't simplify node: (early, text) <d:%d o:%zu l:%zu f:%d>, cnt=%d, text=%d\n",
                self->def,
#if CHPEG_EXTENSION_TRIM
                self->match_offset, self->match_length,
#else
                self->offset, self->length,
#endif
                self->flags,
                cnt, text);
#endif
            return 0;
        }

#if CHPEG_EXTENSION_TRIM
        pos = p->match_offset;
#else
        pos = p->offset;
#endif

        if (!(p->flags & CHPEG_FLAG_IGNORE)) {
            // a non-ignore node is candidate
            if (++cnt > 1) {
#if CHPEG_DEBUG_CAN_SIMPLIFY
                fprintf(stderr, "can't simplify node: (early, node) <d:%d o:%zu l:%zu f:%d>, cnt=%d, text=%d\n",
                    self->def,
#if CHPEG_EXTENSION_TRIM
                    self->match_offset, self->match_length,
#else
                    self->offset, self->length,
#endif
                    self->flags,
                    cnt, text);
#endif
                return 0;
            }
        }
        p = p->next;
    }

#if CHPEG_EXTENSION_TRIM
    assert(pos >= self->match_offset);
    if (pos > self->match_offset)
#else
    assert(pos >= self->offset);
    if (pos > self->offset)
#endif

    {
        ++text;
    }
    // if we have one candidate node and no text nodes, we can simplify
    if (cnt == 1 && !text) {
#if CHPEG_DEBUG_CAN_SIMPLIFY
        fprintf(stderr, "can simplify node: <d:%d o:%zu l:%zu f:%d>, cnt=%d, text=%d\n",
            self->def,
#if CHPEG_EXTENSION_TRIM
            self->match_offset, self->match_length,
#else
            self->offset, self->length,
#endif
            self->flags,
            cnt, text);
#endif
        return 1;
    }

#if CHPEG_DEBUG_CAN_SIMPLIFY
    fprintf(stderr, "can't simplify node: <d:%d o:%zu l:%zu f:%d>, cnt=%d, text=%d\n",
        self->def,
#if CHPEG_EXTENSION_TRIM
        self->match_offset, self->match_length,
#else
        self->offset, self->length,
#endif
        self->flags,
        cnt, text);
#endif

    return 0;
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

// reverse a tree
ChpegNode *ChpegNode_reverse(ChpegNode *self)
{
    ChpegNode *p = self->head; self->head = NULL;
    ChpegNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = ChpegNode_reverse(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

// reverse a tree, deleting IGNORE nodes in the process
ChpegNode *ChpegNode_reverse_clean(ChpegNode *self)
{
    ChpegNode *p = self->head; self->head = NULL;
    ChpegNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        if (p->flags & CHPEG_FLAG_IGNORE) {
            ChpegNode_free(p);
            --self->num_children;
        }
        else {
            p = ChpegNode_reverse_clean(p);
            p->next = self->head;
            self->head = p;
        }
    }
    return self;
}


#if CHPEG_PACKRAT

#if CHPEG_UNDO
typedef struct _ChpegPUndoNode
{
    struct _ChpegPUndoNode *head;
    struct _ChpegPUndoNode *next;
    ChpegUndoAction *undo_action;
} ChpegPUndoNode;
#endif

typedef struct _ChpegPNode
{
    struct _ChpegPNode *head;
    struct _ChpegPNode *next;
    ChpegNode *node;
#if CHPEG_UNDO
    ChpegPUndoNode *undo;
#endif
} ChpegPNode;

static void ChpegPUndoNode_free(ChpegPUndoNode *self)
{
    if (self) {
        ChpegUndoAction_free(self->undo_action);
        CHPEG_FREE(self);
    }
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

        ChpegPUndoNode *utmp;
        for (ChpegPUndoNode *pu = self->undo; pu; pu = utmp) {
            utmp = pu->next;
            ChpegPUndoNode_free(pu);
        }
        self->undo = NULL;

        ChpegNode_free_nr(self->node);
        CHPEG_FREE(self);
    }
}


static ChpegPNode *ChpegPNode_push_child(ChpegPNode *self, ChpegPNode *child)
{
    child->next = self->head;
    self->head = child;
    return child;
}

#if CHPEG_UNDO
static ChpegPUndoNode *ChpegPNode_push_undo(ChpegPNode *self, ChpegPUndoNode *undo)
{
    undo->next = self->undo;
    self->undo = undo;
    return undo;
}
#endif


static int chpeg_packrat_node_count(ChpegNode *node, int *count, int depth, int limit, ChpegParser *parser)
{
    // 'unwrap' nodes except the top one (skip useless intermediate nodes with only one child)
    if (depth > 0 && parser->simplification == 1) {
        while (node->num_children == 1 && !(node->flags & (CHPEG_FLAG_STOP | CHPEG_FLAG_LEAF))) {
            node = node->head;
        }
    }

    for (ChpegNode *p = node->head; p; p = p->next) {
        chpeg_packrat_node_count(p, count, depth + 1, limit, parser);
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
static ChpegPNode *ChpegPNode_from_node(ChpegPNode *pnode, ChpegNode *node, int depth, ChpegParser *parser)
{
    if (depth == 0) {
        int count = 0;
        if (chpeg_packrat_node_count(node, &count, 0, CHPEG_PACKRAT_NODE_LIMIT, parser) >=
            CHPEG_PACKRAT_NODE_LIMIT)
        {
            return NULL;
        }
    }
    // 'unwrap' nodes except the top one (skip useless intermediate nodes with only one child)
    // this can lead to quite a performance increase by cutting down on nodes created in _node_copy()
    if (depth > 0 && parser->simplification == 1) {
        while (node->num_children == 1 && !(node->flags & (CHPEG_FLAG_STOP | CHPEG_FLAG_LEAF))) {
            node = node->head;
        }
    }

    if (!pnode) {
        pnode = (ChpegPNode *)CHPEG_CALLOC(1, sizeof(ChpegPNode));
    }

    pnode->node = node;
    ++node->ref_count;
    for (ChpegNode *p = node->head; p; p = p->next) {
        ChpegPNode *pchild = (ChpegPNode *)CHPEG_CALLOC(1, sizeof(ChpegPNode));
        ChpegPNode_from_node(pchild, p, depth + 1, parser);
        ChpegPNode_push_child(pnode, pchild);
    }

#if CHPEG_UNDO
    for (ChpegUndoAction *action = node->undo_action; action; action = action->next) {
        ChpegPUndoNode *pu = (ChpegPUndoNode *)CHPEG_CALLOC(1, sizeof(ChpegPUndoNode));
        pu->undo_action = action;
        ++action->ref_count;
        ChpegPNode_push_undo(pnode, pu);
    }
#endif

    return pnode;
}

static ChpegNode *ChpegPNode_export(ChpegParser *parser, ChpegPNode *pnode)
{
    assert(pnode);
    ChpegNode *node = (ChpegNode *)CHPEG_MALLOC(sizeof(ChpegNode));
    memcpy(node, pnode->node, sizeof(ChpegNode));
    node->next = NULL;
    node->head = NULL;
    node->num_children = 0;
    node->ref_count = 1;
    for (ChpegPNode *p = pnode->head; p; p = p->next) {
        ChpegNode *child = ChpegPNode_export(parser, p) ;
        ChpegNode_push_child(node, child);
    }
#if CHPEG_UNDO
    for (ChpegPUndoNode *pu = pnode->undo; pu; pu = pu->next) {
        void *action_data = (void *)CHPEG_MALLOC(pu->undo_action->data_size);
        memcpy(action_data, pu->undo_action->data, pu->undo_action->data_size);
        ChpegNode_add_undo(node, pu->undo_action->func, action_data, pu->undo_action->data_size);
    }
#endif
#if CHPEG_EXTENSION_REFS
    node->refs = (ChpegReferenceInfo *)CHPEG_MALLOC(parser->bc->num_refs * sizeof(ChpegReferenceInfo));
    memcpy(node->refs, pnode->node->refs, parser->bc->num_refs * sizeof(ChpegReferenceInfo));
#endif
    return node;
}

#if CHPEG_VM_TRACE
static void ChpegPNode_print(ChpegPNode *self, ChpegParser *parser,
    const unsigned char *input, int depth, FILE *fp)
{
    if (!self) return;

    char flags[CHPEG_FLAGS_DISPLAY_SIZE];
    char *data = chpeg_esc_bytes(&input[self->node->offset], self->node->length, 40);
    const char *def_name = ChpegByteCode_def_name(parser->bc, self->node->def);

    if (depth == 0) {
        fprintf(fp, "PNODE BEG ---------------------------------- \n");
        fprintf(fp, " OFFSET   LEN     ID NC DP FLG IDENT \"DATA\"\n");
    }

    fprintf(fp, "%6zu %6zu %6d %2d %2d %3s %*s%s \"%s\"\n",
        self->node->offset,
        self->node->length,
        self->node->def,
        self->node->num_children,
        depth,
        chpeg_flags(flags, self->node->flags),
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

// packrat_no_match is a negative cache (attempted match, but match failed)
static ChpegPNode *packrat_no_match = (ChpegPNode *)-1;

typedef struct _ChpegPackrat
{
    ChpegParser *parser;
    int num_defs;
    size_t window_size;
    size_t window_end;
    int selective;
    ChpegPNode **pnodes;
} ChpegPackrat;

int ChpegPackrat_init(ChpegPackrat *self, ChpegParser *parser)
{
    self->parser = parser;
    self->num_defs = parser->bc->num_defs;
    self->window_size = parser->packrat_window_size ?
        parser->packrat_window_size : parser->length + 1;
    self->window_end = self->window_size - 1;

    self->pnodes = (ChpegPNode **)CHPEG_CALLOC(
        self->num_defs * self->window_size, sizeof(ChpegPNode **));

    for (int i = 0; i < self->num_defs; ++i) {
        if (self->parser->bc->def_flags[i] & CHPEG_FLAG_PACKRAT) {
            self->selective = 1;
            break;
        }
    }
    return 0;
}

int ChpegPackrat_cleanup(ChpegPackrat *self)
{
    int num_defs = self->parser->bc->num_defs;
    size_t loop_end = num_defs * self->window_size, z = 0;

    for (z = 0; z < loop_end; ++z) {
        ChpegPNode *pnode = self->pnodes[z];
        if (pnode && pnode != packrat_no_match) {
            assert(pnode->node->ref_count > 0);
            ChpegPNode_free(pnode);
        }
    }
    CHPEG_FREE(self->pnodes);
    return 0;
}

int ChpegPackrat_slide_window(ChpegPackrat *self, size_t offset)
{
    size_t z, loop_end;
    int num_defs = self->parser->bc->num_defs;

    if (offset > self->window_end) {
        if (offset - self->window_end >= self->window_size) {
#if CHPEG_VM_TRACE
            if (self->parser->vm_trace & 2) {
                fprintf(stderr, "packrat MOVE WINDOW END from %zu to %zu "
                    "(CLEAR ENTIRE WINDOW)\n", self->window_end, offset);
            }
#endif
            loop_end = self->window_size * num_defs;
            for (z = 0; z < loop_end; ++z) {
                ChpegPNode *pnode = self->pnodes[z];
                if (pnode && pnode != packrat_no_match) {
                    ChpegPNode_free(pnode);
                }
            }
            memset(self->pnodes, 0, self->window_size * num_defs * sizeof(ChpegPNode **));
        }
        else {
#if CHPEG_VM_TRACE
            if (self->parser->vm_trace & 2) {
                fprintf(stderr, "packrat MOVE WINDOW END from %zu to %zu "
                    "(CLEAR WINDOW from %zu to %zu)\n",
                    self->window_end, offset,
                    (self->window_end + 1) % self->window_size, offset % self->window_size);
            }
#endif
            loop_end = (offset + 1) * num_defs;
            for (z = (self->window_end + 1) * num_defs; z < loop_end; ++z) {
                size_t packrat_index = z % (self->window_size * num_defs);
                ChpegPNode *pnode = self->pnodes[packrat_index];
                if (pnode && pnode != packrat_no_match) {
                    ChpegPNode_free(pnode);
                }
                self->pnodes[packrat_index] = NULL;
            }
        }
        self->window_end = offset;
    }
    return 0;
}

ChpegPNode *ChpegPackrat_lookup(ChpegPackrat *self, size_t offset, int def_id)
{
    int num_defs = self->parser->bc->num_defs;
    size_t packrat_index = 0;
    ChpegPNode *packrat_lookup = NULL;

    if (offset >= self->window_end - self->window_size + 1 && offset <= self->window_end) {
        packrat_index = (offset % self->window_size) * (size_t)num_defs + (size_t)def_id;
        packrat_lookup = self->pnodes[packrat_index];
    }
    else {
        packrat_lookup = NULL;
    }

#if CHPEG_VM_TRACE
    if (self->parser->vm_trace & 2) {
        char *esc_str = NULL;
        if (packrat_lookup && packrat_lookup != packrat_no_match) {
            esc_str = chpeg_esc_bytes(self->parser->input + offset,
#if CHPEG_EXTENSION_TRIM
                packrat_lookup->node->match_length,
#else
                packrat_lookup->node->length,
#endif
                40);
        }
        fprintf(stderr, "packrat LOOKUP %7s %6zu %4d %20s %s%s%s\n",
            packrat_lookup == NULL ? "<NULL>" :
            (packrat_lookup == packrat_no_match ? "NEG" : "FOUND"),
            offset, def_id,
            ChpegByteCode_def_name(self->parser->bc, def_id),
            esc_str ? " \"" : "",
            esc_str ? esc_str : "",
            esc_str ? "\"" : ""
            );
        if (esc_str) { CHPEG_FREE(esc_str); esc_str = NULL; }
    }
#endif

    return packrat_lookup;
}

int ChpegPackrat_memoize(ChpegPackrat *self, size_t offset, int def_id, ChpegNode *node)
{
    int num_defs = self->parser->bc->num_defs;
    size_t packrat_index = 0;

    int cache_node = !self->selective ||
        (self->parser->bc->def_flags[def_id] & CHPEG_FLAG_PACKRAT);

#if CHPEG_VM_TRACE
    if (self->parser->vm_trace & 2) {
        char *esc_str = chpeg_esc_bytes(self->parser->input + offset,
#if CHPEG_EXTENSION_TRIM
            node->match_length,
#else
            node->length,
#endif
            40);

        fprintf(stderr, "packrat MEMOIZE %6s %6zu %4d %20s \"%s\"\n",
            cache_node ? "CACHE" : "--", offset, def_id,
            ChpegByteCode_def_name(self->parser->bc, def_id), esc_str);
        if (esc_str) { CHPEG_FREE(esc_str); esc_str = NULL; }
    }
#endif

    if (cache_node &&
        offset >= self->window_end - self->window_size + 1 && offset <= self->window_end)
    {
        packrat_index = (offset % self->window_size) * (size_t)num_defs + (size_t)def_id;
        self->pnodes[packrat_index] = ChpegPNode_from_node(
            NULL, node, 0, self->parser);

#if CHPEG_VM_TRACE
        if (self->parser->vm_trace & 4) {
            ChpegPNode_print(self->pnodes[packrat_index],
                self->parser, self->parser->input, 0, stderr);
        }
#endif
    }

    return 0;
}

int ChpegPackrat_memoize_neg(ChpegPackrat *self, size_t offset, int def_id)
{
    int num_defs = self->parser->bc->num_defs;
    size_t packrat_index = 0;

#if CHPEG_VM_TRACE
    if (self->parser->vm_trace & 2) {
        fprintf(stderr, "packrat MEMOIZE %6s %6zu %4d %20s\n",
            "NEG", offset, def_id,
            ChpegByteCode_def_name(self->parser->bc, def_id));
    }
#endif
    if (offset >= self->window_end - self->window_size + 1 && offset <= self->window_end) {
        packrat_index = ((int)offset % self->window_size) * num_defs + def_id;
        self->pnodes[packrat_index] = packrat_no_match;
    }
    return 0;
}


#endif // #if CHPEG_PACKRAT

#if CHPEG_EXTENSION_REFS

#if CHPEG_UNDO
typedef struct _ChpegUndoMarkData {
    int ref_id;
    ChpegReferenceInfo value;
} ChpegUndoMarkData;

void chpeg_undo_mark(ChpegNode *node, void *_data)
{
    ChpegUndoMarkData *data = (ChpegUndoMarkData *)_data;
#if 0
    fprintf(stderr, "chpeg_undo_mark: <o:%zu l:%zu f:%d>\n",
        data->value.offset, data->value.length, data->value.flags);
#endif
    for (ChpegNode *p = node; p; p = p->parent) {
        if (p->flags & CHPEG_FLAG_REFSCOPE) {
            p->refs[data->ref_id] = data->value;
            break;
        }
    }
}

#endif
#endif

//
// ChpegParser
//

ChpegParser *ChpegParser_new(const ChpegByteCode *bc)
{
    ChpegParser *self = (ChpegParser *)CHPEG_CALLOC(1, sizeof(ChpegParser));

    self->bc = bc;
    self->tree_root = NULL;
    self->simplification = 2;
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
    self->prof_ident_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_isucc_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_ifail_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_choice_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_cisucc_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->prof_cifail_cnt = (int *)CHPEG_MALLOC(bc->num_defs * sizeof(int));
    self->farthest_backtrack = 0;
#endif

#if CHPEG_PACKRAT
    self->packrat = 0;
    self->packrat_enabled = 0;
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
#if CHPEG_ERRORS
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

static inline void ChpegParser_expected(ChpegParser *self, size_t offset, int depth, int def, int pc)
{
    ChpegErrorInfo *error = NULL;
    if (offset >= self->error_offset) {
        if (offset > self->error_offset) {
            self->error_offset = offset;
            self->errors_size = 0;
        }
        if (self->errors_size + 1 > self->errors_capacity) {
            if (!self->errors) {
                self->errors_capacity = 1;
                self->errors = (ChpegErrorInfo *)CHPEG_MALLOC(
                    self->errors_capacity * sizeof(ChpegErrorInfo));
            }
            else {
                self->errors_capacity *= 2;
            }
            self->errors = (ChpegErrorInfo *)CHPEG_REALLOC(
                self->errors, self->errors_capacity * sizeof(ChpegErrorInfo));
        }
        error = &self->errors[self->errors_size++];
        error->depth = depth;
        error->def = def;
        error->pc = pc;
    }
}

// limit = 0: no limit (print all errors)
// limit = n: print last n errors
void ChpegParser_print_errors(ChpegParser *self, const unsigned char *input, int limit)
{
#if CHPEG_ERRORS
    if (self->errors_size == 0) {
        fprintf(stderr, "No errors tracked.\n");
        return;
    }
    assert(self->errors_size > 0);

    int start = 0;
    if (limit > 0) {
        start = self->errors_size - limit;
        if (start < 0) {
            start = 0;
        }
    }

    int min_depth = INT_MAX;
    for (int i = start; i < self->errors_size; ++i) {
        ChpegErrorInfo *error = &self->errors[i];
        if (error->depth < min_depth) {
            min_depth = error->depth;
        }
    }
    assert (min_depth < INT_MAX);

    for (int i = start; i < self->errors_size; ++i) {
        ChpegErrorInfo *error = &self->errors[i];
        const char *def_name = ChpegByteCode_def_name(self->bc, error->def);
        int inst = self->bc->instructions[error->pc];
        int op = inst & 0xff;
        int arg = inst >> 8;

        size_t line, col;
        char *esc = NULL;
        const char *str = NULL;
        int depth_indent = (error->depth - min_depth) *2;
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

        chpeg_line_col(input, self->error_offset, &line, &col);

        switch (op) {
            case CHPEG_OP_IDENT:
                str = ChpegByteCode_def_name(self->bc, arg);
                fprintf(stderr, "input:%zu:%zu: %11s %*s%s in %s\n",
                    line, col,
                    expected,
                    depth_indent, "",
                    str ? str : "<N/A>>",
                    def_name ? def_name : "<N/A>");
                break;
            case CHPEG_OP_LIT:
                esc = chpeg_esc_bytes((unsigned char *)self->bc->strings[arg],
                    self->bc->str_len[arg], 20);
                fprintf(stderr, "input:%zu:%zu: %11s %*s\"%s\" in %s\n",
                    line, col,
                    expected,
                    depth_indent, "",
                    esc ? esc : "<NULL>",
                    def_name ? def_name : "<N/A>");
                CHPEG_FREE(esc); esc = NULL;
                break;
            case CHPEG_OP_CHRCLS:
                esc = chpeg_esc_bytes((unsigned char *)self->bc->strings[arg],
                    self->bc->str_len[arg], 20);
                fprintf(stderr, "input:%zu:%zu: %11s %*s[%s] in %s\n",
                    line, col,
                    expected,
                    depth_indent, "",
                    esc ? esc : "<NULL>",
                    def_name ? def_name : "<N/A>");
                CHPEG_FREE(esc); esc = NULL;
                break;
            case CHPEG_OP_DOT:
                fprintf(stderr, "input:%zu:%zu: %11s %*scharacter in %s\n",
                    line, col,
                    expected,
                    depth_indent, "",
                    def_name ? def_name : "<N/A>");
                break;
            default: // unhandled op, show instruction in <> for debugging
                fprintf(stderr, "input:%zu:%zu: %11s %*sSyntax error in %s <%s,%d>\n",
                    line, col,
                    "Error:",
                    depth_indent, "",
                    def_name ? def_name : "<N/A>",
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
    ChpegParser_print_errors(self, input, 1);
}

static inline int chpeg_rule_requires_node(const int *def_flags, int def_id)
{
    return !!(def_flags[def_id] & CHPEG_FLAG_REFSCOPE);
}

// TODO: work on check sanity checks and underflow/overflow/range checks, make macros to make it easier
//
// CHPEG_SANITY_CHECKS should only contain checks for conditions that should never
// occur, and if they do occur, it's a bug/programming error. It shouldn't
// cover any cases that can be triggered by user input (grammar or input data)
// Same idea for any assert() in the code.
//
// CHPEG_SANITY_CHECKS and assert() are enabled by default, but this may change
// later. Stack overflow (either stack) can happen due to user input (grammar
// and/or input), so checks for that should never be removed (don't include
// within CHPEG_SANITY_CHECKS)
//
// Things that should be checked for in CHPEG_SANITY_CHECKS:
// - Stack underflow
// - Accessing anything out of range (stack, arrays)
//
// These are things that should not happen, and if they do it is a
// programming/bytecode error. We kind of have to trust the bytecode is
// correct. If there are errors in bytecode, then there are bugs in the
// compiler. If the user is creating their own bytecode, it's on them to
// make sure it's correct.
//
// TODO: cnt_max/RUNAWAY may not fit CHPEG_SANITY_CHECKS rules (pondering how to deal
// with things this is meant to detect)

#define CHPEG_CHECK_STACK_OVERFLOW(size) (top + (size) >= CHPEG_STACK_SIZE)
#define CHPEG_CHECK_STACK_UNDERFLOW(size) (top - (size) < 0)
#define CHPEG_CHECK_TSTACK_OVERFLOW(size) (tree_top + (size) >= CHPEG_TSTACK_SIZE)
#define CHPEG_CHECK_TSTACK_UNDERFLOW(size) (tree_top - (size) < 0)

int ChpegParser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed)
{
    self->input = input;
    self->length = length;

    int restricted = 0, cur_def = -1, retval = 0, i = 0;

#if CHPEG_ERRORS
    int err_locked = 0;
#endif

#if CHPEG_SANITY_CHECKS || CHPEG_VM_TRACE
    unsigned long long cnt = 0, cnt_max = 0;
#endif

#if CHPEG_SANITY_CHECKS
    const int num_instructions = self->bc->num_instructions;
#endif

#if CHPEG_VM_TRACE
    char *esc_str = NULL;
    const char *def_name = NULL;
#endif

#if CHPEG_VM_PRINT_TREE
    int tree_changed = 0;
#endif

#if CHPEG_EXTENSION_TRIM
    size_t token_offset = -1;
    size_t token_length = -1;
#endif

#if CHPEG_EXTENSION_REFS
    const int num_refs = self->bc->num_refs;
    assert(num_refs >= 0);
    ChpegReferenceInfo *ref = NULL;
#if CHPEG_UNDO
    ChpegUndoMarkData *undo_mark = NULL;
#endif
#endif

    //const int num_defs = self->bc->num_defs;
    //const char **def_names = (const char **)self->bc->def_names;
    const int *def_flags = self->bc->def_flags;
    const int *def_addrs = self->bc->def_addrs;
    const int *instructions = self->bc->instructions;
    //const int num_strings = self->bc->num_strings;
    const unsigned char **strings = (const unsigned char **)self->bc->strings;
    const int *str_len = self->bc->str_len;

#if CHPEG_PACKRAT
    ChpegPNode *packrat_lookup = NULL;
    size_t poffset = 0;
    if (self->packrat_enabled) {
        self->packrat = (ChpegPackrat *)CHPEG_CALLOC(1, sizeof(ChpegPackrat));
        ChpegPackrat_init(self->packrat, self);
    }
    else {
        self->packrat = NULL;
    }
#endif

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

    size_t offset = 0;
    int op = 0, arg = 0, pc = 0, top = 0, tree_top = 0;

    size_t stack[CHPEG_STACK_SIZE];
    ChpegNode *tree_stack[CHPEG_TSTACK_SIZE];

    if (CHPEG_CHECK_STACK_OVERFLOW(1)) {
        retval = CHPEG_ERR_STACK_OVERFLOW;
        goto done;
    }
    stack[top] = 0; // to make life easier, put a zero on the stack;
                    // top is then always in range [0, CHPEG_STACK_SIZE)
                    // top is incremented before push, so first real element is stack[1]

    if (CHPEG_CHECK_TSTACK_OVERFLOW(1)) {
        retval = CHPEG_ERR_TSTACK_OVERFLOW;
        goto done;
    }

    self->tree_root = ChpegNode_new(0, 0, 0, 0);

#if CHPEG_EXTENSION_REFS
    ChpegNode_alloc_refs(self->tree_root, num_refs);
#endif

    tree_stack[tree_top] = self->tree_root;

#if CHPEG_SANITY_CHECKS || CHPEG_VM_TRACE
    cnt_max = (length <= 2642245) ? (length < 128 ? 2097152 : length * length * length) : (unsigned long long)-1LL;
    for(cnt = 0; cnt < cnt_max; ++cnt)
#else
    for(;;)
#endif
    {

#if CHPEG_SANITY_CHECKS
        if (offset > length) { retval = CHPEG_ERR_INVALID_OFFSET; goto done; }
        if (pc < 0 || pc > num_instructions) { retval = CHPEG_ERR_INVALID_PC; goto done; }
        if (top >= CHPEG_STACK_SIZE) { retval = CHPEG_ERR_STACK_RANGE; goto done; }
        if (top < 0) { retval = CHPEG_ERR_STACK_RANGE; goto done; }
        if (tree_top >= CHPEG_TSTACK_SIZE) { retval = CHPEG_ERR_TSTACK_RANGE; goto done; }
        if (tree_top < 0) { retval = CHPEG_ERR_TSTACK_RANGE; goto done; }
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
#if CHPEG_EXTENSION_TRIM
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

                // stack content after IDENT call:
                // stack[top-5] = token_offset [ if TRIM ]
                // stack[top-4] = token_length [ if TRIM ]
                // stack[top-3] = bits
                // stack[top-2] = cur_def
                // stack[top-1] = offset
                // stack[top]   = pc

#if CHPEG_SANITY_CHECKS
                if (arg < 0) {
                    pc = -1; retval = CHPEG_ERR_INVALID_IDENTIFIER; break;
                }
#endif

#if CHPEG_VM_PROFILE
                ++self->prof_ident_cnt[arg];
#endif

#if CHPEG_PACKRAT
                if (self->packrat_enabled) {

                    ChpegPackrat_slide_window(self->packrat, offset);

                    packrat_lookup = ChpegPackrat_lookup(self->packrat, offset, arg);

                    if (packrat_lookup) {
                        if (packrat_lookup != packrat_no_match) {
                            if (!restricted || chpeg_rule_requires_node(def_flags, arg)) {
                                if (self->simplification == 2) {
                                    if (!(packrat_lookup->node->flags & CHPEG_FLAG_IGNORE) ||
#if CHPEG_EXTENSION_TRIM
                                        packrat_lookup->node->match_length
#else
                                        packrat_lookup->node->length
#endif
                                       )
                                    {
                                        ChpegNode_push_child(tree_stack[tree_top],
                                            ChpegPNode_export(self, packrat_lookup));

                                        // if we can simplify, unpack the node in-place
                                        if (ChpegNode_can_simplify(tree_stack[tree_top]->head)) {
                                            ChpegNode_unpack_child(tree_stack[tree_top]);
                                        }
                                    }
                                }
                                else {
                                    if ((packrat_lookup->node->flags & CHPEG_FLAG_IGNORE) == 0) {
                                        ChpegNode_push_child(tree_stack[tree_top],
                                            ChpegPNode_export(self, packrat_lookup));
                                    }
                                }

#if CHPEG_VM_PRINT_TREE
                                tree_changed = 1;
#endif
                            }
#if CHPEG_EXTENSION_TRIM
                            offset += packrat_lookup->node->match_length;
#else
                            offset += packrat_lookup->node->length;
#endif
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
                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_IDENT_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

#if CHPEG_EXTENSION_TRIM
                stack[++top] = token_offset; // push token_offset
                stack[++top] = token_length; // push token_length
#endif

                stack[++top] = 0; // push bits (1=child pushed; 2=restricted set)
                if (!restricted || chpeg_rule_requires_node(def_flags, arg)) {
                    if (CHPEG_CHECK_TSTACK_OVERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TSTACK_OVERFLOW; break;
                    }
                    stack[top] |= 1;
                    if (def_flags[arg] & (CHPEG_FLAG_LEAF | CHPEG_FLAG_IGNORE)) {
                        stack[top] |= 2; restricted = 1;
                    }
                    tree_stack[tree_top+1] = ChpegNode_push_child(tree_stack[tree_top],
                        ChpegNode_new(arg, offset, 0,
                            def_flags[arg] | ( arg == 0 ? CHPEG_FLAG_REFSCOPE : 0)
                            ));
#if CHPEG_EXTENSION_TRIM
                    // set working values to -1 (not trimmed)
                    token_offset = -1;
                    token_length = -1;
#endif
#if CHPEG_EXTENSION_REFS
                    ChpegNode_alloc_refs(tree_stack[tree_top+1], num_refs);
#endif
                    ++tree_top;
                }

                stack[++top] = cur_def; // push cur_def
                cur_def = arg;

                stack[++top] = offset; // push offset
                stack[++top] = pc; // push pc

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                pc = def_addrs[arg];
                break;

            case CHPEG_OP_ISUCC: // arg = def; Identifier "call" match success, "return"
                                 // pc restored to pc+2, skipping next instruction
                                 // see IDENT for stack layout
#if CHPEG_SANITY_CHECKS
                assert(cur_def == arg);
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_IDENT_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if CHPEG_VM_PROFILE
                ++self->prof_isucc_cnt[arg];
#endif
                pc = stack[top--] + 2; // pop pc

#if CHPEG_PACKRAT
                poffset = stack[top--]; // pop offset
#else
                --top; // skip offset
#endif

                cur_def = stack[top--]; // pop cur_def

                if (stack[top] & 2) { restricted = 0; } // stack[top] = bits
                if (stack[top] & 1) {

#if CHPEG_SANITY_CHECKS
                    if (CHPEG_CHECK_TSTACK_UNDERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TSTACK_UNDERFLOW; break;
                    }
#endif

#if CHPEG_EXTENSION_TRIM
                    // set match_length
                    tree_stack[tree_top]->match_length = offset -
                        tree_stack[tree_top]->match_offset;

                    // if trimmed left, set offset to token_offset
                    tree_stack[tree_top]->offset = (token_offset == (size_t)-1) ?
                        tree_stack[tree_top]->match_offset :
                        token_offset;

                    // if trimmed right, set length to token_length
                    tree_stack[tree_top]->length = (token_length == (size_t)-1) ?
                        tree_stack[tree_top]->match_length :
                        token_length;
#else
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
#endif
                    if ((tree_stack[tree_top]->flags & (CHPEG_FLAG_LEAF | CHPEG_FLAG_IGNORE)) &&
                        tree_stack[tree_top]->num_children > 0)
                    {
                        ChpegNode_clear(tree_stack[tree_top]);
                    }

#if CHPEG_PACKRAT
                    if (self->packrat_enabled) {
                        ChpegPackrat_memoize(self->packrat, poffset, arg, tree_stack[tree_top]);
                    }
#endif

                    if (self->simplification == 2) {
                        // drop zero-length ignore nodes now
                        if ((def_flags[arg] & CHPEG_FLAG_IGNORE) &&
                            // s2 is concerned about full match length
#if CHPEG_EXTENSION_TRIM
                            tree_stack[tree_top]->match_length
#else
                            tree_stack[tree_top]->length
#endif
                            == 0)
                        {
                            ChpegNode_pop_child(tree_stack[tree_top - 1]);
                        }
                        // else, if we can simplify, unpack the node in-place
                        else if (ChpegNode_can_simplify(tree_stack[tree_top])) {
                            ChpegNode_unpack_child(tree_stack[tree_top - 1]);
                        }
                    }
                    else {
                        if (def_flags[arg] & CHPEG_FLAG_IGNORE) {
                            ChpegNode_pop_child(tree_stack[tree_top - 1]);
                        }
                    }

                    --tree_top;
                }
                --top;

#if CHPEG_EXTENSION_TRIM
                // restore token length/offset
                token_length = stack[top--];
                token_offset = stack[top--];
#endif

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                // pc is restored +2 (next instruction skipped)
                break;

            case CHPEG_OP_IFAIL: // Identifier "call" match failure, "return"
                                 // pc restored +1 (next instruction not skipped)
                                 // see IDENT for stack layout
#if CHPEG_SANITY_CHECKS
                assert(cur_def == arg);
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_IDENT_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if CHPEG_VM_PROFILE
                ++self->prof_ifail_cnt[arg];
#endif
                pc = stack[top--]; // pop pc
                offset = stack[top--]; // pop offset
                cur_def = stack[top--]; // pop cur_def

#if CHPEG_PACKRAT
                if (self->packrat_enabled) {
                    ChpegPackrat_memoize_neg(self->packrat, offset, arg);
                }
#endif

#if CHPEG_ERRORS && CHPEG_ERRORS_IDENT
                if (!err_locked) { // Tracking errors here is probably bare minimum of usefulness
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_top > 0 ? tree_stack[tree_top-1]->def : -1, pc); // parent def, pc
#if CHPEG_DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif

                if (stack[top] & 2) { restricted = 0; } // stack[top] = bits
                if (stack[top] & 1) {
#if CHPEG_SANITY_CHECKS
                    if (CHPEG_CHECK_TSTACK_UNDERFLOW(1)) {
                        pc = -1; retval = CHPEG_ERR_TSTACK_UNDERFLOW; break;
                    }
#endif
                    CHPEG_NODE_FAIL_POP_CHILD(tree_stack[--tree_top]);
                }
                --top;

#if CHPEG_EXTENSION_TRIM
                // discard saved token offset/length
                top -= 2;
#endif

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                // pc has been restored, resume execution at next instruction
                ++pc;
                break;

//
// Choice
//
            case CHPEG_OP_CHOICE: // arg: unused

                // stack content after CHOICE op:
                // stack[top-2] = num_undo       [ if UNDO ]
                // stack[top-1] = num_children
                // stack[top]   = offset

#if CHPEG_VM_PROFILE
                ++self->prof_choice_cnt[cur_def];
#endif
                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_CHOICE_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                // save info needed to backtrack
#if CHPEG_UNDO
                stack[++top] = tree_stack[tree_top]->num_undo; // push num_undo
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // push num_children
                stack[++top] = offset; // push offset

                ++pc; // next instruction
                break;

            case CHPEG_OP_CISUCC: // Choice Item SUCCess arg = success/fail pc addr
#if CHPEG_VM_PROFILE
                ++self->prof_cisucc_cnt[cur_def];
#endif
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_CHOICE_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                top -= CHPEG_CHOICE_PUSHES;
                pc = arg;
                break;

            case CHPEG_OP_CIFAIL: // Choice Item FAIL; arg: unused
#if CHPEG_VM_PROFILE
                ++self->prof_cifail_cnt[cur_def];
#endif
                // backtrack
                offset = stack[top];

                // restore children to previous state
                for (i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i) {
                    CHPEG_NODE_FAIL_POP_CHILD(tree_stack[tree_top]);
                }
#if CHPEG_UNDO
                // undo until previous state
                for (i = tree_stack[tree_top]->num_undo - stack[top-2]; i > 0; --i) {
                    ChpegNode_pop_undo(tree_stack[tree_top]);
                }
#endif

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                ++pc; // next instruction
                break;

            case CHPEG_OP_CFAIL: // Choice FAIL; arg: pc
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_CHOICE_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                top -= CHPEG_CHOICE_PUSHES;
                pc = arg;
                break;

//
// Repeat +
//
            case CHPEG_OP_RPBEG: // Repeat Plus BEGin (+)

                // stack content after RPBEG op:
                // stack[top-3] = num_undo [ if UNDO ]
                // stack[top-2] = num_children
                // stack[top-1] = offset
                // stack[top]   = match count

                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_RP_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                // save info needed to backtrack
#if CHPEG_UNDO
                stack[++top] = tree_stack[tree_top]->num_undo; // push num_undo
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // push num_children
                stack[++top] = offset; // push offset

                stack[++top] = 0; // push cnt (match count)
                ++pc; // next instruction
                break;

            case CHPEG_OP_RPMAT: // Repeat Plus MATch; arg = loop pc addr
                ++stack[top]; // incr match count
                // only loop if consuming
                if (offset != stack[top-1]) {
                    // update backtrack info
#if CHPEG_UNDO
                    stack[top-3] = tree_stack[tree_top]->num_undo;
#endif
                    stack[top-2] = tree_stack[tree_top]->num_children;
                    stack[top-1] = offset;
                    pc = arg; // continue looping
                }
                else {
                    ++pc; // next instruction
                }
                break;

            case CHPEG_OP_RPDONE: // Repeat Plus DONE; arg = match fail pc addr
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_RP_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                // backtrack to point where match failed
                offset = stack[top-1];

                // restore children to previous state
                for (i = tree_stack[tree_top]->num_children - stack[top-2]; i > 0; --i)
                    CHPEG_NODE_FAIL_POP_CHILD(tree_stack[tree_top]);
#if CHPEG_UNDO
                // undo until previous state
                for (i = tree_stack[tree_top]->num_undo - stack[top-3]; i > 0; --i) {
                    ChpegNode_pop_undo(tree_stack[tree_top]);
                }
#endif

                if (stack[top] > 0) { // op+ SUCCESS
                    top -= CHPEG_RP_PUSHES;
                    ++pc; // next instruction
                }
                else { // op+ FAIL
                    top -= CHPEG_RP_PUSHES;
                    pc = arg;
                }

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

//
// Repeat *|?
//
            case CHPEG_OP_RSBEG: // Repeat Star BEGin (*); arg=unused
            case CHPEG_OP_RQBEG: // Repeat Question BEGin (?); arg=unused

                // stack content after R[SQ]BEG op:
                // stack[top-2] = num_undo [ if UNDO ]
                // stack[top-1] = num_children
                // stack[top]   = offset

                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_RSQ_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                // save info needed to backtrack
#if CHPEG_UNDO
                stack[++top] = tree_stack[tree_top]->num_undo; // num_undo
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children
                stack[++top] = offset; // offset
                ++pc; // next instruction
                break;

            case CHPEG_OP_RSMAT: // Repeat Star MATch; arg = loop pc addr
                // only loop if consuming
                if (offset != stack[top]) {
                    // update backtrack info
#if CHPEG_UNDO
                    stack[top-2] = tree_stack[tree_top]->num_undo;
#endif
                    stack[top-1] = tree_stack[tree_top]->num_children;
                    stack[top] = offset;
                    pc = arg; // continue looping
                }
                else {
                    ++pc; // next instruction
                }
                break;

            case CHPEG_OP_RSDONE: // Repeat Star DONE (*): always succeeds, proceeds to next instr.
            case CHPEG_OP_RQDONE: // Repeat Question DONE (?): always succeeds, proceeds to next instr.
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_RSQ_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                // backtrack to point where match failed
                offset = stack[top];

                // restore children to previous state
                for (i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    CHPEG_NODE_FAIL_POP_CHILD(tree_stack[tree_top]);
#if CHPEG_UNDO
                // undo until previous state
                for (i = tree_stack[tree_top]->num_undo - stack[top-2]; i > 0; --i) {
                    ChpegNode_pop_undo(tree_stack[tree_top]);
                }
#endif
                top -= CHPEG_RSQ_PUSHES;

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                ++pc; // next instruction
                break;

// Repeat ?
            case CHPEG_OP_RQMAT: // Repeat Question MATch (?); no looping for ?
                // update backtrack info
#if CHPEG_UNDO
                stack[top-2] = tree_stack[tree_top]->num_undo;
#endif
                stack[top-1] = tree_stack[tree_top]->num_children;
                stack[top] = offset;
                ++pc; // next instruction
                break;

#if CHPEG_EXTENSION_MINMAX
//
// Repeat {min, max}
//
            case CHPEG_OP_RMMBEG: // Repeat Min/Max BEGin
                                  // arg=min

                // stack content after RMMBEG op:
                // stack[top-5] = num_undo [ if UNDO ]
                // stack[top-4] = num_children
                // stack[top-3] = offset
                // stack[top-2] = loop address
                // stack[top-1] = min
                // stack[top]   = match count

                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_RMM_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                // save info needed to backtrack
#if CHPEG_UNDO
                stack[++top] = tree_stack[tree_top]->num_undo; // push num_undo
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // push num_children
                stack[++top] = offset; // push offset

                stack[++top] = pc+1; // push loop address
                stack[++top] = arg; // push min
                stack[++top] = 0; // push match count
                ++pc; // next instruction
                break;

            case CHPEG_OP_RMMMAT: // Repeat Min/Max MATch; arg = max
                ++stack[top]; // incr match count
                // loop if consuming and have not met max matches
                if (offset != stack[top-3] && stack[top] < (size_t)arg) {
                    // update backtrack info
#if CHPEG_UNDO
                    stack[top-5] = tree_stack[tree_top]->num_undo;
#endif
                    stack[top-4] = tree_stack[tree_top]->num_children;
                    stack[top-3] = offset;

                    pc = stack[top-2]; // continue looping
                }
                else {
                    ++pc; // next instruction (will be RMMDONE)
                }
                break;

            case CHPEG_OP_RMMDONE: // Repeat Min/Max DONE; arg = max
                                   // on success, skip next instruction
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_RMM_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif

                // min/max test
                if (stack[top] >= stack[top-1] && stack[top] <= (size_t)arg) {
                    // SUCCESS
                    top -= CHPEG_RMM_PUSHES;
                    pc += 2; // skip next instruction
                }
                else {
                    // FAIL
                    // backtrack to point where match failed
                    offset = stack[top-3];
                    // restore children to previous state
                    for (i = tree_stack[tree_top]->num_children - stack[top-4]; i > 0; --i)
                        CHPEG_NODE_FAIL_POP_CHILD(tree_stack[tree_top]);
#if CHPEG_UNDO
                    // undo until previous state
                    for (i = tree_stack[tree_top]->num_undo - stack[top-5]; i > 0; --i) {
                        ChpegNode_pop_undo(tree_stack[tree_top]);
                    }
#endif
                    top -= CHPEG_RMM_PUSHES;
                    ++pc; // next instruction
                }

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

#endif // if CHPEG_EXTENSION_MINMAX

#if CHPEG_EXTENSION_TRIM
//
// Trim
//
            case CHPEG_OP_TRIM: // TRIM start ('<')
                                // arg: unused

                // stack content after TRIM op:
                // stack[top-1] = original token_offset
                // stack[top]   = changed token_offset

                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_TRIM_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                // Save original offset
                stack[++top] = token_offset;

                // Trim on left
                token_offset = offset;

                // Save changed offset
                stack[++top] = token_offset;

                ++pc; // next instruction
                break;

            case CHPEG_OP_TRIMS: // TRIM Success ('>'): the contents inside ('<' ... '>') matched
                                 // arg = next pc

#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_TRIM_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                // Restore the changed offset from the stack
                token_offset = stack[top--];

                // Trim on right
                token_length = offset - token_offset;

                // Discard the original offset saved on stack
                --top;

                pc = arg;
                break;

            case CHPEG_OP_TRIMF: // TRIM Failed ('>'): the contents inside ('<' ... '>') did not match
                                 // arg = next pc

#if SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_TRIM_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif

                // Discard the changed offset saved on stack
                top--;

                // Restore original offset
                token_offset = stack[top--];

                pc = arg;
                break;

#endif // #if CHPEG_EXTENSION_TRIM

#if CHPEG_EXTENSION_REFS
//
// References
//
            case CHPEG_OP_MARK: // MARK start; arg: ref_id

                // stack content after MARK op:
                // stack[top-1] = beginning offset
                // stack[top]   = ref_id

                assert(num_refs);
                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_MARK_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                stack[++top] = offset; // push beginning offset
                stack[++top] = arg;    // push ref_id

#if CHPEG_VM_TRACE
                if (self->vm_trace & 8) {
                    fprintf(stderr, "+    MARK: START  ref %d (%12s) offset=%zu\n",
                        arg, ChpegByteCode_ref_name(self->bc, arg), offset);
                }
#endif

                ++pc; // next instruction
                break;

            case CHPEG_OP_MARKS: // MARK Success: the contents inside ($name'<' ... '>') matched
                                 // arg = next pc

#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_MARK_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                assert(num_refs && (int)stack[top] < num_refs);

                // find the ref in nearest reference scope
                ref = NULL;
                for (i = tree_top; i >= 0; --i) {
                    if (tree_stack[i]->flags & CHPEG_FLAG_REFSCOPE) {
                        ref = &tree_stack[i]->refs[stack[top]]; // stack[top] contains ref_id
                        break;
                    }
                }
                assert(ref);

#if CHPEG_UNDO
                undo_mark = (ChpegUndoMarkData *)CHPEG_MALLOC(sizeof(ChpegUndoMarkData));
                undo_mark->ref_id = stack[top];
                undo_mark->value = *ref;
                ChpegNode_add_undo(tree_stack[tree_top],
                    chpeg_undo_mark, undo_mark, sizeof(ChpegUndoMarkData));
                undo_mark = NULL;
#endif

                ref->offset = stack[top-1];
                ref->length = offset - ref->offset;
                ref->flags = 1;

#if CHPEG_VM_TRACE
                if (self->vm_trace & 8) {
                    char *esc = chpeg_esc_bytes(input + ref->offset, ref->length, 40);
                    fprintf(stderr, "+   MARKS: SET    ref %d (%12s) "
                        "tlevel=%d <offset:%zu length:%zu data:\"%s\">\n",
                        (int)stack[top], ChpegByteCode_ref_name(self->bc, stack[top]), i,
                        ref->offset, ref->length, esc);
                    CHPEG_FREE(esc);
                }
#endif

                top -= 2;
                pc = arg;
                break;

            case CHPEG_OP_MARKF: // MARK Failed: the contents inside ($name'<' ... '>') did not match
                                 // arg = next pc

#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_MARK_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                assert(num_refs && (int)stack[top] < num_refs);

                // find the ref in nearest reference scope
                ref = NULL;
                for (i = tree_top; i >= 0; --i) {
                    if (tree_stack[i]->flags & CHPEG_FLAG_REFSCOPE) {
                        ref = &tree_stack[i]->refs[stack[top]]; // stack[top] contains ref_id
                        break;
                    }
                }
                assert(ref);

#if CHPEG_UNDO
                undo_mark = (ChpegUndoMarkData *)CHPEG_MALLOC(sizeof(ChpegUndoMarkData));
                undo_mark->ref_id = stack[top];
                undo_mark->value = *ref;
                ChpegNode_add_undo(tree_stack[tree_top],
                    chpeg_undo_mark, undo_mark, sizeof(ChpegUndoMarkData));
                undo_mark = NULL;
#endif

                // unset the reference (flags=0)
                ref->offset = 0;
                ref->length = 0;
                ref->flags = 0;

#if CHPEG_VM_TRACE
                if (self->vm_trace & 8) {
                    fprintf(stderr, "+   MARKF: UNSET  ref %d (%12s) tlevel=%d <flags:%d>\n",
                        (int)stack[top], ChpegByteCode_ref_name(self->bc, stack[top]), i,
                        ref->flags);
                }
#endif

                top -= 2;
                pc = arg;
                break;

            case CHPEG_OP_REF: // match REFerence; arg = ref_id;
                               // skip next instruction on match

                assert(num_refs && arg < num_refs);

#if CHPEG_VM_TRACE
                if (self->vm_trace & 8) {
                    fprintf(stderr, "+     REF: LOOKUP ref %d (%12s) offset=%zu\n",
                        arg, ChpegByteCode_ref_name(self->bc, arg), offset);
                }
#endif
                for (i = tree_top; i >= 0; --i) {
                    ref = &tree_stack[i]->refs[arg];
#if CHPEG_VM_TRACE
                    if (self->vm_trace & 8) {
                        fprintf(stderr, "+     REF: SEARCH ref %d (%12s) tlevel=%d <f:%d o:%zu l:%zu>\n",
                            arg, ChpegByteCode_ref_name(self->bc, arg), i,
                            ref->flags, ref->offset, ref->length);
                    }
#endif
                    if (ref->flags == 1) {
                        if ((offset < (length - (ref->length - 1))) &&
                            memcmp(input + offset, input + ref->offset, ref->length) == 0)
                        {
#if CHPEG_VM_TRACE
                            if (self->vm_trace & 8) {
                                char *esc = chpeg_esc_bytes(input + ref->offset, ref->length, 40);
                                fprintf(stderr, "+     REF: MATCH  ref %d (%12s) tlevel=%d "
                                    "<f:%d o:%zu l:%zu> data:\"%s\"\n",
                                    arg, ChpegByteCode_ref_name(self->bc, arg), i,
                                    ref->flags, ref->offset, ref->length, esc);
                                CHPEG_FREE(esc);
                            }
#endif
                            offset += ref->length;
                            pc += 2;
                            goto op_ref_done; // need to break 2 levels
                        }
                        else {
                            break;
                        }
                    }
                }
#if CHPEG_VM_TRACE
                if (self->vm_trace & 8) {
                    char *esc = chpeg_esc_bytes(input + ref->offset, ref->length, 40);
                    fprintf(stderr, "+     REF: NOMAT  ref %d (%12s) offset=%zu\n",
                        arg, ChpegByteCode_ref_name(self->bc, arg), offset);
                    CHPEG_FREE(esc);
                }
#endif
#if CHPEG_ERRORS && CHPEG_ERRORS_REFS
                if (!err_locked) {
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_stack[tree_top]->def, pc); // def, pc
#if CHPEG_DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif
                ++pc; // failed match, go to next instruction
op_ref_done:
                break;
#endif // #if CHPEG_EXTENSION_REFS

//
// Predicate
//
            case CHPEG_OP_PREDA:
            case CHPEG_OP_PREDN:
                // Predicate begin, should be followed with child instructions,
                // then PMATCH{S,F}, then PNOMAT{S,F}, depending on op (&,!)

                // stack content after PREDA/PREDN op
                // stack[top-3] = num_children
                // stack[top-2] = offset
                // stack[top-1] = pc (for error) [ if ERRORS && ERRORS_PRED ]
                // stack[top]   = set err_locked [ if ERRORS ]

                if (CHPEG_CHECK_STACK_OVERFLOW(CHPEG_PRED_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }

                stack[++top] = tree_stack[tree_top]->num_children; // push num_children
                stack[++top] = offset; // push offset

#if CHPEG_ERRORS
#if CHPEG_ERRORS_PRED
                stack[++top] = pc; // push pc (for error)
#endif
                // push 'set err_locked' (meaning this op set it)
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
#if CHPEG_ERRORS && CHPEG_ERRORS_PRED
                if (stack[top]) {
                    ChpegParser_expected(self, stack[top-2], tree_top, // offset, depth
                        tree_stack[tree_top]->def, stack[top-1]); // def, pc
#if CHPEG_DEBUG_ERRORS
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
#if CHPEG_SANITY_CHECKS
                if (CHPEG_CHECK_STACK_UNDERFLOW(CHPEG_PRED_PUSHES)) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if CHPEG_ERRORS
                if (stack[top--]) { // pop 'set err_locked'
                    err_locked = 0;
                }
#if CHPEG_ERRORS_PRED
                --top;  // pop (discard) error pc
#endif
#endif
                // backtrack
                offset = stack[top--]; // restore saved offset (don't consume) (pop offset)
                for (i = tree_stack[tree_top]->num_children - stack[top--]; i > 0; --i) // pop num_children
                    CHPEG_NODE_FAIL_POP_CHILD(tree_stack[tree_top]);
#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// CharClass
            case CHPEG_OP_CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < length) {
                        int mlen = str_len[arg];
                        const unsigned char *mstr = strings[arg];
                        for (i = 0; i < mlen; ++i) {
                            if ((mstr[i+1] == '-') && (i < mlen - 2)) {
                                if ((input[offset] >= mstr[i]) && (input[offset] <= mstr[i+2])) {
                                    ++offset;
                                    pc += 2;
                                    goto chrcls_done; // need to break two levels
                                }
                                i+=2;
                            }
                            else if (mstr[i] == input[offset]) {
                                ++offset;
                                pc += 2;
                                goto chrcls_done; // need to break two levels
                            }
                        }
                    }
#if CHPEG_ERRORS && CHPEG_ERRORS_TERMINALS
                    if (!err_locked) {
                        ChpegParser_expected(self, offset, tree_top, // offset, depth
                            tree_stack[tree_top]->def, pc); // def, pc
#if CHPEG_DEBUG_ERRORS
                        ChpegParser_print_error(self, input);
#endif
                    }
#endif
                    ++pc; // next instruction (upon failure)
                }
chrcls_done:
                break;

#if CHPEG_EXTENSION_NCHRCLS
            case CHPEG_OP_NCHRCLS: // Negated CharClass: matching a character in the chrcls is failure
                                   // arg = chrcls str_id; skip next instruction on success
                {
                    if (offset < length) {
                        int mlen = str_len[arg];
                        const unsigned char *mstr = strings[arg];
                        for (i = 0; i < mlen; ++i) {
                            if ((mstr[i+1] == '-') && (i < mlen - 2)) {
                                if ((input[offset] >= mstr[i]) && (input[offset] <= mstr[i+2])) {
                                    ++pc; // next instruction (failure)
                                    goto nchrcls_done; // need to break two levels
                                }
                                i+=2;
                            }
                            else if (mstr[i] == input[offset]) {
                                ++pc; // next instruction (failure)
                                goto nchrcls_done; // need to break two levels
                            }
                        }
                        // success (no matches)
                        ++offset;
                        pc += 2;
                        break;
                    }
#if CHPEG_ERRORS && CHPEG_ERRORS_TERMINALS
                    if (!err_locked) {
                        ChpegParser_expected(self, offset, tree_top, // offset, depth
                            tree_stack[tree_top]->def, pc); // def, pc
#if CHPEG_DEBUG_ERRORS
                        ChpegParser_print_error(self, input);
#endif
                    }
#endif
                    ++pc; // next instruction (failure)
                }
nchrcls_done:
                break;
#endif

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
#if CHPEG_ERRORS && CHPEG_ERRORS_TERMINALS
                if (!err_locked) {
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_stack[tree_top]->def, pc); // def, pc
#if CHPEG_DEBUG_ERRORS
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
#if CHPEG_ERRORS && CHPEG_ERRORS_TERMINALS
                if (!err_locked) {
                    ChpegParser_expected(self, offset, tree_top, // offset, depth
                        tree_stack[tree_top]->def, pc); // def, pc
#if CHPEG_DEBUG_ERRORS
                    ChpegParser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                break;

// End
            case CHPEG_OP_SUCC: // overall success
                pc = -1; // we're done


#if CHPEG_EXTENSION_TRIM
                // set match_length
                tree_stack[tree_top]->match_length = offset -
                    tree_stack[tree_top]->match_offset;

                // if trimmed left, set offset to token_offset
                tree_stack[tree_top]->offset = (token_offset == (size_t)-1) ?
                    tree_stack[tree_top]->match_offset :
                    token_offset;

                // if trimmed right, set length to token_length
                tree_stack[tree_top]->length = (token_length == (size_t)-1) ?
                    tree_stack[tree_top]->match_length :
                    token_length;
#else
                tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
#endif

                //
                // reverse, simplify, and cleanup based on algo
                // self->tree_root should have either 0 or 1 child after cleanup:
                // 0 children if root node is ignored, 1 otherwise.
                //

                switch (self->simplification) {
                    case 1:
                        assert(self->tree_root->num_children >= 0 && self->tree_root->num_children <= 1);
                        if (self->tree_root->num_children == 0) {
                            self->tree_root = NULL;
                            ChpegNode_free_nr(tree_stack[0]);
                        }
                        else if (self->tree_root->num_children == 1) {
                            self->tree_root = ChpegNode_unwrap(self->tree_root);
                        }
                        break;
                    case 2:
                        self->tree_root = ChpegNode_reverse_clean(self->tree_root);
                        assert(self->tree_root->num_children >= 0 && self->tree_root->num_children <= 1);
                        if (self->tree_root->num_children == 0) {
                            self->tree_root = NULL;
                            ChpegNode_free_nr(tree_stack[0]);
                        }
                        else if (self->tree_root->num_children == 1) {
                            self->tree_root = self->tree_root->head;
                            ChpegNode_free_nr(tree_stack[0]);
                        }
                        break;
                    default:
                        assert(self->tree_root->num_children >= 0 && self->tree_root->num_children <= 1);
                        if (self->tree_root->num_children == 0) {
                            self->tree_root = NULL;
                            ChpegNode_free_nr(tree_stack[0]);
                        }
                        else if (self->tree_root->num_children == 1) {
                            self->tree_root = ChpegNode_reverse(self->tree_root->head);
                            ChpegNode_free_nr(tree_stack[0]);
                        }
                        break;
                }

#if CHPEG_VM_PRINT_TREE
                tree_changed = 1;
#endif

                if (top != 0) {
                    retval = CHPEG_ERR_STACK_DATA;
                }
                else if (tree_top != 0) {
                    retval = CHPEG_ERR_TSTACK_DATA;
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
    if (self->packrat_enabled) {
        ChpegPackrat_cleanup(self->packrat);
        CHPEG_FREE(self->packrat);
        self->packrat = NULL;
    }
#endif

    self->parse_result = retval;
    return retval;
}

#undef CHPEG_CHECK_STACK_OVERFLOW
#undef CHPEG_CHECK_STACK_UNDERFLOW
#undef CHPEG_CHECK_TSTACK_OVERFLOW
#undef CHPEG_CHECK_TSTACK_UNDERFLOW

#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: parser.c

