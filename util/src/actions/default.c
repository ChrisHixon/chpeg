//
// DefaultAction: default action
//

#include "app.h"
#include "parse.h"
#include "actions/default.h"

void DefaultAction_usage(App *app, FILE *fp)
{
    fprintf(stderr, "Usage: %s --%s [GRAMMAR] FILE\n", app->argv[0], app->action->name);
}

int DefaultAction_run(void *_, App *app)
{
    int err = 0;
    Parse input;

    // Case 0: no files supplied as arguments, but a successful parse exists on
    // the parse stack (from -p/-P, for example).
    if (app->files.size == 0 && app->parses.size > 0 &&
        app->parses.data[app->parses.size -1 ].parser &&
        app->parses.data[app->parses.size -1 ].parse_result == 0)
    {
        ChpegParser_print_tree(app->parses.data[app->parses.size - 1].parser,
            app->parses.data[app->parses.size - 1].data, stdout);
        err = 0;
    }

    // Case 1: 1 file supplied as an argument
    // This file should be parsed with the default grammar, or
    // with the last grammar supplied using -g/-G
    else if (app->files.size == 1) {
        // This should automatically handle the cases of:
        // 1) grammar specified with -g/-G
        // 2) no grammar specified (use default)
        Parse_init_file(&input, app->files.data[0]);
        err = Parse_parse(&input, app);
        if (err == 0) {
            ChpegParser_print_tree(input.parser, input.data, stdout);
        }
        Parse_cleanup(&input);
    }

    // Case 2: 2 files supplied as arguments.
    // Treat the first file as a grammar file
    // Treat the second file as the file to parse with that grammar
    else if (app->files.size == 2) {
        if (push_grammar_file(app, app->files.data[0]) == 0) {
            Parse_init_file(&input, app->files.data[1]);
            err = Parse_parse(&input, app);
            if (err == 0) {
                ChpegParser_print_tree(input.parser, input.data, stdout);
            }
            Parse_cleanup(&input);
        }
    }

    // Case 3: no files: if this is the default action, and not the last action,
    // treat as a no-op and return with no error.
    // This allows user to set some global options, etc., and treat
    // the default action like it doesn't really exist.
    // Processing then 'begins', from the user's perspective, with next action.
    else if (app->files.size == 0 && app->action_default
        && app->arg != app->argc)
    {
        err = 0;
    }

    // Else, error
    else {
        if (app->action_default) {
            usage(app, stderr);
        }
        else {
            DefaultAction_usage(app, stderr);
        }
        err = 1;
    }

    return err;
}

Action default_action = {
    .name = "ast",
    .description = "print parse tree/ast (default action)",
    .run = DefaultAction_run,
    .usage = DefaultAction_usage,
};

