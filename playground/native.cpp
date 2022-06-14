extern "C" {
#define WITH_CHPEG_NOCASE2
#define CHPEG_CUSTOM_SHOW_MESSAGE
#ifdef WITH_CHPEG_NOCASE
    #define WITHOUT_MAIN
    #define CHPEG_PACKRAT 1
    #include "../examples/chpeg_nocase.c"
#else
    #define CHPEG_EXTENSION_ALL
    #define CHPEG_DEFINITION_TRACE
    //#include "../include/chpeg.h"
    #include "../include/chpeg_ext.h"
#endif
}
#include <cstdio>
#ifndef MAIN_CMD
#include <emscripten/bind.h>
#endif
#include <functional>
#include <iomanip>
#include <sstream>

// https://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c/33799784#33799784
std::string escape_json(const std::string &s) {
  std::ostringstream o;
  for (auto c : s) {
    if (c == '"' || c == '\\' || ('\x00' <= c && c <= '\x1f')) {
      o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
    } else {
      o << c;
    }
  }
  return o.str();
}
/*
std::function<void(size_t, size_t, const std::string &)>
makeJSONFormatter(std::string &json, bool &init) {
  init = true;
  return [&](size_t ln, size_t col, const std::string &msg) mutable {
    if (!init) { json += ","; }
    json += "{";
    json += R"("ln":)" + std::to_string(ln) + ",";
    json += R"("col":)" + std::to_string(col) + ",";
    json += R"("msg":")" + escape_json(msg) + R"(")";
    json += "}";

    init = false;
  };
}

bool parse_grammar(const std::string &text, peg::parser &peg,
                   std::string &json) {
  bool init;
  peg.log = makeJSONFormatter(json, init);
  json += "[";
  auto ret = peg.load_grammar(text.data(), text.size());
  json += "]";
  return ret;
}

bool parse_code(const std::string &text, peg::parser &peg, std::string &json,
                std::shared_ptr<peg::Ast> &ast) {
  peg.enable_ast();
  bool init;
  peg.log = makeJSONFormatter(json, init);
  json += "[";
  auto ret = peg.parse_n(text.data(), text.size(), ast);
  json += "]";
  return ret;
}
*/

CHPEG_API int chpeg_show_message(ChpegParserPtr parser, int msg_type, const char *fmt, ...) {

#define MSG_INPUT_STR "input:"
    if(parser->udata && strncmp(MSG_INPUT_STR, fmt, sizeof(MSG_INPUT_STR)-1) == 0) {
        std::string &json = *((std::string *)parser->udata);
        char buf[2048];
        
        va_list argp1, argp2;
        int rc;
        va_start(argp1, fmt);
        va_copy(argp2, argp1);
        rc = vsnprintf(buf, sizeof(buf), fmt, argp1);
        va_end(argp1);
        isz_t line = va_arg(argp2, isz_t);
        isz_t col = va_arg(argp2, isz_t);
        va_end(argp2);
        json += "{";
        json += R"("ln":)" + std::to_string(line) + ",";
        json += R"("col":)" + std::to_string(col) + ",";
        json += R"("msg":")" + escape_json(buf) + R"(")";
        json += "},";
        return rc;
    }
    return 0;
}

void ChpegNode_print(ChpegNode *self, ChpegParser *parser, const unsigned char *input, int depth, std::string &output)
{
    char buf[2048];
    char flags[CHPEG_FLAGS_DISPLAY_SIZE];
#if CHPEG_EXTENSION_TRIM
    char *data = chpeg_esc_bytes(&input[self->token_offset], self->token_length, 40);
#else
    char *data = chpeg_esc_bytes(&input[self->offset], self->length, 40);
#endif
    const char *def_name = ChpegByteCode_def_name(parser->bc, self->def);

    if (depth == 0) {
        snprintf(buf, sizeof(buf),
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
            "     "
#endif
            " OFFSET   LEN     ID NC FLG IDENT \"DATA\"\n");
        output += buf;
    }

    snprintf(buf, sizeof(buf),
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
        "%4d "
#endif
        "%6" ISZ_FMT2 " %6" ISZ_FMT2 " %6d %2d %3s %*s%s \"%s\"\n",
#if CHPEG_NODE_REF_COUNT && CHPEG_DEBUG_REFS
        self->ref_count,
#endif
#if CHPEG_EXTENSION_TRIM
        self->token_offset,
        self->token_length,
#else
        self->offset,
        self->length,
#endif
        self->def,
        self->num_children,
        chpeg_flags(flags, self->flags),
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : "<NULL>"
        );
    output += buf;

    if (data) { CHPEG_FREE(data); data = NULL; }
    for (ChpegNode *p = self->head; p; p = p->next) {
        ChpegNode_print(p, parser, input, depth + 1, output);
    }
}

