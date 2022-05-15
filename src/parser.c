#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "util.h"
#include "parser.h"
#include "opcodes.h"

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

#define ERROR_REPEAT_INHIBIT 0 // probably flawed idea or implementation, don't enable

// VM_TRACE:
// Set to non-zero to compile in support for tracing parser VM instruction execution.
// To use, set parser->vm_trace to non-zero before calling Parser_parse()
#ifndef VM_TRACE
#define VM_TRACE 0
#endif

// VM_PRINT_TREE:
// Set to non-zero to compile in support for printing the parse tree as it is being built.
// To use, set parser->vm_print_tree to non-zero before calling Parser_parse()
#ifndef VM_PRINT_TREE
#define VM_PRINT_TREE 0
#endif

//
// ChpegNode
//

void Node_print(ChpegNode *self, ChpegParser *parser, const unsigned char *input, int depth)
{
    int flags = self->flags;
    char *data = esc_bytes(&input[self->offset], self->length, 40);
    const char *def_name = Parser_def_name(parser, self->def);

    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
        printf(" Begin    Len  DefID  Flags  Def. Name / Data\n");
        printf("---------------------------------------------------------------------------------\n");
    }
    printf("%6lu %6lu %6d | %s%s%s | %*s%s \"%s\"\n",
        self->offset,
        self->length,
        self->def,
        flags & CHPEG_STOP ? "S" : " ",
        flags & CHPEG_IGNORE ? "I" : " ",
        flags & CHPEG_LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : "<NULL>"
        );
    if (data) { CHPEG_FREE(data); data = NULL; }
    for (ChpegNode *p = self->head; p; p = p->next) {
        Node_print(p, parser, input, depth + 1);
    }
    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
    }
}

ChpegNode *Node_new(int def, size_t offset, size_t length, int flags)
{
    ChpegNode *self = (ChpegNode *)CHPEG_MALLOC(sizeof(ChpegNode));
    self->def = def;
    self->offset = offset;
    self->length = length;
    self->flags = flags;
    self->num_children = 0;
    self->head = NULL;
    self->next = NULL;
    return self;
}

void Node_free(ChpegNode *self)
{
    ChpegNode *tmp;
    for (ChpegNode *p = self->head; p; p = tmp) {
        tmp = p->next;
        Node_free(p);
    }
    self->head = NULL;
    CHPEG_FREE(self);
}

ChpegNode *Node_push_child(ChpegNode *self, int def, size_t offset, size_t length, int flags)
{
    ChpegNode *node = Node_new(def, offset, length, flags);
    node->next = self->head;
    self->head = node;
    ++(self->num_children);
    return node;
}

void Node_pop_child(ChpegNode *self)
{
    if (self->head) {
        ChpegNode *tmp = self->head;
        self->head = self->head->next;
        Node_free(tmp);
        --(self->num_children);
    }
}

