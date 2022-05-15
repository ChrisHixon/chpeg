#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mem.h"
#include "parser.h"
#include "compiler.h"

#include "opcodes.h"
#include "chpeg_bytecode.h"

#ifndef DEBUG_COMPILER
#define DEBUG_COMPILER 0
#endif

//
// Gnode: grammar tree node (temporary use during compilation)
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

static ChpegGNode *GNode_new()
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

static void GNode_free(ChpegGNode *self)
{
    ChpegGNode *tmp;
    for (ChpegGNode *p = self->head; p; p = tmp) {
        tmp = p->next;
        GNode_free(p);
    }
    self->head = NULL;
    CHPEG_FREE(self);
}

static ChpegGNode *GNode_push_child(ChpegGNode *self, ChpegGNode *child)
{
    child->next = self->head;
    self->head = child;
    ++(self->num_children);
    return child;
}

// unused
#if 0
static void GNode_pop_child(ChpegGNode *self)
{
    if (self->head) {
        ChpegGNode *tmp = self->head;
        self->head = self->head->next;
        GNode_free(tmp);
        --(self->num_children);
    }
}
#endif

static ChpegGNode *GNode_reverse(ChpegGNode *self)
{
    ChpegGNode *p = self->head; self->head = NULL;
    ChpegGNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = GNode_reverse(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

//
// Compiler
//

typedef struct _CompilationUnit
{
    ChpegParser *parser;
    const unsigned char *input;
    ChpegByteCode *bc;
    ChpegGNode *root;
    int strings_allocated;
} CompilationUnit;

#if DEBUG_COMPILER
static void CompilationUnit_print(CompilationUnit *cu, ChpegGNode *gnode, const unsigned char *input, int depth)
{
    int flags = 0;
    char *data = NULL;
    const char *def_name = ChpegParser_def_name(cu->parser, gnode->type);

    ChpegNode *node = gnode->node;
    if (node) {
        data = esc_bytes(&input[node->offset], node->length, 40);
        flags = node->flags;
    }

    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
        printf(" Begin    Len  DefID    Parse  PNext  PFail  Flags  Def. Name / Data\n");
        printf("---------------------------------------------------------------------------------\n");
    }
    printf("%6d %6d %6d | %6d %6d %6d | %s%s%s | %*s%s \"%s\"\n",
        node ? node->offset : -1,
        node ? node->length : -1,
        gnode->type,
        gnode->parse_state,
        gnode->parent_next_state,
        gnode->parent_fail_state,
        flags & STOP ? "S" : " ",
        flags & IGNORE ? "I" : " ",
        flags & LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : ""
        );
    if (data) { CHPEG_FREE(data); data = NULL; }

    for (ChpegGNode *p = gnode->head; p; p = p->next) {
        CompilationUnit_print(cu, p, input, depth + 1);
    }
}
#endif

static void Compiler_setup_defs(CompilationUnit *cu)
{
    ChpegNode *p = NULL;
    int i = 0, j = 0;

    cu->bc->num_defs = cu->parser->tree_root->num_children;
#if DEBUG_COMPILER
    printf("Compiler_setup_defs: num_defs=%d\n", cu->bc->num_defs);
#endif

    cu->bc->def_names = (char **)CHPEG_MALLOC(cu->bc->num_defs * sizeof(char *));
    cu->bc->def_flags = (int *)CHPEG_MALLOC(cu->bc->num_defs * sizeof(int));
    cu->bc->def_addrs = (int *)CHPEG_MALLOC(cu->bc->num_defs * sizeof(int));

    for (p = cu->parser->tree_root->head, i = 0; p; p = p->next, ++i) {
        if (CHPEG_DEFINITION != p->def) { continue; }

        ChpegNode *tmp = p->head; // Identifier, definition name
        if (NULL == tmp || CHPEG_IDENTIFIER != tmp->def) { continue; }
        cu->bc->def_names[i] = (char *)CHPEG_MALLOC(1 + tmp->length);
        memcpy(cu->bc->def_names[i], &cu->input[tmp->offset], tmp->length);
        cu->bc->def_names[i][tmp->length] = '\0';

        int flags = 0;
        tmp = tmp->next; // Options, flags
        if (NULL != tmp && CHPEG_OPTIONS == tmp->def) {
            for (j = 0; j < tmp->length; ++j) {
                switch(cu->input[tmp->offset + j]) {
                    case 'S': flags |= CHPEG_STOP; break;
                    case 'I': flags |= CHPEG_IGNORE; break;
                    case 'L': flags |= CHPEG_LEAF; break;
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

static void Compiler_setup_def_addrs(CompilationUnit *cu)
{
    int i = 0;
    ChpegGNode *p = NULL;
    for (i = 0, p = cu->root->head; p; p = p->next, ++i) {
        cu->bc->def_addrs[i] = p->head->next->parse_state - 1;
    }
}

static int Compiler_find_def(CompilationUnit *cu, ChpegNode *ident)
{
    char buf[ident->length + 1];
    memcpy(buf, cu->input + ident->offset, ident->length);
    buf[ident->length] = '\0';
    for (int i = 0; i < cu->bc->num_defs; ++i) {
        if (0 == strcmp(buf, cu->bc->def_names[i])) {
            return i;
        }
    }
    return -1;
}

static void Compiler_build_tree(CompilationUnit *cu, ChpegNode *np, ChpegGNode *gp)
{
    if (NULL == np) { np = cu->parser->tree_root; }
    if (NULL == gp) { gp = cu->root; }
    gp->node = np;
    gp->type = np->def;
    for (ChpegNode *p = np->head; p; p = p->next) {
        ChpegGNode *g = GNode_new();
        Compiler_build_tree(cu, p, g);
        GNode_push_child(gp, g);
    }
}

static inline int Compiler_alloc_inst(CompilationUnit *cu)
{
    return cu->bc->num_instructions++;
}

static void Compiler_alloc_instructions(CompilationUnit *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_GRAMMAR:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
            }
            break;
        case CHPEG_DEFINITION:
            Compiler_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->next->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case CHPEG_CHOICE:
            gp->parse_state = Compiler_alloc_inst(cu);
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
                p->parent_next_state = Compiler_alloc_inst(cu);
                p->parent_fail_state = Compiler_alloc_inst(cu);
            }
            Compiler_alloc_inst(cu);
            break;
        case CHPEG_SEQUENCE:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
            }
            gp->parse_state = gp->head->parse_state;
            break;
        case CHPEG_REPEAT:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_instructions(cu, gp->head);
            gp->head->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case CHPEG_PREDICATE:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->next->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case CHPEG_DOT:
            gp->parse_state = Compiler_alloc_inst(cu);
            break;
        case CHPEG_IDENTIFIER:
        case CHPEG_CHARCLASS:
        case CHPEG_LITERAL:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            break;
    }
}

static inline void Compiler_add_inst(CompilationUnit *cu, int inst)
{
    cu->bc->instructions[cu->bc->num_instructions++] = inst;
}

static void Compiler_add_instructions(CompilationUnit *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_GRAMMAR:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, 0));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_FAIL, 0));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_SUCC, 0));
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_add_instructions(cu, p);
            }
            break;
        case CHPEG_DEFINITION:
            Compiler_add_instructions(cu, gp->head->next);
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_ISUCC, Compiler_find_def(cu, gp->head->node)));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_IFAIL, 0));
            break;
        case CHPEG_CHOICE:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_CHOICE, 0));
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_add_instructions(cu, p);
                Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_CISUCC, gp->parent_next_state - 1));
                Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_CIFAIL, 0));
            }
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_CFAIL, gp->parent_fail_state - 1));
            break;
        case CHPEG_SEQUENCE:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : gp->parent_next_state;
                p->parent_fail_state = gp->parent_fail_state;
                Compiler_add_instructions(cu, p);
            }
            break;
        case CHPEG_REPEAT:
            {
                unsigned char op = cu->input[gp->head->next->node->offset];
                switch (op) {
                    case '+':
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RPBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RPMAT, gp->head->parse_state - 1));
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RPDONE, gp->parent_fail_state - 1));
                        break;
                    case '*':
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RSBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RSMAT, gp->head->parse_state - 1));
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RSDONE, 0));
                        break;
                    case '?':
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RQBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RQMAT, 0));
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_RQDONE, 0));
                        break;
                }
            }
            break;
        case CHPEG_PREDICATE:
            {
                unsigned char op = cu->input[gp->head->node->offset];
                switch (op) {
                    case '&':
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PRED, 0));
                        Compiler_add_instructions(cu, gp->head->next);
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHS, gp->parent_fail_state - 1));
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATF, gp->parent_fail_state - 1));
                        break;
                    case '!':
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PRED, 0));
                        Compiler_add_instructions(cu, gp->head->next);
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PMATCHF, gp->parent_fail_state - 1));
                        Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_PNOMATS, gp->parent_fail_state - 1));
                        break;
                }
            }
            break;
        case CHPEG_DOT:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_DOT, gp->parent_fail_state - 1));
            break;
        case CHPEG_IDENTIFIER:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_IDENT, Compiler_find_def(cu, gp->node)));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
        case CHPEG_CHARCLASS:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_CHRCLS, gp->val.ival));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
        case CHPEG_LITERAL:
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_LIT, gp->val.ival));
            Compiler_add_inst(cu, CHPEG_INST(CHPEG_OP_GOTO, gp->parent_fail_state - 1));
            break;
    }
}

