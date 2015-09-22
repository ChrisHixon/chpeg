#define SANITY_CHECKS 1
#define DEBUG_ERRORS 0
#define ERRORS 1
#define ERRORS_PRED 1
#define ERRORS_IDENT 1
#define ERRORS_TERMINALS 1
#define ERROR_REPEAT_INHIBIT 0 // probably flawed idea, don't enable

static char *OpNames[] =
{
    "GOTO",
    "IDENT",
    "ISUCC",
    "IFAIL",
    "RSBEG",
    "RQBEG",
    "CHOICE",
    "CISUCC",
    "CFAIL",
    "CIFAIL",
    "RPBEG",
    "RPMAT",
    "RPDONE",
    "RSMAT",
    "RSDONE",
    "RQDONE",
    "RQMAT",
    "PRED",
    "PMATCHF",
    "PNOMATF",
    "PMATCHS",
    "PNOMATS",
    "CHRCLS",
    "LIT",
    "DOT",
    "SUCC",
    "FAIL"
};

// debugging print helper, caller must free() returned value
unsigned char *esc_string(const unsigned char *str, int str_length, int limit)
{
    static const char *hex_digit = "0123456789ABCDEF";
    static const char *esc_chars = "\\\"\t\r\n";
    if (!str || str_length < 0) { return NULL; }
    if (limit > 0 && limit < 3) { return NULL; }

    int clen = 0, len = 0, i = 0, trunc = 0;
    unsigned char c = 0;
    unsigned char *res = NULL;

    for (i = 0; i < str_length; ++i) {
        clen = 1;
        c = str[i];
        if (strchr(esc_chars, c)) {
            clen = 2;
        }
        else if (c < 32 || c > 126) {
            clen = 4;
        }

        if (limit && len + clen > limit - 3) {
            trunc = 1;
            break;
        }
        len += clen;
    }

    clen = i;
    if (trunc) { len += 3; }

    res = (unsigned char *)malloc(len + 1);
    unsigned char *p = res;
    for (i = 0; i < clen; ++i) {
        c = str[i];
        if (strchr(esc_chars, c)) {
            *p++ = '\\';
            switch (c) {
                case '\r': *p++ = 'r'; break;
                case '\t': *p++ = 't'; break;
                case '\n': *p++ = 'n'; break;
                default: *p++ = c; break;
            }
        }
        else if (c < 32 || c > 126) {
            *p++ = '\\';
            *p++ = 'x';
            *p++ = hex_digit[(c >> 4) & 0xf];
            *p++ = hex_digit[c & 0xf];
        }
        else {
            *p++ = c;
        }
    }
    if (trunc) { *p++ = '.'; *p++ = '.'; *p++ = '.'; }
    *p = '\0';
    return res;
}

void Node_print(Node *self, Parser *parser, const unsigned char *input, int depth)
{
    int flags = self->flags;
    unsigned char *data = esc_string(&input[self->offset], self->length, 40);
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
        data ? data : (unsigned char *)""
        );
    if (data) { free(data); data = NULL; }
    for (Node *p = self->head; p; p = p->next) {
        Node_print(p, parser, input, depth + 1);
    }
} 

Node *Node_new(int def, int offset, int length, int flags)
{
    Node *self = (Node *)malloc(sizeof(Node));
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
    free(self);
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
    Parser *self = (Parser *)malloc(sizeof(Parser));

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
    self->error_offset = -1;
    self->error_parent_def = -1;
    self->error_def = -1;
    self->error_inst = -1;
    self->error_expected = -1;

    return self;
}

