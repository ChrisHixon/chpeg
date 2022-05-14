#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

#ifdef DEBUG_MEM
    #include <mcheck.h>
#endif

#include "mem.h"
#include "util.h"
#include "parser.h"
#include "compiler.h"

int main(int argc, char *argv[])
{
    unsigned char *input = NULL;
    size_t length = 0;
    ByteCode *byte_code = NULL;
    Parser *parser = NULL;
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
        if (read_file(grammar_filename, &input, &length) != 0) {
            fprintf(stderr, "Could not read grammar file: %s\n", grammar_filename);
            ret = 2;
            goto done;
        }

        // Compile the grammar file into byte_code
        byte_code = Compiler_compile(input, length, &parse_result, 1);
        if (!byte_code) {
            fprintf(stderr, "Grammar file failed to compile. Parser returned: %d\n", parse_result);
            ret = 3;
            goto done;
        }
        else {
            fprintf(stderr, "Grammar file compiled successfully. Parser returned: %d\n", parse_result);
        }

        // uncomment to print a dump of the byte code (defs, instructions, and strings)
        //ByteCode_print(byte_code);

        CHPEG_FREE(input);
        input = NULL;
        length = 0;
    }
    // Otherwise, use default chpeg grammar
    else {
        fprintf(stderr, "Using default chpeg grammar\n");
    }

    // Read the file to parse into input
    if (read_file(input_filename, &input, &length) != 0) {
        fprintf(stderr, "Could not read file: %s\n", input_filename);
        ret = 4;
        goto done;
    }

    // Parse the data file using byte code
    parser = Parser_new(byte_code ? byte_code : Compiler_bytecode());

#if VM_TRACE
    // set to non-zero to enable VM instruction execution tracing
    parser->vm_trace = 0;
#endif

#if VM_PRINT_TREE
    // set to non-zero to print parse tree as it is being built
    parser->vm_print_tree = 0;
#endif

    size_t consumed = 0;
    parse_result = Parser_parse(parser, input, length, &consumed);
    if (parse_result == 0) {
        if (consumed == length) {
            printf("Parse successful.\n");
        }
        Parser_print_tree(parser, input);
    }
    else {
        if (parse_result == EXTRANEOUS_INPUT) {
            printf("Extraneous input: parse consumed %lu bytes out of %lu\n", consumed, length);
        }
        else {
            printf("Parse failed with result: %d\n", parse_result);
        }
        Parser_print_error(parser, input);
        ret = 5;
        goto done;
    }

    ret = 0;
done:

    if (grammar_filename && byte_code) {
        ByteCode_free(byte_code);
        byte_code = NULL;
    }
    if (input) {
        CHPEG_FREE(input);
        input = NULL;
    }
    if (parser) {
        Parser_free(parser);
        parser = NULL;
    }

#ifdef DEBUG_MEM
    muntrace();
#endif

    return ret;
}