CHPEG_API void ChpegByteCode_output_definition(const ChpegByteCode *bc, int def_id, std::string &output)
{
    char buf[2048];
    buf[0] = '\0';
    int nested_choice = 0;
    const char *def_flag = "";
    switch(bc->def_flags[def_id]){
        case CHPEG_FLAG_STOP: def_flag = "{S} "; break;
        case CHPEG_FLAG_IGNORE: def_flag = "{I} "; break;
        case CHPEG_FLAG_LEAF: def_flag = "{L} "; break;
    }
    snprintf(buf, sizeof(buf), "%s %s<- ", bc->def_names[def_id], def_flag); output += buf;
    for(int i = bc->def_addrs[def_id], inst_count = 0; i < bc->num_instructions; ++i, ++inst_count) {
        int op = CHPEG_INST_OP(bc->instructions[i]);
        int arg = CHPEG_INST_ARG(bc->instructions[i]);
        switch(op) {
            case CHPEG_OP_IDENT: snprintf(buf, sizeof(buf), "%s ", bc->def_names[arg]);  output += buf; break;
            case CHPEG_OP_LIT:
            case CHPEG_OP_LIT_NC: {
                char *str = chpeg_esc_bytes(bc->strings[arg], bc->str_len[arg], 0);
                snprintf(buf, sizeof(buf), "\"%s\"%s ", str, (op == CHPEG_OP_LIT_NC) ? "i" : ""); output += buf;
                if (str) CHPEG_FREE(str);
            }
            break;
            case CHPEG_OP_CHRCLS: {
                char *str = chpeg_esc_bytes(bc->strings[arg], bc->str_len[arg], 0);
                snprintf(buf, sizeof(buf), "[%s] ", str); output += buf;
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
				snprintf(buf, sizeof(buf), "( "); output += buf;
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
			snprintf(buf, sizeof(buf), ")"); output += buf;
                }

		switch(op) {
		    case CHPEG_OP_RPDONE: snprintf(buf, sizeof(buf), "+ "); output += buf; break;
		    case CHPEG_OP_RSDONE: snprintf(buf, sizeof(buf), "* "); output += buf; break;
		    case CHPEG_OP_RQDONE: snprintf(buf, sizeof(buf), "? "); output += buf; break;
			default:
				snprintf(buf, sizeof(buf), ":?OP?: "); output += buf; break;
		}
		break;

            case CHPEG_OP_PREDN:
            case CHPEG_OP_PREDA:
                snprintf(buf, sizeof(buf), "%s ", op == CHPEG_OP_PREDN ? "!" : "&"); output += buf;
                switch(CHPEG_INST_OP(bc->instructions[i+3])) {
			case CHPEG_OP_PMATCHF:
			case CHPEG_OP_PNOMATS:
				break;
			default: //Not at the end of predicate
				snprintf(buf, sizeof(buf), "( "); output += buf;
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
				snprintf(buf, sizeof(buf), ") "); output += buf;
			}
                }
                break;

            case CHPEG_OP_DOT: snprintf(buf, sizeof(buf), ". "); output += buf; break;

#if CHPEG_EXTENSION_TRIM
            case CHPEG_OP_TRIM: snprintf(buf, sizeof(buf), "< "); output += buf; break;
                break;
            case CHPEG_OP_TRIMS: snprintf(buf, sizeof(buf), "> "); output += buf; break;
#endif

#if CHPEG_EXTENSION_REFS
            case CHPEG_OP_MARK: snprintf(buf, sizeof(buf), "$%s< ", bc->refs[arg]); output += buf; break;
            case CHPEG_OP_MARKS: snprintf(buf, sizeof(buf), "> "); output += buf; break;
            case CHPEG_OP_REF: snprintf(buf, sizeof(buf), "$%s ", bc->refs[arg]); output += buf; break;
#endif

            case CHPEG_OP_CHOICE:
                    ++nested_choice;
                    snprintf(buf, sizeof(buf), "( "); output += buf; break;
                break;


            //case CHPEG_OP_CHOICE: fprintf(fp, "/ "); break;
            case CHPEG_OP_CISUCC:
                if(CHPEG_INST_OP(bc->instructions[i+2]) != CHPEG_OP_CFAIL) {//Not at the end of choice
                    snprintf(buf, sizeof(buf), "/ "); output += buf;
                } else {
                    --nested_choice;
                    snprintf(buf, sizeof(buf), ") "); output += buf; break;
                }
            break;
            case CHPEG_OP_ISUCC: i = bc->num_instructions; //stop processing
        }
    }
}

