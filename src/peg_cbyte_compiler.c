#define DEBUG_COMPILER 0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "peg_cbyte_parser.h"
#include "peg_cbyte_compiler.h"

//
// Gnode
//

GNode *GNode_new()
{
    GNode *self = (GNode *)malloc(sizeof(GNode));

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

void GNode_free(GNode *self)
{
    GNode *tmp;
    for (GNode *p = self->head; p; p = tmp) {
        tmp = p->next;
        GNode_free(p);
    }
    self->head = NULL;
    free(self);
}

GNode *GNode_push_child(GNode *self, GNode *child)
{
    child->next = self->head;
    self->head = child;
    ++(self->num_children);
    return child;
}

void GNode_pop_child(GNode *self)
{
    if (self->head) {
        GNode *tmp = self->head;
        self->head = self->head->next;
        GNode_free(tmp);
        --(self->num_children);
    }
}

GNode *GNode_reverse(GNode *self)
{
    GNode *p = self->head; self->head = NULL;
    GNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = GNode_reverse(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

void GNode_print(GNode *self, CompilationUnit *cu, const unsigned char *input, int depth)
{
    int flags = 0;
    unsigned char *data = NULL;
    const char *def_name = Parser_def_name(cu->parser, self->type); 

    Node *node = self->node;
    if (node) {
        data = esc_string(&input[node->offset], node->length, 40);
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
        self->type,
        self->parse_state,
        self->parent_next_state,
        self->parent_fail_state,
        flags & STOP ? "S" : " ",
        flags & IGNORE ? "I" : " ",
        flags & LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : (unsigned char *)""
        );
    if (data) { free(data); data = NULL; }

    for (GNode *p = self->head; p; p = p->next) {
        GNode_print(p, cu, input, depth + 1);
    }
} 

//
// Compiler
//

void Compiler_setup_defs(CompilationUnit *cu)
{
    Node *p = NULL;
    int i = 0, j = 0;

    cu->bc->num_defs = cu->parser->tree_root->num_children;
#if DEBUG_COMPILER
    printf("Compiler_setup_defs: num_defs=%d\n", cu->bc->num_defs);
#endif

    cu->bc->def_names = (char **)malloc(cu->bc->num_defs * sizeof(char *));
    cu->bc->def_flags = (int *)malloc(cu->bc->num_defs * sizeof(int));
    cu->bc->def_addrs = (int *)malloc(cu->bc->num_defs * sizeof(int));

    for (p = cu->parser->tree_root->head, i = 0; p; p = p->next, ++i) {
        if (PEG_DEFINITION != p->def) { continue; }

        Node *tmp = p->head; // Identifier, definition name
        if (NULL == tmp || PEG_IDENTIFIER != tmp->def) { continue; }
        cu->bc->def_names[i] = (char *)malloc(1 + tmp->length);
        memcpy(cu->bc->def_names[i], &cu->input[tmp->offset], tmp->length);
        cu->bc->def_names[i][tmp->length] = '\0';

        int flags = 0;
        tmp = tmp->next; // Options, flags
        if (NULL != tmp && PEG_OPTIONS == tmp->def) {
            for (j = 0; j < tmp->length; ++j) {
                switch(cu->input[tmp->offset + j]) {
                    case 'S': flags |= STOP; break;
                    case 'I': flags |= IGNORE; break;
                    case 'L': flags |= LEAF; break;
                }
            }
            p->head->next = tmp->next; // eliminate OPTIONS node
            Node_free(tmp);
        }
        cu->bc->def_flags[i] = flags;
#if DEBUG_COMPILER
        printf("Compile_setup_defs: def[%i]: name=%s flags=%d\n", i, cu->bc->def_names[i], cu->bc->def_flags[i]);
#endif
    }
}

void Compiler_setup_def_addrs(CompilationUnit *cu)
{
    int i = 0;
    GNode *p = NULL;
    for (i = 0, p = cu->root->head; p; p = p->next, ++i) {
        cu->bc->def_addrs[i] = p->head->next->parse_state - 1;
    }
}

int Compiler_find_def(CompilationUnit *cu, Node *ident)
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

void Compiler_build_tree(CompilationUnit *cu, Node *np, GNode *gp)
{
    if (NULL == np) { np = cu->parser->tree_root; }
    if (NULL == gp) { gp = cu->root; }
    gp->node = np;
    gp->type = np->def;
    for (Node *p = np->head; p; p = p->next) {
        GNode *g = GNode_new();
        Compiler_build_tree(cu, p, g);
        GNode_push_child(gp, g);
    }
}

static inline int Compiler_alloc_inst(CompilationUnit *cu)
{
    return cu->bc->num_instructions++;
}

void Compiler_alloc_instructions(CompilationUnit *cu, GNode *gp)
{
    switch (gp->type) {
        case PEG_GRAMMAR:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
            }
            break;
        case PEG_DEFINITION:
            Compiler_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->next->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case PEG_CHOICE:
            gp->parse_state = Compiler_alloc_inst(cu);
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
                p->parent_next_state = Compiler_alloc_inst(cu);
                p->parent_fail_state = Compiler_alloc_inst(cu);
            }
            Compiler_alloc_inst(cu);
            break;
        case PEG_SEQUENCE:
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_instructions(cu, p);
            }
            gp->parse_state = gp->head->parse_state;
            break;
        case PEG_REPEAT:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_instructions(cu, gp->head);
            gp->head->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case PEG_PREDICATE:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_instructions(cu, gp->head->next);
            gp->head->next->parent_next_state = Compiler_alloc_inst(cu);
            gp->head->next->parent_fail_state = Compiler_alloc_inst(cu);
            break;
        case PEG_DOT:
            gp->parse_state = Compiler_alloc_inst(cu);
            break;
        case PEG_IDENTIFIER:
        case PEG_CHARCLASS:
        case PEG_LITERAL:
            gp->parse_state = Compiler_alloc_inst(cu);
            Compiler_alloc_inst(cu);
            break;
    }
}

