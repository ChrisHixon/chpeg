//
// chpeg: bytecode.c {
//

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/mem.h"
#include "chpeg/util.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

//
// ByteCode
//

ChpegByteCode *ChpegByteCode_new(void)
{
    ChpegByteCode *self = (ChpegByteCode *)CHPEG_CALLOC(1, sizeof(ChpegByteCode));
    return self;
}

void ChpegByteCode_free(ChpegByteCode *self)
{
    if (!self) return;

    int i;

    if (self->def_names && self->num_defs > 0) {
        for (i = 0; i < self->num_defs; ++i) {
            CHPEG_FREE(self->def_names[i]);
        }
    }
    self->num_defs = 0;
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

    if (self->instructions) {
        CHPEG_FREE(self->instructions);
        self->instructions = NULL;
    }
    self->num_instructions = 0;

    if (self->strings && self->num_strings > 0) {
        for (i = 0; i < self->num_strings; ++i) {
            CHPEG_FREE(self->strings[i]);
        }
    }
    self->num_strings = 0;
    if (self->strings) {
        CHPEG_FREE(self->strings);
        self->strings = NULL;
    }
    if (self->str_len) {
        CHPEG_FREE(self->str_len);
        self->str_len = NULL;
    }

#if CHPEG_EXTENSION_REFS
    if (self->num_refs > 0) {
        for (i = 0; i < self->num_refs; ++i) {
            CHPEG_FREE(self->refs[i]);
        }
        CHPEG_FREE(self->refs);
        self->refs = NULL;
        self->num_refs = 0;
    }
#endif

    CHPEG_FREE(self);
}

const char *ChpegByteCode_def_name(const ChpegByteCode *self, int index)
{
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return NULL;
}

#if CHPEG_EXTENSION_REFS
const char *ChpegByteCode_ref_name(const ChpegByteCode *self, int index)
{
    if (index >= 0 && index < self->num_refs) {
        return self->refs[index];
    }
    return NULL;
}
#endif

void ChpegByteCode_print_instructions(const ChpegByteCode *self)
{
    const char *arg_str = NULL;
    char *tmp = NULL;
    int op, arg;
    printf("NUM_INST %4d\n", self->num_instructions);
    for (int i = 0; i < self->num_instructions; ++i) {
        op = self->instructions[i] & 0xff;
        arg = self->instructions[i] >> 8;
        switch (op) {
            case CHPEG_OP_IDENT:
            case CHPEG_OP_ISUCC:
            case CHPEG_OP_IFAIL:
#if CHPEG_EXTENSION_TRIM
            case CHPEG_OP_TRIM:
#endif
                arg_str = ChpegByteCode_def_name(self, arg);
                printf("INST %8d %12s %8d %s\n",
                    i, Chpeg_op_name(op), arg, arg_str ? arg_str : "<NULL>");
                break;
#if CHPEG_EXTENSION_REFS
            case CHPEG_OP_MARK:
            case CHPEG_OP_REF:
                arg_str = ChpegByteCode_ref_name(self, arg);
                printf("INST %8d %12s %8d %s\n",
                    i, Chpeg_op_name(op), arg, arg_str ? arg_str : "<NULL>");
                break;
#endif
            case CHPEG_OP_LIT:
            case CHPEG_OP_CHRCLS:
                tmp = chpeg_esc_bytes(
                    self->strings[arg], self->str_len[arg], 256);
                printf("INST %8d %12s %8d \"%s\"\n", i, Chpeg_op_name(op), arg,
                    tmp ? tmp : "<NULL>");
                if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
                break;
            default:
                printf("INST %8d %12s %8d\n", i, Chpeg_op_name(op), arg);
        }
    }
}

void ChpegByteCode_print_defs(const ChpegByteCode *self)
{
    printf("NUM_DEFS %4d\n", self->num_defs);
    for (int i = 0; i < self->num_defs; ++i) {
        printf("DEF  %8d %12s %8d %6d\n", i, self->def_names[i], self->def_addrs[i], self->def_flags[i]);
    }
}