void ChpegParser_print_trace(ChpegParser *self, const unsigned char *input, size_t input_length, std::string &output)
{
#ifdef CHPEG_DEFINITION_TRACE
    char buf[2048];
    int itotal_def_count = 0, itotal_def_succ_count = 0, itotal_def_fail_count = 0;
    for(int i=0; i < self->bc->num_defs; ++i) {
        itotal_def_count += self->def_count[i];
        itotal_def_succ_count += self->def_succ_count[i];
        itotal_def_fail_count += self->def_fail_count[i];
    }
    double dtotal_count = itotal_def_count;
    snprintf(buf, sizeof(buf), "Total VM loops %d,  instructions per input byte %.2f\n\n", self->vm_count, (self->vm_count/(input_length*1.0)));
    output += buf;
    snprintf(buf, sizeof(buf), "%4s  %10s  %5s  %10s  %10s  %s\n", "id", "total", "%", "success", "fail", "definition");
    output += buf;
    snprintf(buf, sizeof(buf), "\n%4s  %10.d  %5s  %10.d  %10.d  Total counters", "", itotal_def_count, "", itotal_def_succ_count, itotal_def_fail_count);
    output += buf;
    snprintf(buf, sizeof(buf), "\n%4s  %12s  %5s  %8.2f  %10.2f  %% success/fail\n\n", "", "", "", (itotal_def_succ_count/dtotal_count)*100.0, (itotal_def_fail_count/dtotal_count)*100.0);
    output += buf;
    for(int i=0; i < self->bc->num_defs; ++i) {
        snprintf(buf, sizeof(buf), "%4.d  %10.d  %5.2f  %10.d  %10.d  ", i, self->def_count[i], (self->def_count[i]/dtotal_count)*100.0, self->def_succ_count[i], self->def_fail_count[i]);
        output += buf;
        ChpegByteCode_output_definition(self->bc, i, output);
        //snprintf(buf, sizeof(buf), "\n");
        output += "\n";
    }
#endif
}

static void addResult(std::string &json, std::string& result) {
    if(result.size() > 1) result.resize(result.size()-1); //erase last comma
    json += result + "]";
} 