// 'Unwrap' a ChpegNode, recursively removing unnecessary parent nodes containing only one child.
// In the process, this reverses the reverse node insertion used in tree building, so should only
// be called once on the tree root after a successful parse.
ChpegNode *Node_unwrap(ChpegNode *self)
{
    if (!(self->flags & (CHPEG_STOP | CHPEG_LEAF)) && self->num_children == 1) {
        ChpegNode *tmp = Node_unwrap(self->head);
        self->head = NULL;
        Node_free(self);
        return tmp;
    }
    ChpegNode *p = self->head; self->head = NULL;
    ChpegNode *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = Node_unwrap(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

//
// ChpegParser
//

ChpegParser *Parser_new(const ChpegByteCode *byte_code)
{
    ChpegParser *self = (ChpegParser *)CHPEG_MALLOC(sizeof(ChpegParser));

    self->num_defs = byte_code->num_defs;
    self->def_names = byte_code->def_names;
    self->def_flags = byte_code->def_flags;
    self->def_addrs = byte_code->def_addrs;
    self->num_instructions = byte_code->num_instructions;
    self->instructions = byte_code->instructions;
    self->num_strings = byte_code->num_strings;
    self->strings = byte_code->strings;
    self->str_len = byte_code->str_len;

    self->tree_root = NULL;
    self->max_tree_depth = 256;
    self->max_stack_size = 256 * 8;
    self->error_offset = 0;
    self->error_parent_def = -1;
    self->error_def = -1;
    self->error_inst = -1;
    self->error_expected = -1;

#if VM_TRACE
    self->vm_trace = 0;
#endif
#if VM_PRINT_TREE
    self->vm_print_tree = 0;
#endif

    return self;
}

void Parser_free(ChpegParser *self)
{
    if (self->tree_root) {
        Node_free(self->tree_root);
        self->tree_root = NULL;
    }
    CHPEG_FREE(self);
}

void Parser_print_tree(ChpegParser *self, const unsigned char *input)
{
    Node_print(self->tree_root, self, input, 0);
}

void Parser_expected(ChpegParser *self, int parent_def, int def, int inst, size_t offset, int expected)
{
    if (offset >= self->error_offset && !(def == 0 && inst == -1)) {
        self->error_offset = offset;
        self->error_parent_def = parent_def;
        self->error_def = def;
        self->error_inst = inst;
        self->error_expected = expected;
    }
}

void Parser_print_error(ChpegParser *self, const unsigned char *input)
{
#if ERRORS
    const char *parent_def_name = Parser_def_name(self, self->error_parent_def);
    const char *def_name = Parser_def_name(self, self->error_def);

    if (self->error_expected >= 0) {
        if (self->error_inst >= 0) {
            int op = self->error_inst & 0xff;
            int arg = self->error_inst >> 8;
            char *esc = NULL;
            const char *str = NULL;
            switch (op) {
                case CHPEG_OP_DOT:
                    str = "character"; break;
                case CHPEG_OP_IDENT:
                    str = Parser_def_name(self, arg); break;
                case CHPEG_OP_LIT:
                    esc = esc_bytes((unsigned char *)self->strings[arg], self->str_len[arg], 20);
                    break;
                default: // unhandled op, show instruction in <> for debugging
                    esc = esc_bytes((unsigned char *)&self->error_inst, sizeof(int), 20);
                    break;
            }
            printf("%s \"%s\" in %s at offset %lu\n",
                    self->error_expected ? "Expected" : "Unexpected",
                    str ? str : (esc ? esc : "<NULL>"),
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
            if (esc) {
                CHPEG_FREE(esc);
                esc = NULL;
            }
        }
        else {
            if (parent_def_name) {
                printf("%s %s in %s at offset %lu\n",
                        self->error_expected ? "Expected" : "Unexpected",
                        def_name ? def_name : "<N/A>",
                        parent_def_name ? parent_def_name : "<N/A>",
                        self->error_offset);
            }
            else {
                printf("%s %s at offset %lu\n",
                        self->error_expected ? "Expected" : "Unexpected",
                        def_name ? def_name : "<N/A>",
                        self->error_offset);
            }
        }
    }
    else {
        printf("No errors detected / tracked.\n");
    }
#else
    printf("Error tracking disabled at compile time.\n");
#endif
}

const char *Parser_def_name(ChpegParser *self, int index)
{
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return 0;
}

// TODO: check sanity checks and overflow checks, make macros to make it easier
int Parser_parse(ChpegParser *self, const unsigned char *input, size_t length, size_t *consumed)
{
    int locked = 0, retval = 0;
    size_t offset = 0;

#if ERRORS
    int err_locked = 0;
#endif

#if VM_PRINT_TREE
    int tree_changed = 0;
#endif

    size_t stack[self->max_stack_size]; int top = -1;
    ChpegNode *tree_stack[self->max_tree_depth]; int tree_top = -1;

    const int *instructions = self->instructions;

    int op = 0, arg = 0, pc = 0;

    if (length < 0) { return CHPEG_ERR_INVALID_LENGTH; }

    self->tree_root = Node_new(0, 0, -1, 0);
    if (tree_top >= self->max_tree_depth - 2) return CHPEG_ERR_TREE_STACK_OVERFLOW;
    tree_stack[++tree_top] = self->tree_root;

    unsigned long long cnt = 0, cnt_max = 0;
    cnt_max = (length <= 2642245) ? (length < 128 ? 2097152 : length * length * length) : (unsigned long long)-1LL;

#if SANITY_CHECKS
    int num_inst = self->num_instructions;
    for(cnt = 0; cnt < cnt_max && pc < num_inst; ++cnt, ++pc)
#else
    for(cnt = 0; cnt < cnt_max; ++cnt, ++pc)
#endif
    {
        op = instructions[pc] & 0xff;
        arg = instructions[pc] >> 8;

        /*
        printf("pc=%d op=%d arg=%d top=%d tt=%d stack: ", pc, op, arg, top, tree_top);
        for (int i = 0; i <= top; i++) {
            printf("%d ", stack[i]);
        }
        printf("\n");
        */

#if VM_TRACE
        if (self->vm_trace) {
            if (cnt == 0) {
                fprintf(stderr, "=     CNT   OFFSET       PC           OP   ARG\n");
            }
            char *tmp;
            const char *def_name;
            switch (op) {
                case CHPEG_OP_IDENT:
                case CHPEG_OP_ISUCC:
                    def_name = Parser_def_name(self, arg);
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s%s\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "",
                        def_name ? def_name : "<INVALID>");
                    def_name = NULL;
                    break;
                case CHPEG_OP_LIT:
                case CHPEG_OP_CHRCLS:
                    tmp = esc_bytes(
                        self->strings[arg], self->str_len[arg], 28);
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s\"%s\"\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "",
                        tmp ? tmp : "<NULL>");
                    CHPEG_FREE(tmp);
                    tmp = NULL;
                    break;
                default:
                    fprintf(stderr, "=%8llu %8lu %8d %12s %5d %*s-\n",
                        cnt, offset, pc, Chpeg_op_name(op), arg, tree_top*2, "");
            }
        }
#endif

#if VM_PRINT_TREE
        tree_changed = 0;
#endif

        // Note: don't return from inside this switch, set pc = -1, retval = return value, then break;
        // This is to allow finalization like tree printing (if enabled)

        switch (op) {
            case CHPEG_OP_GOTO: // arg = addr; GOTO addr; pc is set to addr
                pc = arg; break;

// Identifier
            case CHPEG_OP_IDENT: // arg = def; Identifier "call"; on success, next instruction skipped (See ISUCC, IFAIL)
                if (arg < 0) {
                    pc = -1; retval = CHPEG_ERR_INVALID_IDENTIFIER; break;
                }
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                if (!locked) {
                    if (tree_top >= self->max_tree_depth - 2) {
                        pc = -1; retval = CHPEG_ERR_TREE_STACK_OVERFLOW; break;
                    }
                    if (self->def_flags[arg] & (CHPEG_LEAF | CHPEG_IGNORE)) {
                        stack[++top] = 1; locked = 1;
                    }
                    else {
                        stack[++top] = 0;
                    }
                    tree_stack[tree_top+1] = Node_push_child(tree_stack[tree_top], arg, offset, -1, self->def_flags[arg]);
                    ++tree_top;
                }
                else {
                    stack[++top] = 0;
                }
                stack[++top] = offset;
                stack[++top] = pc;
                pc = self->def_addrs[arg];
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

            case CHPEG_OP_ISUCC: // arg = def; Identifier "call" match success, "return", pc restored to pc+1, skipping next instruction
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                pc = stack[top--] + 1;
                --top;
                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                    --tree_top;
                    if (self->def_flags[arg] & CHPEG_IGNORE) {
#if SANITY_CHECKS
                        if (tree_top < 0) {
                            pc = -1; retval = CHPEG_ERR_TREE_STACK_UNDERFLOW; break;
                        }
#endif
                        Node_pop_child(tree_stack[tree_top]);
                    }
                }
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

            case CHPEG_OP_IFAIL: // Identifier "call" match failure, "return", pc restored (next instruction not skipped)
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                pc = stack[top--];
                offset = stack[top--];

#if ERRORS && ERRORS_IDENT
                if (!err_locked) { // Tracking errors here is probably bare minimum of usefulness
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, -1, offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif

                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
#if SANITY_CHECKS
                    if (tree_top < 0) {
                        pc = -1; retval = CHPEG_ERR_TREE_STACK_UNDERFLOW; break;
                    }
#endif
                    Node_pop_child(tree_stack[--tree_top]);
                }
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// Choice
            case CHPEG_OP_CHOICE:
                if (top >= self->max_stack_size - 3) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case CHPEG_OP_CISUCC: // arg = success/fail pc addr
            case CHPEG_OP_CFAIL:
#if SANITY_CHECKS
                if (top < 1) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                top -= 2;
                pc = arg;
                break;

            case CHPEG_OP_CIFAIL:
                // backtrack
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;


// Repeat +
            case CHPEG_OP_RPBEG:
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
#if ERRORS && ERROR_REPEAT_INHIBIT
                stack[++top] = 0; // used to inhibit error tracking after 1st rep
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // offset - backtrack point
                stack[++top] = 0; // cnt (match count)
                break;

            case CHPEG_OP_RPMAT: // arg = loop pc addr
                ++stack[top]; // incr match count
                if (offset != stack[top-1]) { // only loop if consuming
                    stack[top-2] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top-1] = offset; // update backtrack point
                    pc = arg; // continue looping
#if ERRORS && ERROR_REPEAT_INHIBIT
                    if (!err_locked) { // inhibit error tracking after 1st rep
                        stack[top-3] = 1; err_locked = 1;
                    }
#endif
                }
                break;

            case CHPEG_OP_RPDONE: // arg = match fail pc addr
#if SANITY_CHECKS
                if (top < 3) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                offset = stack[top-1];
                for (int i = tree_stack[tree_top]->num_children - stack[top-2]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                if (stack[top] > 0) { // op+ SUCCESS
                    top -= 3;
                }
                else { // op+ FAIL
                    top -= 3;
                    pc = arg;
                }
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (stack[top--]) { err_locked = 0; } // reenable error tracking (if we disabled it)
#endif
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// Repeat *|?
            case CHPEG_OP_RSBEG:
            case CHPEG_OP_RQBEG:
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (!err_locked) { // inhibit error tracking
                    stack[++top] = 1; err_locked = 1;
                }
                else {
                    stack[++top] = 0;
                }
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case CHPEG_OP_RSMAT: // arg = loop pc addr
                if (offset != stack[top]) { // only loop if consuming
                    stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top] = offset; // update backtrack point
                    pc = arg; // continue looping
                } // else next instruction, which is a R*DONE
                break;

            case CHPEG_OP_RSDONE: // * always succeeds, proceeds to next instr.
            case CHPEG_OP_RQDONE: // ? always succeeds, proceeds to next instr.
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                top -= 2;
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (stack[top--]) { err_locked = 0; } // reenable error tracking (if we disabled it)
#endif
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// Repeat ?
            case CHPEG_OP_RQMAT: // no looping for ?
                stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                stack[top] = offset; // update backtrack point
                break;

// Predicate
            case CHPEG_OP_PRED:
                // Predicate begin, should be followed with child instructions,
                // then PMATCH{S,F}, then PNOMAT{S,F}, depending on op (&,!)
                if (top >= self->max_stack_size - 3) {
                    pc = -1; retval = CHPEG_ERR_STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
#if ERRORS
#if ERRORS_PRED
                stack[++top] = pc + 1; // 1st child inst address for error
#endif
                if (!err_locked) {
                    stack[++top] = 1; err_locked = 1;
                }
                else {
                    stack[++top] = 0;
                }
#endif
                break;

            case CHPEG_OP_PMATCHF: // Predicate matched, match is considered failure, arg = failure address
            case CHPEG_OP_PNOMATF: // Predicate not matched, not match is considered failure, arg = failure address
#if ERRORS && ERRORS_PRED
                if (stack[top]) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[stack[top-1]],
                            offset, (CHPEG_OP_PNOMATF == op));
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                goto pred_cleanup;

            case CHPEG_OP_PMATCHS: // Predicate matched, match is considered success; next instruction skipped
                ++pc;
                // passthrough to pred_cleanup

            case CHPEG_OP_PNOMATS: // Predicate not matched, not match is considered success
pred_cleanup:
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = CHPEG_ERR_STACK_UNDERFLOW; break;
                }
#endif
#if ERRORS
                if (stack[top--]) { err_locked = 0; }
#if ERRORS_PRED
                --top;
#endif
#endif
                offset = stack[top--]; // restore saved offset (don't consume)
                for (int i = tree_stack[tree_top]->num_children - stack[top--]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// CharClass
            case CHPEG_OP_CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < length) {
                        int mlen = self->str_len[arg], i;
                        const unsigned char *mstr = self->strings[arg];
                        for (i = 0; i < mlen; ++i) {
                            if (mstr[i] == input[offset]) {
                                ++offset;
                                ++pc;
                                break;
                            }
                            if ((i < mlen - 2) && (mstr[i+1] == '-')) {
                                if ((input[offset] >= mstr[i]) && (input[offset] <= mstr[i+2])) {
                                    ++offset;
                                    ++pc;
                                    break;
                                }
                                i+=2;
                            }
                        }
                        if (i < mlen) break;
                    }
#if ERRORS && ERRORS_TERMINALS
                    if (!err_locked) {
                        Parser_expected(self,
                                tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                                tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                        Parser_print_error(self, input);
#endif
                    }
#endif
                }
                break;

// Literal
            case CHPEG_OP_LIT: // arg = str_id; match literal string; skip next instruction on match
                {
                    int len = self->str_len[arg];
                    if ((offset < (length - (len - 1))) && !memcmp(&input[offset], self->strings[arg], len)) {
                        offset += len;
                        ++pc;
                        break;
                    }
                }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                break;

// Dot
            case CHPEG_OP_DOT: // arg = fail addr; match any char; goto addr on failure
                if (offset < length) { offset++; break; }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                break;

// End
            case CHPEG_OP_SUCC: // overall success
                pc = -1; // we're done
#if SANITY_CHECKS
                if (tree_top < 0) {
                    retval = CHPEG_ERR_TREE_STACK_UNDERFLOW; break;
                }
#endif
                tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                --tree_top;

                // clean up the parse tree, reversing the reverse node insertion in the process
                self->tree_root = Node_unwrap(self->tree_root);
#if VM_PRINT_TREE
                tree_changed = 1;
#endif

                if (top != -1) {
                    retval = CHPEG_ERR_UNEXPECTED_STACK_DATA;
                }
                else if (tree_top != -1) {
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
                retval = CHPEG_ERR_UNKNOWN_INSTRUCTION;
                break;
        }

#if VM_PRINT_TREE
        if (self->vm_print_tree && tree_changed) {
            Parser_print_tree(self, input);
        }
#endif
        if (pc < 0) {
            self->parse_result = retval;
            return retval;
        }
    }

    retval = CHPEG_ERR_RUNAWAY;
    self->parse_result = retval;
    return retval;
}
