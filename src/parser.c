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
// Node
//

void Node_print(Node *self, Parser *parser, const unsigned char *input, int depth)
{
    int flags = self->flags;
    char *data = esc_bytes(&input[self->offset], self->length, 40);
    const char *def_name = Parser_def_name(parser, self->def);

    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
        printf(" Begin    Len  DefID  Flags  Def. Name / Data\n");
        printf("---------------------------------------------------------------------------------\n");
    }
    printf("%6d %6d %6d | %s%s%s | %*s%s \"%s\"\n",
        self->offset,
        self->length,
        self->def,
        flags & STOP ? "S" : " ",
        flags & IGNORE ? "I" : " ",
        flags & LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : "<NULL>"
        );
    if (data) { CHPEG_FREE(data); data = NULL; }
    for (Node *p = self->head; p; p = p->next) {
        Node_print(p, parser, input, depth + 1);
    }
    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
    }
}

Node *Node_new(int def, int offset, int length, int flags)
{
    Node *self = (Node *)CHPEG_MALLOC(sizeof(Node));
    self->def = def;
    self->offset = offset;
    self->length = length;
    self->flags = flags;
    self->num_children = 0;
    self->head = NULL;
    self->next = NULL;
    return self;
}

void Node_free(Node *self)
{
    Node *tmp;
    for (Node *p = self->head; p; p = tmp) {
        tmp = p->next;
        Node_free(p);
    }
    self->head = NULL;
    CHPEG_FREE(self);
}

Node *Node_push_child(Node *self, int def, int offset, int length, int flags)
{
    Node *node = Node_new(def, offset, length, flags);
    node->next = self->head;
    self->head = node;
    ++(self->num_children);
    return node;
}

void Node_pop_child(Node *self)
{
    if (self->head) {
        Node *tmp = self->head;
        self->head = self->head->next;
        Node_free(tmp);
        --(self->num_children);
    }
}

