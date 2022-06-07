//
// chpeg: bytecode.c {
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef CHPEG_AMALGAMATION
#include "chpeg/util.h"
#include "chpeg/bytecode.h"
#include "chpeg/opcodes.h"
#endif

//
// ByteCode
//

CHPEG_API ChpegByteCode *ChpegByteCode_new()
{
    ChpegByteCode *self = (ChpegByteCode *)CHPEG_CALLOC(1, sizeof(ChpegByteCode));
    return self;
}

CHPEG_API void ChpegByteCode_free(ChpegByteCode *self)
{
    if (!self) return;

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
        self->strings = NULL;
        CHPEG_FREE(self->str_len);
        self->str_len = NULL;
        self->num_strings = 0;
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

CHPEG_API const char *ChpegByteCode_def_name(const ChpegByteCode *self, int index)
{
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return NULL;
}

#ifdef CHPEG_EXTENSION_REFS
CHPEG_API const char *ChpegByteCode_ref_name(const ChpegByteCode *self, int index)
{
    if (index >= 0 && index < self->num_refs) {
        return self->refs[index];
    }
    return NULL;
}
#endif

CHPEG_API void ChpegByteCode_print_instructions(const ChpegByteCode *self)
{
    const char *arg_str = NULL;
    char *tmp = NULL;
    int op, arg;
    printf("NUM_INST %4d\n", self->num_instructions);
    for (int i = 0; i < self->num_instructions; ++i) {
        op = CHPEG_INST_OP(self->instructions[i]);
        arg = CHPEG_INST_ARG(self->instructions[i]);
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

CHPEG_API void ChpegByteCode_print_defs(const ChpegByteCode *self)
{
    printf("NUM_DEFS %4d\n", self->num_defs);
    for (int i = 0; i < self->num_defs; ++i) {
        printf("DEF  %8d %12s %8d %6d\n", i, self->def_names[i], self->def_addrs[i], self->def_flags[i]);
    }
}

#if CHPEG_EXTENSION_REFS
CHPEG_API void ChpegByteCode_print_refs(const ChpegByteCode *self)
{
    printf("NUM_REFS %4d\n", self->num_refs);
    for (int i = 0; i < self->num_refs; ++i) {
        printf("REF  %8d %s\n", i, self->refs[i]);
    }
}
#endif

CHPEG_API void ChpegByteCode_print_strings(const ChpegByteCode *self)
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

CHPEG_API void ChpegByteCode_print(const ChpegByteCode *self)
{
    ChpegByteCode_print_defs(self);
    ChpegByteCode_print_instructions(self);
    ChpegByteCode_print_strings(self);
#if CHPEG_EXTENSION_REFS
    ChpegByteCode_print_refs(self);
#endif
}

CHPEG_API void ChpegByteCode_output_h(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname, const char *prefix, const char *opcodes)
{
    int i, j, slen;

    if (prefix == NULL) {
        prefix = "chpeg_bc";
    }

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
        fprintf(fp, "_H\n");
    }

    fprintf(fp, "\n#ifndef CHPEG_AMALGAMATION\n");
    fprintf(fp, "#include \"chpeg/util.h\"\n");
    fprintf(fp, "#include \"chpeg/bytecode.h\"\n");
    fprintf(fp, "#include \"%s\"\n", opcodes ? opcodes : "chpeg/opcodes.h");
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
        slen = strlen(self->def_names[j]);
        for (i = 0; i < slen; i++) {
            fputc(toupper(self->def_names[j][i]), fp);
        }
        fprintf(fp, " %d\n", j);
    }
    fputc('\n', fp);

    fprintf(fp, "CHPEG_API const ChpegByteCode %s;\n\n", varname ? varname : basename);

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

CHPEG_API void ChpegByteCode_output_c(const ChpegByteCode *self, FILE *fp,
    const char *basename, const char *varname)
{
    int i;
    char *str;

    fprintf(fp, "#ifndef CHPEG_AMALGAMATION\n#include \"%s.h\"\n#endif\n", basename);
    fprintf(fp, "\n");

    fprintf(fp, "CHPEG_DEF const ChpegByteCode %s = {\n", varname ? varname : basename);

    fprintf(fp, "  .num_defs = %d,\n", self->num_defs);

    if (self->num_defs > 0) {
        fprintf(fp, "  .def_names = (char*[%d]) {", self->num_defs);
        for (i = 0; i < self->num_defs; i++) {
            fprintf(fp, "%s\"%s\"", i ? ", " : "", self->def_names[i]);
        }
        fprintf(fp, "},\n");

        fprintf(fp, "  .def_flags = (int[%d]) {", self->num_defs);
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
        fprintf(fp, "},\n");

        fprintf(fp, "  .def_addrs = (int[%d]) {", self->num_defs);
        for (i = 0; i < self->num_defs; i++) {
            fprintf(fp, "%s%d", i ? ", " : "", self->def_addrs[i]);
        }
        fprintf(fp, "},\n");
    }

    fprintf(fp, "  .num_instructions = %d,\n", self->num_instructions);

    if (self->num_instructions > 0) {
        fprintf(fp, "  .instructions = (int[%d]) {\n", self->num_instructions);
        const char *arg_str = NULL;
        int op, arg;
        for (int i = 0; i < self->num_instructions; i++) {
            op = CHPEG_INST_OP(self->instructions[i]);
            arg = CHPEG_INST_ARG(self->instructions[i]);
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
        fprintf(fp, "  },\n");
    }

    fprintf(fp, "  .num_strings = %d,\n", self->num_strings);

    if (self->num_strings > 0) {
        fprintf(fp, "  .strings = (unsigned char**)(char*[%d]) {", self->num_strings);
        for (i = 0; i < self->num_strings; i++) {
            str = chpeg_esc_bytes(self->strings[i], self->str_len[i], 0);
            fprintf(fp, "%s\"%s\"", i ? ", " : "", str);
            CHPEG_FREE(str);
        }
        fprintf(fp, "},\n");

        fprintf(fp, "  .str_len = (int[%d]) {", self->num_strings);
        for (i = 0; i < self->num_strings; i++) {
            fprintf(fp, "%s%d", i ? ", " : "", self->str_len[i]);
        }
        fprintf(fp, "},\n");
    }

#if CHPEG_EXTENSION_REFS
    if (self->num_refs > 0) {
        fprintf(fp, "#if CHPEG_EXTENSION_REFS\n");
        fprintf(fp, "  .num_refs = %d,\n", self->num_refs);

        fprintf(fp, "  .refs = (char*[%d]) {", self->num_refs);
        for (i = 0; i < self->num_refs; i++) {
            fprintf(fp, "%s\"%s\"", i ? ", " : "", self->refs[i]);
        }
        fprintf(fp, "},\n");
        fprintf(fp, "#endif\n");
    }
#endif

    fprintf(fp, "};\n");
}

CHPEG_API int ChpegByteCode_compare(const ChpegByteCode *a, const ChpegByteCode *b)
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
                    Chpeg_op_name(CHPEG_INST_OP(a->instructions[i])), CHPEG_INST_ARG(a->instructions[i]));
            printf("b->instructions[%d] = %s %d\n", i,
                    Chpeg_op_name(CHPEG_INST_OP(b->instructions[i])), CHPEG_INST_ARG(b->instructions[i]));
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

CHPEG_API void ChpegByteCode_output_definition(const ChpegByteCode *bc, int def_id, FILE *fp)
{
    int nested_choice = 0;
    const char *def_flag = "";
    switch(bc->def_flags[def_id]){
        case CHPEG_FLAG_STOP: def_flag = "{S} "; break;
        case CHPEG_FLAG_IGNORE: def_flag = "{I} "; break;
        case CHPEG_FLAG_LEAF: def_flag = "{L} "; break;
    }
    fprintf(fp, "%s %s<- ", bc->def_names[def_id], def_flag);
    for(int i = bc->def_addrs[def_id], inst_count = 0; i < bc->num_instructions; ++i, ++inst_count) {
        int op = CHPEG_INST_OP(bc->instructions[i]);
        int arg = CHPEG_INST_ARG(bc->instructions[i]);
        switch(op) {
            case CHPEG_OP_IDENT: fprintf(fp, "%s ", bc->def_names[arg]); break;
            case CHPEG_OP_LIT:
            case CHPEG_OP_LIT_NC: {
                char *str = chpeg_esc_bytes(bc->strings[arg], bc->str_len[arg], 0);
                fprintf(fp, "\"%s\"%s ", str, (op == CHPEG_OP_LIT_NC) ? "i" : "");
                if (str) CHPEG_FREE(str);
            }
            break;
            case CHPEG_OP_CHRCLS: {
                char *str = chpeg_esc_bytes(bc->strings[arg], bc->str_len[arg], 0);
                fprintf(fp, "[%s] ", str);
                if (str) CHPEG_FREE(str);
            }
            break;

            case CHPEG_OP_RPBEG:
            case CHPEG_OP_RQBEG:
            case CHPEG_OP_RSBEG:
                switch(CHPEG_INST_OP(bc->instructions[i+4])) {
			case CHPEG_OP_RPDONE:
			case CHPEG_OP_RSDONE:
			case CHPEG_OP_RQDONE:
				break;
			default: //Not at the end of predicate
				fprintf(fp, "( ");
		}
                break;

            case CHPEG_OP_RPDONE:
            case CHPEG_OP_RSDONE:
            case CHPEG_OP_RQDONE:
                switch(CHPEG_INST_OP(bc->instructions[i-4])) {
                    case CHPEG_OP_RPBEG:
                    case CHPEG_OP_RQBEG:
                    case CHPEG_OP_RSBEG:
                        break;
                    default:
			fprintf(fp, ")");
                }

		switch(op) {
		    case CHPEG_OP_RPDONE: fprintf(fp, "+ "); break;
		    case CHPEG_OP_RSDONE: fprintf(fp, "* "); break;
		    case CHPEG_OP_RQDONE: fprintf(fp, "? "); break;
			default:
				fprintf(fp, ":?OP?: "); break;
		}
		break;

            case CHPEG_OP_PREDN:
            case CHPEG_OP_PREDA:
                fprintf(fp, "%s ", op == CHPEG_OP_PREDN ? "!" : "&");
                switch(CHPEG_INST_OP(bc->instructions[i+3])) {
			case CHPEG_OP_PMATCHF:
			case CHPEG_OP_PNOMATS:
				break;
			default: //Not at the end of predicate
				fprintf(fp, "( ");
		}
                break;
            case CHPEG_OP_PNOMATS:
                switch(CHPEG_INST_OP(bc->instructions[i-4])) {
                    case CHPEG_OP_PREDN:
                    case CHPEG_OP_PREDA:
                        break;
                    default:
			switch(CHPEG_INST_OP(bc->instructions[i-3])) {
			    case CHPEG_OP_PREDN:
			    case CHPEG_OP_PREDA:
				break;
			    default:
				fprintf(fp, ") ");
			}
                }
                break;

            case CHPEG_OP_DOT: fprintf(fp, ". "); break;

#ifdef CHPEG_EXTENSION_TRIM
            case CHPEG_OP_TRIM: fprintf(fp, "< "); break;
                break;
            case CHPEG_OP_TRIMS: fprintf(fp, "> "); break;
#endif

#ifdef CHPEG_EXTENSION_REFS
            case CHPEG_OP_RSCOPE: fprintf(fp, "$< "); break;
            case CHPEG_OP_MARK: fprintf(fp, "$%s< ", bc->refs[arg]); break;
            case CHPEG_OP_RSCOPES:
            case CHPEG_OP_MARKS: fprintf(fp, "> "); break;
            case CHPEG_OP_REF: fprintf(fp, "$%s ", bc->refs[arg]); break;
#endif

            case CHPEG_OP_CHOICE:
                    ++nested_choice;
                    fprintf(fp, "( "); break;
                break;


            //case CHPEG_OP_CHOICE: fprintf(fp, "/ "); break;
            case CHPEG_OP_CISUCC:
                if(CHPEG_INST_OP(bc->instructions[i+2]) != CHPEG_OP_CFAIL) //Not at the end of choice
                    fprintf(fp, "/ ");
                else {
                    --nested_choice;
                    fprintf(fp, ") "); break;
                }
            break;
            case CHPEG_OP_ISUCC: i = bc->num_instructions; //stop processing
        }
    }
}

// } chpeg: bytecode.c