static inline void Compiler_add_inst(CompilationUnit *cu, int inst)
{
    cu->bc->instructions[cu->bc->num_instructions++] = inst;
}

void Compiler_add_instructions(CompilationUnit *cu, GNode *gp)
{
    switch (gp->type) {
        case PEG_GRAMMAR:
            Compiler_add_inst(cu, INST(IDENT, 0));
            Compiler_add_inst(cu, INST(FAIL, 0));
            Compiler_add_inst(cu, INST(SUCC, 0));
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_add_instructions(cu, p);
            }
            break;
        case PEG_DEFINITION:
            Compiler_add_instructions(cu, gp->head->next);
            Compiler_add_inst(cu, INST(ISUCC, Compiler_find_def(cu, gp->head->node)));
            Compiler_add_inst(cu, INST(IFAIL, 0));
            break;
        case PEG_CHOICE:
            Compiler_add_inst(cu, INST(CHOICE, 0));
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_add_instructions(cu, p);
                Compiler_add_inst(cu, INST(CISUCC, gp->parent_next_state - 1));
                Compiler_add_inst(cu, INST(CIFAIL, 0));
            }
            Compiler_add_inst(cu, INST(CFAIL, gp->parent_fail_state - 1));
            break;
        case PEG_SEQUENCE:
            for (GNode *p = gp->head; p; p = p->next) {
                p->parent_next_state = p->next ? p->next->parse_state : gp->parent_next_state;
                p->parent_fail_state = gp->parent_fail_state;
                Compiler_add_instructions(cu, p);
            }
            break;
        case PEG_REPEAT:
            {
                unsigned char op = cu->input[gp->head->next->node->offset];
                switch (op) {
                    case '+':
                        Compiler_add_inst(cu, INST(RPBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, INST(RPMAT, gp->head->parse_state - 1));
                        Compiler_add_inst(cu, INST(RPDONE, gp->parent_fail_state - 1));
                        break;
                    case '*':
                        Compiler_add_inst(cu, INST(RSBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, INST(RSMAT, gp->head->parse_state - 1));
                        Compiler_add_inst(cu, INST(RSDONE, 0));
                        break;
                    case '?':
                        Compiler_add_inst(cu, INST(RQBEG, 0));
                        Compiler_add_instructions(cu, gp->head);
                        Compiler_add_inst(cu, INST(RQMAT, 0));
                        Compiler_add_inst(cu, INST(RQDONE, 0));
                        break;
                }
            }
            break;
        case PEG_PREDICATE:
            {
                unsigned char op = cu->input[gp->head->node->offset];
                switch (op) {
                    case '&':
                        Compiler_add_inst(cu, INST(PRED, 0));
                        Compiler_add_instructions(cu, gp->head->next);
                        Compiler_add_inst(cu, INST(PMATCHS, gp->parent_fail_state - 1));
                        Compiler_add_inst(cu, INST(PNOMATF, gp->parent_fail_state - 1));
                        break;
                    case '!':
                        Compiler_add_inst(cu, INST(PRED, 0));
                        Compiler_add_instructions(cu, gp->head->next);
                        Compiler_add_inst(cu, INST(PMATCHF, gp->parent_fail_state - 1));
                        Compiler_add_inst(cu, INST(PNOMATS, gp->parent_fail_state - 1));
                        break;
                }
            }
            break;
        case PEG_DOT:
            Compiler_add_inst(cu, INST(DOT, gp->parent_fail_state - 1));
            break;
        case PEG_IDENTIFIER:
            Compiler_add_inst(cu, INST(IDENT, Compiler_find_def(cu, gp->node)));
            Compiler_add_inst(cu, INST(GOTO, gp->parent_fail_state - 1));
            break;
        case PEG_CHARCLASS:
            Compiler_add_inst(cu, INST(CHRCLS, gp->val.ival));
            Compiler_add_inst(cu, INST(GOTO, gp->parent_fail_state - 1));
            break;
        case PEG_LITERAL:
            Compiler_add_inst(cu, INST(LIT, gp->val.ival));
            Compiler_add_inst(cu, INST(GOTO, gp->parent_fail_state - 1));
            break;
    }
}