// 'Unwrap' a Node, recursively removing unnecessary parent nodes containing only one child.
// In the process, this reverses the reverse node insertion used in tree building, so should only
// be called once on the tree root after a successful parse.
Node *Node_unwrap(Node *self)
{
    if (!(self->flags & (STOP|LEAF)) && self->num_children == 1) {
        Node *tmp = Node_unwrap(self->head);
        self->head = NULL;
        Node_free(self);
        return tmp;
    }
    Node *p = self->head; self->head = NULL;
    Node *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = Node_unwrap(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

//
// Parser
//

Parser *Parser_new(const ByteCode *byte_code)
{
    Parser *self = (Parser *)CHPEG_MALLOC(sizeof(Parser));

    self->bc = byte_code;

    self->tree_root = NULL;
    self->max_tree_depth = 256;
    self->max_stack_size = 256 * 8;
    self->error_offset = -1;
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

void Parser_free(Parser *self)
{
    if (self->tree_root) {
        Node_free(self->tree_root);
        self->tree_root = NULL;
    }
    CHPEG_FREE(self);
}

void Parser_print_tree(Parser *self, const unsigned char *input)
{
    Node_print(self->tree_root, self, input, 0);
}

void Parser_expected(Parser *self, int parent_def, int def, int inst, int offset, int expected)
{
    if (offset >= self->error_offset && !(def == 0 && inst == -1)) {
        self->error_offset = offset;
        self->error_parent_def = parent_def;
        self->error_def = def;
        self->error_inst = inst;
        self->error_expected = expected;
    }
}

void Parser_print_error(Parser *self, const unsigned char *input)
{
#if ERRORS
    const char *parent_def_name = Parser_def_name(self, self->error_parent_def);
    const char *def_name = Parser_def_name(self, self->error_def);

    if (self->error_offset >= 0) {
        if (self->error_inst >= 0) {
            int op = self->error_inst & 0xff;
            int arg = self->error_inst >> 8;
            char *esc = NULL;
            const char *str = NULL;
            switch (op) {
                case DOT:
                    str = "character"; break;
                case IDENT:
                    str = Parser_def_name(self, arg); break;
                case LIT:
                    esc = esc_bytes((unsigned char *)self->bc->strings[arg], self->bc->str_len[arg], 20);
                    break;
                default: // unhandled op, show instruction in <> for debugging
                    esc = esc_bytes((unsigned char *)&self->error_inst, sizeof(int), 20);
                    break;
            }
            printf("%s \"%s\" in %s at offset %d\n",
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
                printf("%s %s in %s at offset %d\n",
                        self->error_expected ? "Expected" : "Unexpected",
                        def_name ? def_name : "<N/A>",
                        parent_def_name ? parent_def_name : "<N/A>",
                        self->error_offset);
            }
            else {
                printf("%s %s at offset %d\n",
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

const char *Parser_def_name(Parser *self, int index)
{
    if (index >= 0 && index < self->bc->num_defs) {
        return self->bc->def_names[index];
    }
    return 0;
}

// TODO: check sanity checks and overflow checks, make macros to make it easier
int Parser_parse(Parser *self, const unsigned char *input, int size)
{
    int offset = 0, locked = 0, retval = 0;

#if ERRORS
    int err_locked = 0;
#endif

#if VM_PRINT_TREE
    int tree_changed = 0;
#endif

    int stack[self->max_stack_size]; int top = -1;
    Node *tree_stack[self->max_tree_depth]; int tree_top = -1;

    const int *instructions = self->bc->instructions;

    int op = 0, arg = 0, pc = 0;

    if (size < 0) { return INVALID_SIZE; }

    self->tree_root = Node_new(0, 0, -1, 0);
    if (tree_top >= self->max_tree_depth - 2) return TREE_STACK_OVERFLOW;
    tree_stack[++tree_top] = self->tree_root;

    unsigned long long cnt = 0, cnt_max = 0;
    cnt_max = (size <= 2642245) ? (size < 128 ? 2097152 : size * size * size) : (unsigned long long)-1LL;

#if SANITY_CHECKS
    int num_inst = self->bc->num_instructions;
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
            unsigned char *tmp;
            const char *def_name;
            switch (op) {
                case IDENT:
                case ISUCC:
                    def_name = Parser_def_name(self, arg);
                    fprintf(stderr, "=%8llu %8d %8d %12s %5d %*s%s\n",
                        cnt, offset, pc, op_name(op), arg, tree_top*2, "",
                        def_name ? def_name : "<INVALID>");
                    def_name = NULL;
                    break;
                case LIT:
                case CHRCLS:
                    tmp = esc_string(
                        self->strings[arg], self->str_len[arg], 28);
                    fprintf(stderr, "=%8llu %8d %8d %12s %5d %*s\"%s\"\n",
                        cnt, offset, pc, op_name(op), arg, tree_top*2, "",
                        tmp ? tmp : (unsigned char *)"<NULL>");
                    CHPEG_FREE(tmp);
                    tmp = NULL;
                    break;
                default:
                    fprintf(stderr, "=%8llu %8d %8d %12s %5d %*s-\n",
                        cnt, offset, pc, op_name(op), arg, tree_top*2, "");
            }
        }
#endif

#if VM_PRINT_TREE
        tree_changed = 0;
#endif

        // Note: don't return from inside this switch, set pc = -1, retval = return value, then break;
        // This is to allow finalization like tree printing (if enabled)

        switch (op) {
            case GOTO: // arg = addr; GOTO addr; pc is set to addr
                pc = arg; break;

// Identifier
            case IDENT: // arg = def; Identifier "call"; on success, next instruction skipped (See ISUCC, IFAIL)
                if (arg < 0) {
                    pc = -1; retval = INVALID_IDENTIFIER; break;
                }
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = STACK_OVERFLOW; break;
                }
                if (!locked) {
                    if (tree_top >= self->max_tree_depth - 2) {
                        pc = -1; retval = TREE_STACK_OVERFLOW; break;
                    }
                    if (self->bc->def_flags[arg] & (LEAF | IGNORE)) {
                        stack[++top] = 1; locked = 1;
                    }
                    else {
                        stack[++top] = 0;
                    }
                    tree_stack[tree_top+1] = Node_push_child(tree_stack[tree_top], arg, offset, -1, self->bc->def_flags[arg]);
                    ++tree_top;
                }
                else {
                    stack[++top] = 0;
                }
                stack[++top] = offset;
                stack[++top] = pc;
                pc = self->bc->def_addrs[arg];
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

            case ISUCC: // arg = def; Identifier "call" match success, "return", pc restored to pc+1, skipping next instruction
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
                }
#endif
                pc = stack[top--] + 1;
                --top;
                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                    --tree_top;
                    if (self->bc->def_flags[arg] & IGNORE) {
#if SANITY_CHECKS
                        if (tree_top < 0) {
                            pc = -1; retval = TREE_STACK_UNDERFLOW; break;
                        }
#endif
                        Node_pop_child(tree_stack[tree_top]);
                    }
                }
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

            case IFAIL: // Identifier "call" match failure, "return", pc restored (next instruction not skipped)
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
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
                        pc = -1; retval = TREE_STACK_UNDERFLOW; break;
                    }
#endif
                    Node_pop_child(tree_stack[--tree_top]);
                }
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;

// Choice
            case CHOICE:
                if (top >= self->max_stack_size - 3) {
                    pc = -1; retval = STACK_OVERFLOW; break;
                }
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case CISUCC: // arg = success/fail pc addr
            case CFAIL:
#if SANITY_CHECKS
                if (top < 1) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
                }
#endif
                top -= 2;
                pc = arg;
                break;

            case CIFAIL:
                // backtrack
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
#if VM_PRINT_TREE
                tree_changed = 1;
#endif
                break;


// Repeat +
            case RPBEG:
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = STACK_OVERFLOW; break;
                }
#if ERRORS && ERROR_REPEAT_INHIBIT
                stack[++top] = 0; // used to inhibit error tracking after 1st rep
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // offset - backtrack point
                stack[++top] = 0; // cnt (match count)
                break;

            case RPMAT: // arg = loop pc addr
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

            case RPDONE: // arg = match fail pc addr
#if SANITY_CHECKS
                if (top < 3) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
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
            case RSBEG:
            case RQBEG:
                if (top >= self->max_stack_size - 4) {
                    pc = -1; retval = STACK_OVERFLOW; break;
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

            case RSMAT: // arg = loop pc addr
                if (offset != stack[top]) { // only loop if consuming
                    stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top] = offset; // update backtrack point
                    pc = arg; // continue looping
                } // else next instruction, which is a R*DONE
                break;

            case RSDONE: // * always succeeds, proceeds to next instr.
            case RQDONE: // ? always succeeds, proceeds to next instr.
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
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
            case RQMAT: // no looping for ?
                stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                stack[top] = offset; // update backtrack point
                break;

// Predicate
            case PRED:
                // Predicate begin, should be followed with child instructions,
                // then PMATCH{S,F}, then PNOMAT{S,F}, depending on op (&,!)
                if (top >= self->max_stack_size - 3) {
                    pc = -1; retval = STACK_OVERFLOW; break;
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

            case PMATCHF: // Predicate matched, match is considered failure, arg = failure address
            case PNOMATF: // Predicate not matched, not match is considered failure, arg = failure address
#if ERRORS && ERRORS_PRED
                if (stack[top]) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[stack[top-1]],
                            offset, (PNOMATF == op));
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                goto pred_cleanup;

            case PMATCHS: // Predicate matched, match is considered success; next instruction skipped
                ++pc;
                // passthrough to pred_cleanup

            case PNOMATS: // Predicate not matched, not match is considered success
pred_cleanup:
#if SANITY_CHECKS
                if (top < 2) {
                    pc = -1; retval = STACK_UNDERFLOW; break;
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
            case CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < size) {
                        int mlen = self->bc->str_len[arg], i;
                        const unsigned char *mstr = self->bc->strings[arg];
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
            case LIT: // arg = str_id; match literal string; skip next instruction on match
                {
                    int len = self->bc->str_len[arg];
                    if ((offset < (size - (len - 1))) && !memcmp(&input[offset], self->bc->strings[arg], len)) {
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
            case DOT: // arg = fail addr; match any char; goto addr on failure
                if (offset < size) { offset++; break; }
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
            case SUCC: // overall success
                pc = -1; // we're done
#if SANITY_CHECKS
                if (tree_top < 0) {
                    retval = TREE_STACK_UNDERFLOW; break;
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
                    retval = UNEXPECTED_STACK_DATA;
                }
                else if (tree_top != -1) {
                    retval = UNEXPECTED_TREE_STACK_DATA;
                }
                else {
                    retval = offset;
                }
                break;

            case FAIL: // overall failure
                pc = -1; // we're done
                retval = arg < 0 ? arg : PARSE_FAILED;
                break;

            default:
                pc = -1; // we're done
                retval = UNKNOWN_INSTRUCTION;
                break;
        }

#if VM_PRINT_TREE
        if (self->vm_print_tree && tree_changed) {
            Parser_print_tree(self, input);
        }
#endif
        if (pc < 0) {
            return retval;
        }
    }

    return RUNAWAY;
}
