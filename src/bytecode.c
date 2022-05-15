#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mem.h"
#include "util.h"
#include "bytecode.h"
#include "opcodes.h"

//
// ByteCode
//

ByteCode *ByteCode_new()
{
    ByteCode *self = (ByteCode *)CHPEG_MALLOC(sizeof(ByteCode));
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
            CHPEG_FREE(self->def_names[i]);
        }
        if (self->def_names) {
            CHPEG_FREE(self->def_names);
            self->def_names = NULL;
        }
        if (self->def_flags) {
            CHPEG_FREE(self->def_flags);
            self->def_flags = NULL;
        }
        if (self->def_addrs) {
            CHPEG_FREE(self->def_addrs);
            self->def_addrs = NULL;
        }
        self->num_defs = 0;
    }

    if (self->num_instructions > 0) {
        if (self->instructions) {
            CHPEG_FREE(self->instructions);
            self->instructions = NULL;
        }
        self->num_instructions = 0;
    }

    if (self->num_strings > 0) {
        for (i = 0; i < self->num_strings; ++i) {
            CHPEG_FREE(self->strings[i]);
        }
        CHPEG_FREE(self->strings);
        CHPEG_FREE(self->str_len);
        self->num_strings = 0;
    }
    CHPEG_FREE(self);
}

const char *ByteCode_def_name(const ByteCode *self, int index)
{
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return 0;
}

void ByteCode_print_instructions(const ByteCode *self)
{
    const char *arg_str = NULL;
    char *tmp = NULL;
    int op, arg;
    for (int i = 0; i < self->num_instructions; ++i) {
        op = self->instructions[i] & 0xff;
        arg = self->instructions[i] >> 8;
        switch (op) {
            case CHPEG_OP_IDENT:
            case CHPEG_OP_ISUCC:
                arg_str = ByteCode_def_name(self, arg);
                printf("CHPEG_INST %8d %12s %8d %s\n",
                    i, Chpeg_op_name(op), arg, arg_str ? arg_str : "<N/A>");
                break;
            case CHPEG_OP_LIT:
            case CHPEG_OP_CHRCLS:
                tmp = esc_bytes(
                    self->strings[arg], self->str_len[arg], 256);
                printf("CHPEG_INST %8d %12s %8d \"%s\"\n", i, Chpeg_op_name(op), arg,
                    tmp ? tmp : "<NULL>");
                if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
                break;
            default:
                arg_str = "";
                printf("CHPEG_INST %8d %12s %8d\n", i, Chpeg_op_name(op), arg);
        }
    }
}

void ByteCode_print_defs(const ByteCode *self)
{
    for (int i = 0; i < self->num_defs; ++i) {
        printf("DEF  %8d %12s %8d %6d\n", i, self->def_names[i], self->def_addrs[i], self->def_flags[i]);
    }
}

void ByteCode_print_strings(const ByteCode *self)
{
    char *tmp;
    for (int i = 0; i < self->num_strings; ++i) {
        tmp = esc_bytes(self->strings[i], self->str_len[i], 256);
        printf("STR  %8d %8d \"%s\"\n", i, self->str_len[i],
            tmp ? tmp : "<NULL>");
        if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
    }
}

void ByteCode_print(const ByteCode *self)
{
    ByteCode_print_defs(self);
    ByteCode_print_instructions(self);
    ByteCode_print_strings(self);
}

void ByteCode_output_h(const ByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes)
{
    int i, j, slen;

    // guard
    char *preproc[3] = { "#ifndef", "#define", "#endif // #ifndef" };
    for (j = 0; j < 2; j++) {
        fprintf(fp, "%s ", preproc[j]);
        if (prefix) {
            slen = strlen(prefix);
            for (i = 0; i < slen; i++) {
                fputc(toupper(prefix[i]), fp);
            }
            fputc('_', fp);
        }
        if (basename) {
            slen = strlen(basename);
            for (i = 0; i < slen; i++) {
                fputc(toupper(basename[i]), fp);
            }
        }
        fputc('\n', fp);
    }

    fprintf(fp, "\n#include \"bytecode.h\"\n");
    fprintf(fp, "#include \"%s.h\"\n\n", opcodes ? opcodes : "opcodes");

    // #define for each def name
    for (j = 0; j < self->num_defs; j++) {
        fprintf(fp, "#define ");
        if (prefix) {
            slen = strlen(prefix);
            for (i = 0; i < slen; i++) {
                fputc(toupper(prefix[i]), fp);
            }
            fputc('_', fp);
        }
        slen = strlen(self->def_names[j]);
        for (i = 0; i < slen; i++) {
            fputc(toupper(self->def_names[j][i]), fp);
        }
        fprintf(fp, " %d\n", j);
    }
    fputc('\n', fp);

    fprintf(fp, "extern ByteCode %s;\n\n", varname ? varname : basename);

    fprintf(fp, "%s ", preproc[2]);
    if (prefix) {
        slen = strlen(prefix);
        for (i = 0; i < slen; i++) {
            fputc(toupper(prefix[i]), fp);
        }
        fputc('_', fp);
    }
    if (basename) {
        slen = strlen(basename);
        for (i = 0; i < slen; i++) {
            fputc(toupper(basename[i]), fp);
        }
    }
    fputc('\n', fp);
}