int Compiler_alloc_string(CompilationUnit *cu, const unsigned char *str, int len)
{
    for (int i = 0; i < cu->bc->num_strings; ++i) {
        if (0 == memcmp(cu->bc->strings[i], str, len)) {
            return i;
        }
    }
    if (NULL == cu->bc->strings) {
        cu->bc->num_strings = 0; 
        cu->strings_allocated = 16; 
        cu->bc->strings = (unsigned char **)malloc(cu->strings_allocated * sizeof(unsigned char *));
        cu->bc->str_len = (int *)malloc(cu->strings_allocated * sizeof(int));
    }
    if (cu->bc->num_strings == cu->strings_allocated) {
        cu->strings_allocated *= 2; 
        cu->bc->strings = (unsigned char **)realloc(cu->bc->strings, cu->strings_allocated * sizeof(unsigned char *));
        cu->bc->str_len = (int *)realloc(cu->bc->str_len, cu->strings_allocated * sizeof(int));
    }
    int idx = cu->bc->num_strings++;
    cu->bc->strings[idx] = (unsigned char *)malloc(len + 1);
    memcpy(cu->bc->strings[idx], str, len);
    cu->bc->strings[idx][len] = '\0'; // safety
    cu->bc->str_len[idx] = len; 
    return idx;
}

