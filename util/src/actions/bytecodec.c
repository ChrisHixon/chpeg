#include <assert.h>
#include <string.h>

#include "chpeg/compiler.h"
#include "app.h"
#include "actions/bytecode.h"

//
// ByteCodeC: --bytecode-c action
//

// data struct used to collect arguments
typedef struct _ByteCodeCAction {
    const char *basename;
    const char *varname;
    const char *prefix;
    const char *h;
    const char *c;
} ByteCodeCAction;

void *ByteCodeCAction_create(void)
{
    return calloc(1, sizeof(ByteCodeCAction));
}

void ByteCodeCAction_free(void *data)
{
    free(data);
}

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

// arg callback: collect arguments in our struct for later use during run callback
int ByteCodeCAction_arg(void *data, App *app)
{
    ByteCodeCAction *self = (ByteCodeCAction *)data;

    static const char *str_args[] = {
        "-basename", "-varname", "-prefix", "-cout", "-hout", NULL
    };
    const char **str_addrs[] = {
        &self->basename, &self->varname, &self->prefix,
        &self->c, &self->h, NULL
    };

    for (int i = 0; str_args[i]; i++) {
        if (strcmp(app->argv[app->arg], str_args[i]) == 0) {
            if (app->arg + 1 >= app->argc) {
                fprintf(stderr, "%s: option '%s' requires a value (action: %s, arg: %d)\n",
                    app->argv[0], str_args[i], app->action->name, app->arg);
                // set app->error = 1 and return 0 for error
                app->error = 1;
                return 0;
            }
            app->arg++;
            assert(str_addrs[i]);
            if (str_addrs[i]) {
                *str_addrs[i] = app->argv[app->arg];
            }
            // return 0 if accepting arg
            return 0;
        }
    }
    return 1; // return 1 if arg unwanted
}

void ByteCodeCAction_usage(App *app, FILE *fp)
{
    fprintf(fp, "Usage: --bytecodec [ (-g GRAMMAR_FILE | -G GRAMMAR_STRING ) -basename BASENAME] [-varname VARNAME] [-prefix PREFIX] [-cout CFILE] [-hout HFILE]\n");
}

int ByteCodeCAction_run(void *data, App *app)
{
    ByteCodeCAction *self = (ByteCodeCAction *)data;

    int err = 1;
    const ChpegByteCode *bc = NULL;
    FILE *fp = NULL, *fp_c = NULL, *fp_h = NULL;

    // get the bytecode to export
    if ((err = get_bytecode(app, &bc)) != 0) {
        goto done;
    }
    assert(bc);

    // return error if any files have been specified
    if (app->files.size > 0) {
        ByteCodeCAction_usage(app, stderr);
        err = 1; goto done;
    }

    // make sure we have a basename - the only real requirement of the
    // _output_[ch] routines
    if (!self->basename) {
        // if we're using the default bytecode, use the default basename
        if (bc == chpeg_default_bytecode()) {
            self->basename = "chpeg_bytecode";
        }
        // --basename is required if using a grammar file/string (-g/-G)
        else {
            ByteCodeCAction_usage(app, stderr);
            err = 1; goto done;
        }
    }

    // .h file

    if (self->h == NULL || strcmp(self->h, "-") == 0) {
        fp = stdout;
    }
    else {
        fp_h = fopen(self->h, "w");
        if (fp_h == NULL) {
            perror(self->h);
            err = 1; goto done;
        }
        fp = fp_h;
    }

    ChpegByteCode_output_h(bc, fp, self->basename, self->varname,
        self->prefix, NULL);


    // .c file

    if (self->c == NULL || strcmp(self->c, "-") == 0) {
        fp = stdout;
    }
    else {
        fp_c = fopen(self->c, "w");
        if (fp_c == NULL) {
            perror(self->c);
            err = 1; goto done;
        }
        fp = fp_c;
    }

    ChpegByteCode_output_c(bc, fp, self->basename, self->varname,
        self->prefix, NULL);

    err = 0;

done:
    if (fp_h) {
        fclose(fp_h);
    }
    if (fp_c) {
        fclose(fp_c);
    }
    return err;
}

Action bytecodec_action = {

    "bytecodec", // name
    "print bytecode as C language source code", // description
    ByteCodeCAction_create, // create
    ByteCodeCAction_free, // free
    NULL, // init
    NULL, // cleanup
    ByteCodeCAction_arg, // arg
    ByteCodeCAction_run, // run
    NULL, // help
    ByteCodeCAction_usage, // usage
};