static int Compiler_alloc_string(CompilationUnit *cu, const unsigned char *str, int len)
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

static void Compiler_alloc_strings(CompilationUnit *cu, ChpegGNode *gp)
{
    switch (gp->type) {
        case CHPEG_LITERAL:
        case CHPEG_CHARCLASS:
            {
                int len = 0, offset = 0;
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    Compiler_alloc_strings(cu, p);
                    len += p->value_len;
                }
                unsigned char *str = (unsigned char *)CHPEG_MALLOC(len);
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    memcpy(str+offset, p->val.cval, p->value_len);
                    offset += p->value_len;
                }
                gp->val.ival = Compiler_alloc_string(cu, str, len);
#if DEBUG_COMPILER
                char *tmp = esc_bytes(str, len, 20);
                printf("PEG LITERAL/CHARCLASS %s %d\n", tmp, gp->val.ival);
                CHPEG_FREE(tmp);
#endif
                CHPEG_FREE(str);
            }
            break;
        case CHPEG_CHARRANGE:
            {
                for (ChpegGNode *p = gp->head; p; p = p->next) {
                    Compiler_alloc_strings(cu, p);
                }
                gp->val.cval[0] = gp->head->val.cval[0];
                gp->val.cval[1] = '-';
                gp->val.cval[2] = gp->head->next->val.cval[0];
                gp->value_len = 3;
#if DEBUG_COMPILER
                char *tmp = esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_CHARRANGE %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_PLAINCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset];
                gp->value_len = 1;