#if CHPEG_EXTENSION_REFS
void ChpegByteCode_print_refs(const ChpegByteCode *self)
{
    printf("NUM_REFS %4d\n", self->num_refs);
    for (int i = 0; i < self->num_refs; ++i) {
        printf("REF  %8d %s\n", i, self->refs[i]);
    }
}
#endif

void ChpegByteCode_print_strings(const ChpegByteCode *self)
{
    char *tmp;
    printf("NUM_STR %5d\n", self->num_strings);
    for (int i = 0; i < self->num_strings; ++i) {
        tmp = chpeg_esc_bytes(self->strings[i], self->str_len[i], 256);
        printf("STR  %8d %8d \"%s\"\n", i, self->str_len[i],
            tmp ? tmp : "<NULL>");
        if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
    }
}

void ChpegByteCode_print(const ChpegByteCode *self)
{
    ChpegByteCode_print_defs(self);
    ChpegByteCode_print_instructions(self);
    ChpegByteCode_print_strings(self);
#if CHPEG_EXTENSION_REFS
    ChpegByteCode_print_refs(self);
#endif
}

void ChpegByteCode_output_h(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes)
{
    int i, j, slen;

    // varname, prefix, and opcodes are optional (set to NULL to omit)

    varname = varname ? varname : basename;

    if (prefix == NULL) {
        prefix = "chpeg";
    }

    opcodes = opcodes ? opcodes : "chpeg/opcodes.h";

    // guard
    const char *preproc[3] = { "#ifndef", "#define", "#endif // #ifndef" };
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
        fprintf(fp, "_H\n");
    }

    fprintf(fp, "\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n");

    fprintf(fp, "#ifndef CHPEG_AMALGAMATION\n");
    fprintf(fp, "#include \"chpeg/chpeg_api.h\"\n");
    fprintf(fp, "#include \"chpeg/bytecode.h\"\n");
    fprintf(fp, "#include \"%s\"\n", opcodes);
    fprintf(fp, "#endif\n\n");

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
        fprintf(fp, "DEF_");
        slen = strlen(self->def_names[j]);
        for (i = 0; i < slen; i++) {
            fputc(toupper(self->def_names[j][i]), fp);
        }
        fprintf(fp, " %d\n", j);
    }
    fputc('\n', fp);

    fprintf(fp, "extern const ChpegByteCode %s;\n\n", varname);

    fprintf(fp, "#ifdef __cplusplus\n}\n#endif\n\n");

    // guard: end
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
    fprintf(fp, "_H\n");
}

