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

#include "parser.h"
#include "compiler.h"

#define READ_FILE_BUF_SIZE 1024
char *read_file(char *filename, int *length_return)
{
    char buf[READ_FILE_BUF_SIZE];
    char *ret = NULL, *input = NULL, *p = NULL;
    int fd = -1;
    struct stat st;
    off_t length = 0, remain = 0;
    ssize_t bytes_read = 0;

    if (length_return) { *length_return = -1; }

    if (stat(filename, &st) != 0) {
        perror(filename);
        goto done;
    }

    length = st.st_size;
    if (length > INT_MAX) {
        fprintf(stderr, "%s: file size too large\n", filename);
        goto done;
    }

    input = (char *)malloc((size_t)length);
    if (!input) {
        fprintf(stderr, "%s: cannot allocate memory for file\n", filename);
        goto done;
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror(filename);
        goto done;
    }

    p = input;
    remain = length;
    while (remain > 0) {
        bytes_read = read(fd, buf, READ_FILE_BUF_SIZE);
        if (bytes_read < 0) {
            perror(filename);
            goto done;
        }
        else if (bytes_read == 0) {
            break;
        }
        memcpy(p, buf, bytes_read);
        p += bytes_read;
        remain -= bytes_read;
    }

    if (remain > 0) {
        fprintf(stderr, "could not read all of file '%s' into memory\n", filename);
        goto done;
    }

    // success
    ret = input;
    input = NULL;
    close(fd);
    fd = -1;

done:
    if (input) {
        free(input);
        input = NULL;
    }

    if (fd >= 0) {
        close(fd);
        fd = -1;
    }

    if (length_return) { *length_return = length; }
    return ret;
}

int main(int argc, char *argv[])
{
    unsigned char *input = NULL;
    int length = -1;
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
        input = (unsigned char *)read_file(grammar_filename, &length);
        if (!input) {
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

        free(input);
        input = NULL;
        length = -1;
    }
    // Otherwise, use default chpeg grammar
    else {
        fprintf(stderr, "Using default chpeg grammar\n");
    }

    // Read the file to parse into input
    input = (unsigned char *)read_file(input_filename, &length);
    if (!input) {
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

    parse_result = Parser_parse(parser, input, length);
    if (parse_result >= 0) {
        if (parse_result == length) {
            printf("parse ok.\n");
        }
        else {
            printf("parse succeeded but consumed %d bytes out of %d\n", parse_result, length);
        }
        Parser_print_tree(parser, input);
    }
    else {
        printf("parse failed with result: %d\n", parse_result);
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
        free(input);
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