void ByteCode_output_c(const ByteCode *self, FILE *fp, const char *basename, const char *varname)
{
    int i;
    char *str;
    char buf[256];

    fprintf(fp, "#include \"%s.h\"\n", basename);
    fprintf(fp, "\n");

    fprintf(fp, "ByteCode %s = {\n", varname ? varname : basename);
    fprintf(fp, "  .num_defs = %d,\n", self->num_defs);

    fprintf(fp, "  .def_names = (char*[%d]) {", self->num_defs);
    for (i = 0; i < self->num_defs; i++) {
        fprintf(fp, "%s\"%s\"", i ? ", " : "", self->def_names[i]);
    }
    fprintf(fp, "},\n");

    fprintf(fp, "  .def_flags = (int[%d]) {", self->num_defs);
    for (i = 0; i < self->num_defs; i++) {
        str = buf;
        if (self->def_flags[i] & 0x7) {
            if (self->def_flags[i] & STOP) {
                if (str != buf) *str++ = '|';
                strcpy(str, "STOP"); str += 4;
            }
            if (self->def_flags[i] & IGNORE) {
                if (str != buf) *str++ = '|';
                strcpy(str, "IGNORE"); str += 6;
            }
            if (self->def_flags[i] & LEAF) {
                if (str != buf) *str++ = '|';
                strcpy(str, "LEAF"); str += 4;
            }
            *str = '\0';
        }
        else {
            buf[0] = '0'; buf[1] = '\0';
        }
        fprintf(fp, "%s%s", i ? ", " : "", buf);
    }
    fprintf(fp, "},\n");

    fprintf(fp, "  .def_addrs = (int[%d]) {", self->num_defs);
    for (i = 0; i < self->num_defs; i++) {
        fprintf(fp, "%s%d", i ? ", " : "", self->def_addrs[i]);
    }
    fprintf(fp, "}, // presubtracted by 1\n");

    fprintf(fp, "  .num_instructions = %d,\n", self->num_instructions);

    fprintf(fp, "  .instructions = (int[%d]) {\n", self->num_instructions);
    const char *arg_str = NULL;
    int op, arg;
    for (int i = 0; i < self->num_instructions; i++) {
        op = self->instructions[i] & 0xff;
        arg = self->instructions[i] >> 8;
        switch (op) {
            case CHPEG_OP_IDENT:
            case CHPEG_OP_ISUCC:
                arg_str = ByteCode_def_name(self, arg);
                fprintf(fp, "  /* %5d */ CHPEG_INST(CHPEG_OP_%-12s, %8d), /* %s */\n",
                    i, Chpeg_op_name(op), arg, arg_str ? arg_str : "<N/A>");
                break;
            case CHPEG_OP_LIT:
            case CHPEG_OP_CHRCLS:
                str = esc_bytes(self->strings[arg], self->str_len[arg], 40);
                fprintf(fp, "  /* %5d */ CHPEG_INST(CHPEG_OP_%-12s, %8d), /* \"%s\" */\n",
                    i, Chpeg_op_name(op), arg, str ? str : "<NULL>");
                if (str) { CHPEG_FREE(str); str = NULL; }
                break;
            default:
                arg_str = "";
                fprintf(fp, "  /* %5d */ CHPEG_INST(CHPEG_OP_%-12s, %8d),\n",
                    i, Chpeg_op_name(op), arg);
        }
    }
    fprintf(fp, "  },\n");

    fprintf(fp, "  .num_strings = %d,\n", self->num_strings);

    fprintf(fp, "  .strings = (unsigned char**)(char*[%d]) {", self->num_strings);
    for (i = 0; i < self->num_strings; i++) {
        str = esc_bytes(self->strings[i], self->str_len[i], 0);
        fprintf(fp, "%s\"%s\"", i ? ", " : "", str);
        CHPEG_FREE(str);
    }
    fprintf(fp, "},\n");

    fprintf(fp, "  .str_len = (int[%d]) {", self->num_strings);
    for (i = 0; i < self->num_strings; i++) {
        fprintf(fp, "%s%d", i ? ", " : "", self->str_len[i]);
    }
    fprintf(fp, "}\n");

    fprintf(fp, "};\n");
}

int ByteCode_compare(const ByteCode *a, const ByteCode *b)
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
                    Chpeg_op_name(a->instructions[i] & 0xff), a->instructions[i] >> 8);
            printf("b->instructions[%d] = %s %d\n", i,
                    Chpeg_op_name(b->instructions[i] & 0xff), b->instructions[i] >> 8);
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
            char *tmp;
            tmp = esc_bytes(a->strings[i], a->str_len[i], 256);
            printf("a->strings[%d] = \"%s\" (len=%d)\n", i,
                tmp ? tmp : "<NULL>", a->str_len[i]);
            if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
            tmp = esc_bytes(b->strings[i], b->str_len[i], 256);
            printf("b->strings[%d] = \"%s\" (len=%d)\n", i,
                tmp ? tmp : "<NULL>", b->str_len[i]);
            if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
            return 8;
        }
    }

    return 0;
}