void ChpegByteCode_output_c(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes)
{
    int i, j, slen;
    char *str;

    // varname, prefix, and opcodes are optional (set to NULL to omit)

    varname = varname ? varname : basename;

    if (prefix == NULL) {
        prefix = "chpeg";
    }

    opcodes = opcodes ? opcodes : "chpeg/opcodes.h";

    // guard (this may seem unusual for a .c file, but this is useful in amalgamation
    const char *preproc[3] = { "#ifndef", "#define", "#endif // #ifndef" };
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
        fprintf(fp, "_C\n");
    }

    fprintf(fp, "\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n");

    fprintf(fp, "#ifndef CHPEG_AMALGAMATION\n#include \"%s.h\"\n#endif\n", basename);
    fprintf(fp, "\n");

    if (self->num_defs > 0) {
        fprintf(fp, "const char *%s_def_names[%d] = {", varname, self->num_defs);
        for (i = 0; i < self->num_defs; i++) {
            fprintf(fp, "%s\"%s\"", i ? ", " : "", self->def_names[i]);
        }
        fprintf(fp, "};\n\n");

        fprintf(fp, "int %s_def_flags[%d] = {", varname, self->num_defs);
        for (i = 0; i < self->num_defs; i++) {
            fprintf(fp, "%s", i ? ", " : "");
            int flag_out = 0;
            if (self->def_flags[i] & 0x7) {
                if (self->def_flags[i] & CHPEG_FLAG_STOP) {
                    fprintf(fp, "%sCHPEG_FLAG_STOP", flag_out ? " | " : "");
                    flag_out = 1;
                }
                if (self->def_flags[i] & CHPEG_FLAG_IGNORE) {
                    fprintf(fp, "%sCHPEG_FLAG_IGNORE", flag_out ? " | " : "");
                    flag_out = 1;
                }
                if (self->def_flags[i] & CHPEG_FLAG_LEAF) {
                    fprintf(fp, "%sCHPEG_FLAG_LEAF", flag_out ? " | " : "");
                    flag_out = 1;
                }
            }
            else {
                fprintf(fp, "0");
            }
        }
        fprintf(fp, "};\n\n");

        fprintf(fp, "int %s_def_addrs[%d] = {", varname, self->num_defs);
        for (i = 0; i < self->num_defs; i++) {
            fprintf(fp, "%s%d", i ? ", " : "", self->def_addrs[i]);
        }
        fprintf(fp, "};\n\n");
    }

    if (self->num_instructions > 0) {
        fprintf(fp, "int %s_instructions[%d] = {\n", varname, self->num_instructions);
        const char *arg_str = NULL;
        int op, arg;
        for (int i = 0; i < self->num_instructions; i++) {
            op = self->instructions[i] & 0xff;
            arg = self->instructions[i] >> 8;
            switch (op) {
                case CHPEG_OP_IDENT:
                case CHPEG_OP_ISUCC:
                case CHPEG_OP_IFAIL:
#if CHPEG_EXTENSION_TRIM
                case CHPEG_OP_TRIM:
#endif
                    arg_str = ChpegByteCode_def_name(self, arg);
                    fprintf(fp, "  /* %6d */ CHPEG_INST(CHPEG_OP_%s,%*s %8d), // %s\n",
                        i, Chpeg_op_name(op), 8-(int)strlen(Chpeg_op_name(op)), "",
                        arg, arg_str ? arg_str : "<NULL>");
                    break;
#if CHPEG_EXTENSION_REFS
                case CHPEG_OP_MARK:
                case CHPEG_OP_REF:
                    arg_str = ChpegByteCode_ref_name(self, arg);
                    fprintf(fp, "  /* %6d */ CHPEG_INST(CHPEG_OP_%s,%*s %8d), // $%s\n",
                        i, Chpeg_op_name(op), 8-(int)strlen(Chpeg_op_name(op)), "",
                        arg, arg_str ? arg_str : "<NULL>");
                    break;
#endif
                case CHPEG_OP_LIT:
                    str = chpeg_esc_bytes(self->strings[arg], self->str_len[arg], 40);
                    fprintf(fp, "  /* %6d */ CHPEG_INST(CHPEG_OP_%s,%*s %8d), // \"%s\"\n",
                        i, Chpeg_op_name(op), 8-(int)strlen(Chpeg_op_name(op)), "",
                        arg, str ? str : "<NULL>");
                    if (str) { CHPEG_FREE(str); str = NULL; }
                    break;
                case CHPEG_OP_CHRCLS:
                    str = chpeg_esc_bytes(self->strings[arg], self->str_len[arg], 40);
                    fprintf(fp, "  /* %6d */ CHPEG_INST(CHPEG_OP_%s,%*s %8d), // [%s]\n",
                        i, Chpeg_op_name(op), 8-(int)strlen(Chpeg_op_name(op)), "",
                        arg, str ? str : "<NULL>");
                    if (str) { CHPEG_FREE(str); str = NULL; }
                    break;
                default:
                    arg_str = "";
                    fprintf(fp, "  /* %6d */ CHPEG_INST(CHPEG_OP_%s,%*s %8d),\n",
                        i, Chpeg_op_name(op), 8-(int)strlen(Chpeg_op_name(op)), "",
                        arg);
            }
        }
        fprintf(fp, "  };\n\n");
    }

    if (self->num_strings > 0) {
        fprintf(fp, "const char *%s_strings[%d] = {", varname, self->num_strings);
        for (i = 0; i < self->num_strings; i++) {
            str = chpeg_esc_bytes(self->strings[i], self->str_len[i], 0);
            fprintf(fp, "%s\"%s\"", i ? ", " : "", str);
            CHPEG_FREE(str);
        }
        fprintf(fp, "};\n\n");

        fprintf(fp, "int %s_str_len[%d] = {", varname, self->num_strings);
        for (i = 0; i < self->num_strings; i++) {
            fprintf(fp, "%s%d", i ? ", " : "", self->str_len[i]);
        }
        fprintf(fp, "};\n\n");
    }

