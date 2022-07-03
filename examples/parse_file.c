#include <stdio.h>

#include "chpeg_all.h"

int main(int argc, char *argv[])
{
    unsigned char *input = NULL;
    size_t length = 0;
    ChpegByteCode *byte_code = NULL;
    ChpegParser *parser = NULL;
    int parse_result = 0;
    int ret = 0;
    char *grammar_filename = NULL;
    char *input_filename = NULL;

#ifdef DEBUG_MEM
    mtrace();
#endif

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: %s [<grammar>] <input>\n", argv[0]);
        ret = 1;
        goto done;
    }

    if (argc == 2) {
        input_filename = argv[1];
    }
    else if (argc == 3) {
        grammar_filename = argv[1];
        input_filename = argv[2];
    }

    // If grammar file is given, compile it to byte_code
    if (grammar_filename) {

        // Read the grammar file into input
        if (chpeg_read_file(grammar_filename, &input, &length) != 0) {
            fprintf(stderr, "Could not read grammar file: %s\n", grammar_filename);
            ret = 2;
            goto done;
        }

        // Compile the grammar file into byte_code
        parse_result = chpeg_compile(input, length, &byte_code, 1);
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

        CHPEG_FREE(input);
        input = NULL;
        length = 0;
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

#if CHPEG_PACKRAT
    // set to non-zero to enable packrat
    parser->packrat_enabled = 1;
#endif

    size_t consumed = 0;
    parse_result = ChpegParser_parse(parser, input, length, &consumed);
    if (parse_result == 0) {
        if (consumed == length) {
            printf("Parse successful.\n");
        }
        ChpegParser_print_tree(parser, input, stdout);
    }
    else {
        if (parse_result == CHPEG_ERR_EXTRANEOUS_INPUT) {
            printf("Extraneous input: parse consumed %lu bytes out of %lu\n", consumed, length);
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

    if (grammar_filename && byte_code) {
        ChpegByteCode_free(byte_code);
        byte_code = NULL;
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
