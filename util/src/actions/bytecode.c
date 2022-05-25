#include <assert.h>

#include "chpeg/compiler.h"
#include "app.h"
#include "actions/bytecode.h"

//
// ByteCode: --bytecode action
//

// Helper function to get the bytecode from the last grammar specified with -g/-G;
// if nothing is on grammar stack, return the default chpeg bytecode.
static int get_bytecode(App *app, const ChpegByteCode **bc)
{
    assert(bc);
    if (app->grammars.size > 0) {
        *bc = app->grammars.data[app->grammars.size - 1].bc;
        assert(*bc);
        return 0;
    }
    *bc = chpeg_default_bytecode();
    assert(*bc);
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
    .name = "bytecode",
    .description = "print bytecode",
    .run = ByteCodeAction_run,
    .usage = ByteCodeAction_usage,
};