#if CHPEG_EXTENSION_REFS
    if (self->num_refs > 0) {
        fprintf(fp, "#if CHPEG_EXTENSION_REFS\n");
        fprintf(fp, "const char *%s_refs[%d] = {", varname, self->num_refs);
        for (i = 0; i < self->num_refs; i++) {
            fprintf(fp, "%s\"%s\"", i ? ", " : "", self->refs[i]);
        }
        fprintf(fp, "};\n");
        fprintf(fp, "#endif\n\n");
    }
#endif

    fprintf(fp, "const ChpegByteCode %s = {\n", varname);

    fprintf(fp, "  %d, // num_defs\n", self->num_defs);
    if (self->num_defs > 0) {
        fprintf(fp, "  (char **)%s_def_names,\n", varname);
        fprintf(fp, "  %s_def_flags,\n", varname);
        fprintf(fp, "  %s_def_addrs,\n", varname);
    }
    else {
        fprintf(fp, "  NULL, // def_names\n");
        fprintf(fp, "  NULL, // def_flags,\n");
        fprintf(fp, "  NULL, // def_addrs,\n");
    }

    fprintf(fp, "  %d, // num_instructions\n", self->num_instructions);
    if (self->num_instructions > 0) {
        fprintf(fp, "  %s_instructions,\n", varname);
    }
    else {
        fprintf(fp, "  NULL, // instructions,\n");
    }

    fprintf(fp, "  %d, // num_strings\n", self->num_strings);
    if (self->num_strings > 0) {
        fprintf(fp, "  (unsigned char **)%s_strings,\n", varname);
        fprintf(fp, "  %s_str_len,\n", varname);
    }
    else {
        fprintf(fp, "  NULL, // strings,\n");
        fprintf(fp, "  NULL, // str_len,\n");
    }

#if CHPEG_EXTENSION_REFS
    fprintf(fp, "#if CHPEG_EXTENSION_REFS\n");
    fprintf(fp, "  %d, // num_refs\n", self->num_refs);
    if (self->num_refs > 0) {
        fprintf(fp, "  (char **)%s_refs,\n", varname);
    }
    else {
        fprintf(fp, "  NULL, // refs,\n");
    }
    fprintf(fp, "#endif\n");
#endif

    fprintf(fp, "};\n");

    fprintf(fp, "\n#ifdef __cplusplus\n}\n#endif\n\n");

    // guard: end
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
    fprintf(fp, "_C\n");
}

int ChpegByteCode_compare(const ChpegByteCode *a, const ChpegByteCode *b)
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
            tmp = chpeg_esc_bytes(a->strings[i], a->str_len[i], 256);
            printf("a->strings[%d] = \"%s\" (len=%d)\n", i,
                tmp ? tmp : "<NULL>", a->str_len[i]);
            if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
            tmp = chpeg_esc_bytes(b->strings[i], b->str_len[i], 256);
            printf("b->strings[%d] = \"%s\" (len=%d)\n", i,
                tmp ? tmp : "<NULL>", b->str_len[i]);
            if (tmp) { CHPEG_FREE(tmp); tmp = NULL; }
            return 8;
        }
    }

    return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: bytecode.c

