#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "chpeg/compiler.h"
#include "chpeg/parser.h"
#include "chpeg/util.h"

#include "app.h"

//
// Parse
//

void Parse_init_file(Parse *p, const char *filename)
{
    *p = (Parse) {
        .which = 1,
        .filename = filename,
        .parse_result = -1,
    };
}

void Parse_init_str(Parse *p, const char *str)
{
    *p = (Parse) {
        .which = 2,
        .str = str,
        .parse_result = -1,
    };
}

int Parse_cleanup(Parse *p)
{
    if (p) {
        if (p->parser != NULL) {
            ChpegParser_free(p->parser);
        }
        if (p->which == 1) { // file
            free(p->data);
        }
        *p = (Parse) { 0 };
    }
    return 0;
}

int Parse_parse(Parse *p, App *app)
{
    int err = 0;
    unsigned char *file_data = NULL;
    unsigned char *data = NULL;
    size_t length = 0;
    ChpegByteCode *bc = NULL;
    int parse_result = -1;

    // refuse to do anything if parser exists (indicating a parse has happened)
    if (p->parser != NULL) {
        return 1;
    }

    assert(p->which >= 1 && p->which <= 2);
    if (p->which == 1) { // file
        if (app->verbose >= 1) {
            fprintf(stderr, "Parsing file: '%s'\n", p->filename);
        }
        if (chpeg_read_file(p->filename, &file_data, &length) != 0) {
            // chpeg_read_file() will print any errors
            err = 1;
            goto done;
        }
        data = file_data;
    }
    else if (p->which == 2) { // string
        data = (unsigned char *)p->str;
        length = strlen(p->str);
        char *esc = chpeg_esc_bytes((const unsigned char *)p->str, strlen(p->str), 40);
        if (app->verbose >= 1) {
            fprintf(stderr, "Parsing string: \"%s\"\n", esc);
        }
        free(esc);
    }

    // use bytecode from grammar at the top of the grammar stack (if exists)
    if (app->grammars.size > 0) {
        bc = app->grammars.data[app->grammars.size - 1].bc;
    }

    // TODO: print which grammar we're using if verbose

    // Parse the data file using byte code
    p->parser = ChpegParser_new(bc ? bc : chpeg_default_bytecode());

#if CHPEG_VM_TRACE
    // set to non-zero to enable VM instruction execution tracing
    p->parser->vm_trace = app->vm_trace;
#endif

#if CHPEG_VM_PRINT_TREE
    // set to non-zero to print parse tree as it is being built
    p->parser->vm_print_tree = app->vm_print_tree;
#endif

#if CHPEG_VM_PROFILE
    // set to non-zero to enable VM profiling
    p->parser->vm_profile = app->vm_profile;
#endif

#if CHPEG_PACKRAT
    // enable packrat
    p->parser->packrat = app->packrat;
#endif

    size_t consumed = 0;
    parse_result = ChpegParser_parse(p->parser, data, length, &consumed);
    if (parse_result == 0) {
        if (consumed == length) {
            if (app->verbose) {
                fprintf(stderr, "Parse successful.\n");
            }
        }
        if (app->verbose >= 2) {
            ChpegParser_print_tree(p->parser, data, stderr);
        }
    }
    else {
        if (parse_result == CHPEG_ERR_EXTRANEOUS_INPUT) {
            fprintf(stderr, "Extraneous input: parse consumed %lu bytes out of %lu\n",
                consumed, length);
        }
        else {
            fprintf(stderr, "Parse failed with result: %d\n", parse_result);
        }
        ChpegParser_print_error(p->parser, data);
        err = 2;
        goto done;
    }

    err = 0;

done:
    p->data = data;
    p->length = length;
    p->parse_result = parse_result;
    return err;
}
