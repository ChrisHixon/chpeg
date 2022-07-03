#include <emscripten.h>
#include <stdio.h>
#include <string.h>
#include "../build/chpeg_all.h"

EM_JS(void, switch_output, (const char* which), {
    output = UTF8ToString(which);
})

EM_JS(void, set_result, (const char* which, int value), {
    result_name = UTF8ToString(which);
    result[result_name] = value;
})

int parse(const char *grammar, const char *input, int simplification, int packrat, int profile)
{
    int err = 0; // currently, zero is always returned; result codes for each part
                 // are sent to JS via set_result()

    // default result values (-1 indicates we didn't reach a part)
    int compile_result = -1;
    int parse_result = -1;
    int ast_result = -1;
    int profile_result = -1;

    // baked-in verbose levels
    const int compile_verbose = 0;
    const int parse_verbose = 0;

    ChpegParser *parser = NULL;
    ChpegByteCode *bc = NULL;
    size_t consumed = 0;

    const unsigned char *data = NULL;
    size_t length;

    data = (unsigned char *)grammar;
    length = strlen(grammar);

    //
    // Compile Grammar
    //

    switch_output("compile_status");

    compile_result = chpeg_compile(data, length, &bc, compile_verbose);

    if (compile_result != 0) {
        fprintf(stderr, "Error compiling grammar. "
            "Parser returned: %d\n", compile_result);
        goto done;
    }
    else {
        if (compile_verbose) {
            fprintf(stderr, "Grammar compiled successfully. "
                "Parser returned: %d\n", compile_result);
        }
    }

    assert(bc);

    //
    // Parse input
    //

    switch_output("parse_status");

    parser = ChpegParser_new(bc);

    parser->simplification = simplification;
    parser->packrat_enabled = packrat;
#ifdef CHPEG_VM_PROFILE
    parser->vm_profile = profile;
#endif

    data = (unsigned char *)input;
    length = strlen(input);

    parse_result = ChpegParser_parse(parser, data, length, &consumed);

    if (parse_result == 0) {
        if (parse_verbose) {
            fprintf(stderr, "Parse successful.\n");
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
        ChpegParser_print_error(parser, data);
        goto done;
    }

    //
    // AST
    //

    switch_output("ast");
    ChpegParser_print_tree(parser, data, stdout);
    ast_result = 0; // no errors possible

    //
    // Profile
    //

#ifdef CHPEG_VM_PROFILE
    switch_output("profile");
    ChpegParser_print_profile(parser, data, length, stdout);
    profile_result = 0; // no errors possible
#endif

done:

    set_result("compile", compile_result);
    set_result("parse", parse_result);
    set_result("ast", ast_result);
    set_result("profile", profile_result);

    if (bc) {
        ChpegByteCode_free(bc);
    }
    if (parser) {
        ChpegParser_free(parser);
    }

    return err;
}