std::string lint(const std::string &grammarText, const std::string &codeText, int opt_simplification, bool packrat) {
  ChpegParser *parser = NULL;
  int ret = 0;

  std::string grammarResult = "[";
  std::string codeResult = "[";
  std::string astResult;
  std::string profileResult;

  ChpegByteCode *byte_code = NULL;
  auto is_source_valid = false;
  auto is_grammar_valid = false;
  char log_grammar_buf[1024];
  char log_code_buf[1024];
  log_code_buf[0] = '[';
  log_code_buf[1] = ']';
  log_code_buf[2] = '\0';
  int parse_result;
  if(grammarText.size() > 0) {
      ChpegParser *gparser = ChpegParser_new(chpeg_default_bytecode());
      gparser->udata = &grammarResult;
      parse_result = chpeg_compile2((const unsigned char*)grammarText.c_str(), grammarText.size(), &byte_code, 0, gparser);
      is_grammar_valid = byte_code != NULL;
  }
  else {
      is_grammar_valid = true;
  }
  if (!is_grammar_valid) {
    snprintf(log_grammar_buf, sizeof(log_grammar_buf), "[\"Grammar file failed to compile. Parser returned: %d\"]", parse_result);
  }
  else {
    //snprintf(log_grammar_buf, sizeof(log_grammar_buf), "[\"Grammar file compiled successfully. Parser returned: %d\"]", parse_result);
    snprintf(log_grammar_buf, sizeof(log_grammar_buf), "[]");
    // Parse the data file with the compiled bytecode
    parser = ChpegParser_new(byte_code ? byte_code : chpeg_default_bytecode());
    parser->udata = &codeResult;
    parser->simplification = opt_simplification;
    if(packrat) {
#if CHPEG_PACKRAT
        parser->packrat = 1;
#endif
    }
    isz_t consumed;
    parse_result = ChpegParser_parse(parser, (const unsigned char*)codeText.c_str(), (int) codeText.size(), &consumed);
    if (consumed == codeText.size()) {
      snprintf(log_code_buf, sizeof(log_code_buf), "[]");
      is_source_valid = true;
      ChpegNode_print(parser->tree_root, parser, (const unsigned char*)codeText.c_str(), 0, astResult);
      ChpegParser_print_trace(parser, (const unsigned char*)codeText.c_str(), codeText.size(), profileResult);
    }
    else {
      snprintf(log_code_buf, sizeof(log_code_buf), "[\"parse succeeded but consumed %d bytes out of %d\"]", parse_result, (int)codeText.size());
      ChpegParser_print_error(parser, (const unsigned char*)codeText.c_str());
    }
    if(byte_code) ChpegByteCode_free(byte_code);
    ChpegParser_free(parser);
  }

  std::string json;
  json += "{";
  json += std::string("\"grammar_valid\":") + (is_grammar_valid ? "true" : "false");
  json += ",\"grammar\":"; addResult(json, grammarResult);
  json += std::string(",\"source_valid\":") + (is_source_valid ? "true" : "false");
  json += ",\"code\":"; addResult(json, codeResult);
  if (!astResult.empty()) {
    //json += ",\"ast\":\"" + astResult + "\"";
    json += ",\"ast\":\"" + escape_json(astResult) + "\"";
    json += ",\"profile\":\"" + escape_json(profileResult) + "\"";
  }
  json += "}";

  return json;
}

#ifndef MAIN_CMD
EMSCRIPTEN_BINDINGS(native) { emscripten::function("lint", &lint); }
#endif

#ifdef MAIN_CMD
void usage(const char *prog) {
    fprintf(stderr, "usage: %s [<grammar>] <input>\n", prog);
    fprintf(stderr, "   or: %s --cbytecode basename <grammar>\n", prog);
}

static int ud_show_message(void *udata, int error_type, isz_t line, isz_t col, isz_t offset,
        const char *fmt, ...) {
    fprintf(stderr, "message:" ISZ_FMT ":" ISZ_FMT " " ISZ_FMT " %d  %s\n", line, col, offset, error_type, fmt);
    return 0;
}