#if DEBUG_COMPILER
                char *tmp = esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_PLAINCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_ESCCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset + 1];
                gp->value_len = 1;
                switch (gp->val.cval[0]) {
                    case 'n': gp->val.cval[0] = '\n'; break;
                    case 'r': gp->val.cval[0] = '\r'; break;
                    case 't': gp->val.cval[0] = '\t'; break;
                }
#if DEBUG_COMPILER
                char *tmp = esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_ESCCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        case CHPEG_OCTCHAR:
            {
                int val = 0; int len = gp->node->length - 1;
                const unsigned char *ip = cu->input + gp->node->offset + 1;
                for (int i = 0; i < len; ++i) {
                    val = (val << 3) | (ip[i] - '0');
                }
                gp->val.cval[0] = val & 255;
                gp->value_len = 1;
#if DEBUG_COMPILER
                char *tmp = esc_bytes(gp->val.cval, gp->value_len, 10);
                printf("CHPEG_OCTCHAR %s\n", tmp);
                CHPEG_FREE(tmp);
#endif
            }
            break;
        default:
            for (ChpegGNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_strings(cu, p);
            }
            break;
    }
}

ChpegByteCode *Compiler_compile(const unsigned char *input, size_t length, int *result_return, int verbose)
{
    CompilationUnit cu;

    cu.parser = ChpegParser_new(Compiler_bytecode());
    cu.input = input;
    cu.bc = NULL;
    cu.root = NULL;
    cu.strings_allocated = 0;

    size_t consumed = 0;
    int result = ChpegParser_parse(cu.parser, input, length, &consumed);
    if (result_return) {
        *result_return = result;
    }

    if (verbose) {
        if (result == 0) {
            printf("Parse successful.\n");
            if (verbose & 0x2) {
                ChpegParser_print_tree(cu.parser, input);
            }
        }
        else {
            if (result == CHPEG_ERR_EXTRANEOUS_INPUT) {
                printf("Extraneous input: parse consumed %lu bytes out of %lu\n", consumed, length);
            }
            else {
                printf("Parse failed with result: %d\n", result);
            }
            ChpegParser_print_error(cu.parser, input);
            goto done;
        }
    }
    else {
        if (result != 0) {
            goto done;
        }
    }

    cu.bc = ChpegByteCode_new();
    Compiler_setup_defs(&cu);

    cu.root = GNode_new();

    Compiler_build_tree(&cu, NULL, NULL);
    GNode_reverse(cu.root);

    Compiler_alloc_strings(&cu, cu.root);

    Compiler_alloc_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions alloc'd: %d\n", cu.bc->num_instructions);
#endif
    cu.bc->instructions = (int *)CHPEG_CALLOC(cu.bc->num_instructions, sizeof(int));

    cu.bc->num_instructions = 0;
    Compiler_add_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions after add: %d\n", cu.bc->num_instructions);
#endif

    Compiler_setup_def_addrs(&cu);

#if DEBUG_COMPILER
    CompilationUnit_print(&cu, cu.root, input, 0);
#endif

done:
    if (cu.parser) { ChpegParser_free(cu.parser); }
    if (cu.root) { GNode_free(cu.root); }
    return cu.bc;
}

const ChpegByteCode *Compiler_bytecode()
{
    return &chpeg_bytecode;
}