void Compiler_alloc_strings(CompilationUnit *cu, GNode *gp)
{
    switch (gp->type) {
        case PEG_LITERAL:
        case PEG_CHARCLASS:
            {
                int len = 0, offset = 0;
                for (GNode *p = gp->head; p; p = p->next) {
                    Compiler_alloc_strings(cu, p);
                    len += p->value_len;
                }
                unsigned char *str = (unsigned char *)malloc(len);
                for (GNode *p = gp->head; p; p = p->next) {
                    memcpy(str+offset, p->val.cval, p->value_len);
                    offset += p->value_len;
                }
                gp->val.ival = Compiler_alloc_string(cu, str, len);
#if DEBUG_COMPILER
                unsigned char *tmp = esc_string(str, len, 20);
                printf("PEG LITERAL/CHARCLASS %s %d\n", tmp, gp->val.ival);
                free(tmp);
#endif
                free(str);
            }
            break;
        case PEG_CHARRANGE:
            {
                for (GNode *p = gp->head; p; p = p->next) {
                    Compiler_alloc_strings(cu, p);
                }
                gp->val.cval[0] = gp->head->val.cval[0]; 
                gp->val.cval[1] = '-'; 
                gp->val.cval[2] = gp->head->next->val.cval[0]; 
                gp->value_len = 3;
#if DEBUG_COMPILER
                unsigned char *tmp = esc_string(gp->val.cval, gp->value_len, 10);
                printf("PEG_CHARRANGE %s\n", tmp);
                free(tmp);
#endif
            }
            break;
        case PEG_PLAINCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset];
                gp->value_len = 1;
#if DEBUG_COMPILER
                unsigned char *tmp = esc_string(gp->val.cval, gp->value_len, 10);
                printf("PEG_PLAINCHAR %s\n", tmp);
                free(tmp);
#endif
            }
            break;
        case PEG_ESCCHAR:
            {
                gp->val.cval[0] = cu->input[gp->node->offset + 1];
                gp->value_len = 1;
                switch (gp->val.cval[0]) {
                    case 'n': gp->val.cval[0] = '\n'; break;
                    case 'r': gp->val.cval[0] = '\r'; break;
                    case 't': gp->val.cval[0] = '\t'; break;
                }
#if DEBUG_COMPILER
                unsigned char *tmp = esc_string(gp->val.cval, gp->value_len, 10);
                printf("PEG_ESCCHAR %s\n", tmp);
                free(tmp);
#endif
            }
            break;
        case PEG_OCTCHAR:
            {
                int val = 0; int len = gp->node->length - 1;
                const unsigned char *ip = cu->input + gp->node->offset + 1;
                for (int i = 0; i < len; ++i) {
                    val = (val << 3) | (ip[i] - '0');
                }
                gp->val.cval[0] = val & 255;
                gp->value_len = 1;
#if DEBUG_COMPILER
                unsigned char *tmp = esc_string(gp->val.cval, gp->value_len, 10);
                printf("PEG_OCTCHAR %s\n", tmp);
                free(tmp);
#endif
            }
            break;
        default:
            for (GNode *p = gp->head; p; p = p->next) {
                Compiler_alloc_strings(cu, p);
            }
            break;
    }
}

ByteCode *Compiler_compile(const unsigned char *input, int size, int *result_return, int verbose)
{
    CompilationUnit cu;

    cu.parser = Parser_new(&peg_byte_code);
    cu.input = input;
    cu.bc = NULL;
    cu.root = NULL;
    cu.strings_allocated = 0;

    int result = Parser_parse(cu.parser, input, size);
    if (result_return) {
        *result_return = result;
    }

    if (verbose) {
        if (result >= 0) {
            if (result == size) {
                printf("parse ok.\n");
            }
            else {
                printf("parse succeeded but consumed %d bytes out of %d\n", result, size);
            }
            if (verbose & 0x2) {
                Parser_print_tree(cu.parser, input);
            }
        }
        else {
            printf("parse failed with result: %d\n", result);
            Parser_print_error(cu.parser, input);
            goto done; 
        }
    }
    else {
        if (result < 0) {
            goto done;
        }
    }

    cu.bc = ByteCode_new();
    Compiler_setup_defs(&cu);

    cu.root = GNode_new();

    Compiler_build_tree(&cu, NULL, NULL);
    GNode_reverse(cu.root);

    Compiler_alloc_strings(&cu, cu.root);

    Compiler_alloc_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions alloc'd: %d\n", cu.bc->num_instructions);
#endif
    cu.bc->instructions = (int *)calloc(cu.bc->num_instructions, sizeof(int));

    cu.bc->num_instructions = 0; 
    Compiler_add_instructions(&cu, cu.root);
#if DEBUG_COMPILER
    printf("instructions after add: %d\n", cu.bc->num_instructions);
#endif

    Compiler_setup_def_addrs(&cu);

#if DEBUG_COMPILER
    GNode_print(cu.root, &cu, input, 0);
#endif

done:
    if (cu.parser) { Parser_free(cu.parser); }
    if (cu.root) { GNode_free(cu.root); }
    return cu.bc;
}
