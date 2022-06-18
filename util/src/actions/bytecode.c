#include <assert.h>

#include "chpeg/compiler.h"
#include "app.h"
#include "actions/bytecode.h"

//
// ByteCode: --bytecode action
//

// Helper function to get the bytecode from the last grammar specified with -g/-G;
// if nothing is on grammar stack, return the default chpeg bytecode.
static int get_bytecode(App *app, const ChpegByteCode **bc_out)
{
    assert(bc_out);
    if (app->grammars.size > 0) {
        *bc_out = app->grammars.data[app->grammars.size - 1].bc;
        assert(*bc_out);
        return 0;
    }
    *bc_out = chpeg_default_bytecode();
    assert(*bc_out);
    return 0;
}

void ByteCodeAction_usage(App *app, FILE *fp)
{
    fprintf(stderr, "Usage: --bytecode [ -g GRAMMAR_FILE | -G GRAMMAR_STRING ]\n");
}

int ByteCodeAction_run(void *_, App *app)
{
    int err = 1;

    const ChpegByteCode *bc = NULL;

    // accept grammar via -g/-G, or use default bytecode if none specified
    if ((err = get_bytecode(app, &bc)) != 0) {
        goto done;
    }

    assert(bc);
    if (app->files.size == 0)
    {
        ChpegByteCode_print(bc);
        err = 0;
    }
    else {
        ByteCodeAction_usage(app, stderr);
        err = 1;
    }

done:
    return err;
}

Action bytecode_action = {
    "bytecode", // name
    "print bytecode", // description
    NULL, // create
    NULL, // free
    NULL, // init
    NULL, // cleanup
    NULL, // arg
    ByteCodeAction_run, // run
    NULL, // help
    ByteCodeAction_usage, // usage
};