int main(int argc, char *argv[]) {
    unsigned char *input_grammar = NULL;
    isz_t length_grammar = 0;
    unsigned char *input = NULL;
    isz_t length = 0;
    ChpegByteCode *byte_code = NULL;
    ChpegParser *parser = NULL;
    int parse_result = 0;
    int ret = 0;
    char *grammar_filename = NULL;
    char *input_filename = NULL;
    char *base_filename = NULL;
    int gen_cbytecode = 0;
    isz_t consumed = 0;

#ifdef DEBUG_MEM
    mtrace();
#endif

    if (argc < 2 || argc > 4) {
        usage(argv[0]);
        ret = 1;
        goto done;
    }

    gen_cbytecode = strcmp(argv[1], "--cbytecode") == 0;

    if(gen_cbytecode) {
        if(argc != 4) {
            usage(argv[0]);
            ret = 1;
            goto done;
        }
        base_filename = argv[2];
        grammar_filename = argv[3];
    }
    else if (argc == 2) {
        input_filename = argv[1];
    }
    else if (argc == 3) {
        grammar_filename = argv[1];
        input_filename = argv[2];
    }

    // If grammar file is given, compile it to byte_code
    if (grammar_filename) {

        // Read the grammar file into input
        if (chpeg_read_file(grammar_filename, &input_grammar, &length_grammar) != 0) {
            fprintf(stderr, "Could not read grammar file: %s\n", grammar_filename);
            ret = 2;
            goto done;
        }

        // Compile the grammar file into byte_code
        parse_result = chpeg_compile(input_grammar, length_grammar, &byte_code, 1);
        if (parse_result != 0) {
            fprintf(stderr, "Grammar file failed to compile. Parser returned: %d\n", parse_result);
            ret = 3;
            goto done;
        }
        else {
            fprintf(stderr, "Grammar file compiled successfully. Parser returned: %d\n", parse_result);
        }

        // uncomment to print a dump of the byte code (defs, instructions, and strings)
        //ChpegByteCode_print(byte_code);
        if(gen_cbytecode) {
            FILE *fp;
            char strbuf[1024];
            snprintf(strbuf, sizeof(strbuf), "%s.c", base_filename);
            fp = fopen(strbuf, "w");
            if (!fp) {
                perror(strbuf);
                return 1;
            }
            ChpegByteCode_output_c(byte_code, fp, base_filename, NULL);
            fclose(fp);

            snprintf(strbuf, sizeof(strbuf), "%s.h", base_filename);
            fp = fopen(strbuf, "w");
            if (!fp) {
                perror(strbuf);
                return 1;
            }
            ChpegByteCode_output_h(byte_code, fp, base_filename, NULL, base_filename, NULL);
            fclose(fp);
            goto done;
        }

        CHPEG_FREE(input);
    }
    // Otherwise, use default chpeg grammar
    else {
        fprintf(stderr, "Using default chpeg grammar\n");
    }

    // Read the file to parse into input
    if (chpeg_read_file(input_filename, &input, &length) != 0) {
        fprintf(stderr, "Could not read file: %s\n", input_filename);
        ret = 4;
        goto done;
    }

    // Parse the data file using byte code
    parser = ChpegParser_new(byte_code ? byte_code : chpeg_default_bytecode());

#if CHPEG_VM_TRACE
    // set to non-zero to enable VM instruction execution tracing
    parser->vm_trace = 0;
#endif

#if CHPEG_VM_PRINT_TREE
    // set to non-zero to print parse tree as it is being built
    parser->vm_print_tree = 0;
#endif

    parse_result = ChpegParser_parse(parser, input, length, &consumed);
    if (parse_result == 0) {
        if (consumed == length) {
            printf("Parse successful.\n");
        }
        ChpegParser_print_tree(parser, input, length, stdout);
    }
    else {
        if (parse_result == CHPEG_ERR_EXTRANEOUS_INPUT) {
            printf("Extraneous input: parse consumed " ISZ_FMT " bytes out of " ISZ_FMT "\n", consumed, length);
        }
        else {
            printf("Parse failed with result: %d\n", parse_result);
        }
        ChpegParser_print_error(parser, input);
        ret = 5;
        goto done;
    }

    ret = 0;
done:
        
    if(/*ret == 0 &&*/ input_grammar && input) {
      std::string grammarText = (char*)input_grammar;
      std::string codeText = (char*)input;
      std::string json = lint(grammarText, codeText, 2, false);
      printf("\n%s\n", json.c_str());
    }

    if (grammar_filename && byte_code) {
        ChpegByteCode_free(byte_code);
        byte_code = NULL;
    }
    if (input_grammar) {
        CHPEG_FREE(input_grammar);
        input_grammar = NULL;
    }
    if (input) {
        CHPEG_FREE(input);
        input = NULL;
    }
    if (parser) {
        ChpegParser_free(parser);
        parser = NULL;
    }

#ifdef DEBUG_MEM
    muntrace();
#endif

    return ret;
}
#endif //MAIN_CMD
