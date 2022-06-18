//
// ProfileAction: profile action
//

#include "app.h"
#include "parse.h"
#include "actions/profile.h"

void ProfileAction_usage(App *app, FILE *fp)
{
    fprintf(stderr, "Usage: %s --%s [OPTIONS] [[GRAMMAR] FILE]\n", app->argv[0], app->action->name);
}

// turn on profiling on init so whatever follows will have it enabled
int ProfileAction_init(void *_, App *app)
{
#if CHPEG_VM_PROFILE
    app->vm_profile = 1;
#endif
    return 0;
}

int ProfileAction_run(void *_, App *app)
{
#if CHPEG_VM_PROFILE

    int err = 0;
    Parse input;

    // Case 0: no files supplied as arguments, but a successful parse exists on
    // the parse stack (from -p/-P, for example)
    if (app->files.size == 0 && app->parses.size > 0 &&
        app->parses.data[app->parses.size -1].parser &&
        app->parses.data[app->parses.size -1].parse_result == 0)
    {
        // if profiling was on during parse...
        if (app->parses.data[app->parses.size -1].parser->vm_profile) {
            Parse *parse = &app->parses.data[app->parses.size - 1];
            ChpegParser_print_profile(parse->parser, parse->data, parse->length, stdout);
            err = 0;
        }
        else {
            fprintf(stderr, "%s --%s: profiling was not enabled during parse; "
                "try adding '-prof' before parse option (-p/-P)\n",
                app->argv[0], app->action->name);
            err = 1;
        }
    }

    // Case 1: 1 file supplied as an argument
    // This file should be parsed with the default grammar, or
    // with the last grammar supplied using -g/-G
    else if (app->files.size == 1) {
        if (!app->vm_profile) {
            fprintf(stderr, "%s --%s: profiling is not enabled\n",
                app->argv[0], app->action->name);
            err = 1;
        }
        else {
            // This should automatically handle the cases of:
            // 1) grammar specified with -g/-G
            // 2) no grammar specified (use default)
            Parse_init_file(&input, app->files.data[0]);
            err = Parse_parse(&input, app);
            if (err == 0) {
                ChpegParser_print_profile(input.parser, input.data, input.length, stdout);
            }
            Parse_cleanup(&input);
        }
    }

    // Case 2: 2 files supplied as arguments.
    // Treat the first file as a grammar file
    // Treat the second file as the file to parse with that grammar
    else if (app->files.size == 2) {
        if (!app->vm_profile) {
            fprintf(stderr, "%s --%s: profiling is not enabled\n",
                app->argv[0], app->action->name);
            err = 1;
        }
        else {
            if (push_grammar_file(app, app->files.data[0]) == 0) {
                Parse_init_file(&input, app->files.data[1]);
                err = Parse_parse(&input, app);
                if (err == 0) {
                    ChpegParser_print_profile(input.parser, input.data, input.length, stdout);
                }
                Parse_cleanup(&input);
            }
        }
    }

    // Else, error
    else {
        ProfileAction_usage(app, stderr);
    }

    return err;

#else // profiling not compiled in
    return -1;
#endif

}

Action profile_action = {

    "profile", // name
    "print parse profile information", // description
    NULL, // create
    NULL, // free
    ProfileAction_init, // init
    NULL, // cleanup
    NULL, // arg
    ProfileAction_run, // run
    NULL, // help
    ProfileAction_usage, // usage
};