void Parser_free(Parser *self)
{
    if (self->tree_root) {
        Node_free(self->tree_root);
        self->tree_root = NULL;
    }
    free(self);
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
            unsigned char *esc = NULL;
            const char *str = NULL;
            switch (op) {
                case DOT:
                    str = "character"; break;
                case IDENT:
                    str = Parser_def_name(self, arg); break;
                case LIT:
                    esc = esc_string((unsigned char *)self->strings[arg], self->str_len[arg], 20);
                    break;
                default: // unhandled op, show instruction in <> for debugging
                    esc = esc_string((unsigned char *)&self->error_inst, sizeof(int), 20);
                    break;
            }
            printf("%s \"%s\" in %s at offset %d\n",
                    self->error_expected ? "Expected" : "Unexpected",
                    str ? (unsigned char *)str : esc,
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
            if (esc) {
                free(esc);
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
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return 0;
}

// TODO: check sanity checks and overflow checks, make macros to make it easier
int Parser_parse(Parser *self, const unsigned char *input, int size)
{
    int offset = 0, locked = 0;

#if ERRORS
    int err_locked = 0;
#endif

    int stack[self->max_stack_size]; int top = -1;
    Node *tree_stack[self->max_tree_depth]; int tree_top = -1;

    const int *instructions = self->instructions;

    int op = 0, arg = 0, pc = 0; 

    if (size < 0) { return INVALID_SIZE; }

    self->tree_root = Node_new(0, 0, -1, 0);
    if (tree_top >= self->max_tree_depth - 2) return TREE_STACK_OVERFLOW;
    tree_stack[++tree_top] = self->tree_root;

    unsigned long long cnt = 0, cnt_max = 0;
    cnt_max = (size <= 2642245) ? (size < 128 ? 2097152 : size * size * size) : (unsigned long long)-1LL;
#if SANITY_CHECKS
    int num_inst = self->num_instructions;
    for(cnt = 0; cnt < cnt_max && pc < num_inst; ++cnt, ++pc) {
#else
    for(cnt = 0; cnt < cnt_max; ++cnt, ++pc) {
#endif
        op = instructions[pc] & 0xff;
        arg = instructions[pc] >> 8;
        
        /*
        printf("pc=%d op=%d arg=%d top=%d tt=%d stack: ", pc, op, arg, top, tree_top);
        for (int i = 0; i <= top; i++) {
            printf("%d ", stack[i]);
        }
        printf("\n");
        */

        switch (op) {
            case GOTO: // arg = addr; GOTO addr; pc is set to addr
                pc = arg; break;

// Identifier
            case IDENT: // arg = def; Identifier "call"; on success, next instruction skipped (See ISUCC, IFAIL)
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW; 
                if (!locked) {
                    if (tree_top >= self->max_tree_depth - 2) return TREE_STACK_OVERFLOW;
                    if (self->def_flags[arg] & (LEAF | IGNORE)) {
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
                break;

            case ISUCC: // arg = def; Identifier "call" match success, "return", pc restored to pc+1, skipping next instruction
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
#endif
                pc = stack[top--] + 1;
                --top;
                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                    --tree_top;
                    if (self->def_flags[arg] & IGNORE) { 
#if SANITY_CHECKS
                        if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                        Node_pop_child(tree_stack[tree_top]);
                    }
                }
                break;

            case IFAIL: // Identifier "call" match failure, "return", pc restored (next instruction not skipped)
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
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
                    if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                    Node_pop_child(tree_stack[--tree_top]);
                }
                break;

// Choice
            case CHOICE:
                if (top >= self->max_stack_size - 3) return STACK_OVERFLOW;
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case CISUCC: // arg = success/fail pc addr
            case CFAIL:
#if SANITY_CHECKS
                if (top < 1) return STACK_UNDERFLOW;
#endif
                top -= 2;
                pc = arg;
                break;

            case CIFAIL:
                // backtrack
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                break;


// Repeat +
            case RPBEG:
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW; 
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
                if (top < 3) return STACK_UNDERFLOW;
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
                break;

// Repeat *|?
            case RSBEG:
            case RQBEG:
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW;
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
                if (top < 2) return STACK_UNDERFLOW;
#endif
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                top -= 2;
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (stack[top--]) { err_locked = 0; } // reenable error tracking (if we disabled it)
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
                if (top >= self->max_stack_size - 3) return STACK_OVERFLOW;
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
                if (top < 2) return STACK_UNDERFLOW;
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
                break;

// CharClass
            case CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < size) {
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
            case LIT: // arg = str_id; match literal string; skip next instruction on match
                {
                    int len = self->str_len[arg];
                    if ((offset < (size - (len - 1))) && !memcmp(&input[offset], self->strings[arg], len)) {
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
#if SANITY_CHECKS
                if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                --tree_top;
                self->tree_root = Node_unwrap(self->tree_root);
                if (top != -1) return UNEXPECTED_STACK_DATA;
                if (tree_top != -1) return UNEXPECTED_TREE_STACK_DATA;
                return offset;

            case FAIL: // overall failure
                return arg < 0 ? arg : PARSE_FAILED;

            default:
                return UNKNOWN_INSTRUCTION;
        }
    }
    return RUNAWAY;
}

//
// ByteCode
//

ByteCode *ByteCode_new()
{
    ByteCode *self = (ByteCode *)malloc(sizeof(ByteCode));
    if (NULL == self) { return self; }
    self->num_defs = 0;
    self->def_names = NULL;
    self->def_flags = NULL;
    self->def_addrs = NULL;
    self->num_instructions = 0;
    self->instructions = NULL;
    self->num_strings = 0;
    self->strings = NULL;
    self->str_len = NULL;
    return self;
}

void ByteCode_free(ByteCode *self)
{
    int i;

    if (self->num_defs > 0) {
        for (i = 0; i < self->num_defs; ++i) {
            free(self->def_names[i]);
        }
        if (self->def_names) {
            free(self->def_names);
            self->def_names = NULL;
        }
        if (self->def_flags) {
            free(self->def_flags);
            self->def_flags = NULL;
        }
        if (self->def_addrs) {
            free(self->def_addrs);
            self->def_addrs = NULL;
        }
        self->num_defs = 0;
    }

    if (self->num_instructions > 0) {
        if (self->instructions) {
            free(self->instructions);
            self->instructions = NULL;
        }
        self->num_instructions = 0;
    }

    if (self->num_strings > 0) {
        for (i = 0; i < self->num_strings; ++i) {
            free(self->strings[i]);
        }
        free(self->strings);
        free(self->str_len);
        self->num_strings = 0;
    }
    free(self);
}


static inline const char *op_name(int op)
{
    return (op >= 0 && op < NUM_OPS) ? OpNames[op] : "N/A";
}

void ByteCode_print_instructions(ByteCode *self)
{
    for (int i = 0; i < self->num_instructions; ++i) {
        printf("INST %8d %12s %8d\n", i, op_name(self->instructions[i] & 0xff), self->instructions[i] >> 8);
    }
}

void ByteCode_print_defs(ByteCode *self)
{
    for (int i = 0; i < self->num_defs; ++i) {
        printf("DEF  %8d %12s %8d %6d\n", i, self->def_names[i], self->def_addrs[i], self->def_flags[i]);
    }
}

void ByteCode_print_strings(ByteCode *self)
{
    unsigned char *tmp;
    for (int i = 0; i < self->num_strings; ++i) {
        tmp = esc_string(self->strings[i], self->str_len[i], 256);
        printf("STR  %8d %8d \"%s\"\n", i, self->str_len[i], tmp);
        free(tmp);
    }
}

void ByteCode_print(ByteCode *self)
{
    ByteCode_print_defs(self);
    ByteCode_print_instructions(self);
    ByteCode_print_strings(self);
}

int ByteCode_compare(ByteCode *a, ByteCode *b)
{
    int i;

    /* Defs */

    if (a->num_defs != b->num_defs) {
        printf("a->num_defs != b->num_defs\n");
        return 1;
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (strcmp(a->def_names[i], b->def_names[i])) {
            printf("a->def_names[%d] = '%s'\n", i, a->def_names[i]);
            printf("b->def_names[%d] = '%s'\n", i, b->def_names[i]);
            return 2;
        }
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (a->def_flags[i] != b->def_flags[i]) {
            printf("a->def_flags[%d] = %d\n", i, a->def_flags[i]);
            printf("b->def_flags[%d] = %d\n", i, b->def_flags[i]);
            return 3;
        }
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (a->def_addrs[i] != b->def_addrs[i]) {
            printf("a->def_addrs[%d] = %d\n", i, a->def_addrs[i]);
            printf("b->def_addrs[%d] = %d\n", i, b->def_addrs[i]);
            return 4;
        }
    }

    /* Instructions */

    if (a->num_instructions != b->num_instructions) {
        printf("a->num_instructions != b->num_instructions\n");
        return 5;
    }
    for (i = 0; i < a->num_instructions; ++i) {
        if (a->instructions[i] != b->instructions[i]) {
            printf("a->instructions[%d] = %s %d\n", i,
                    op_name(a->instructions[i] & 0xff), a->instructions[i] >> 8);
            printf("b->instructions[%d] = %s %d\n", i,
                    op_name(b->instructions[i] & 0xff), b->instructions[i] >> 8);
            return 6;
        }
    }

    /* Strings */

    if (a->num_strings != b->num_strings) {
        printf("a->num_strings != b->num_strings\n");
        return 7;
    }

    for (i = 0; i < a->num_strings; ++i) {
        if (a->str_len[i] != b->str_len[i] || memcmp(a->strings[i], b->strings[i], a->str_len[i])) {
            unsigned char *tmp;
            tmp = esc_string(a->strings[i], a->str_len[i], 256);
            printf("a->strings[%d] = \"%s\" (len=%d)\n", i, tmp, a->str_len[i]);
            free(tmp);
            tmp = esc_string(b->strings[i], b->str_len[i], 256);
            printf("b->strings[%d] = \"%s\" (len=%d)\n", i, tmp, b->str_len[i]);
            free(tmp);
            return 8;
        }
    }

    return 0;
}
