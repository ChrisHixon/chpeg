#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#ifdef CHPEG_USE_AMALG
#ifdef CHPEG_EXTENSIONS
#include "chpeg_ext.h" // chpeg ext amalgamation
#else
#include "chpeg.h" // chpeg amalgamation
#endif
#else
#include "chpeg/mem.h"
#include "chpeg/util.h"
#endif

#include "app.h"
#include "actions/default.h"
#include "actions/bytecode.h"
#include "actions/bytecodec.h"
#include "actions/test.h"

#if CHPEG_VM_PROFILE
#include "actions/profile.h"
#endif

//
// Actions and argument processing:
//
// [--ACTION [OPTION | FILE]... ]...
//
// An action block consists of an action argument (--ACTION), followed by any
// number of OPTION or FILE arguments. An action block is terminated when
// arguments are exhausted, or the next action argument is encountered. Any
// number of action blocks may occur. If no initial action argument is present,
// the default action is selected.
//
// Arguments processing is 'live'; effects of arguments may be immediate. An
// action is run as soon as its action block is determined to be complete.
//
// HELP:
// If -h or --help occurs as an OPTION, help is printed once, and the action will
// not be run; however, all arguments are processed and any effects of argument
// processing will still occur. This behavior may change in the future. It's
// recommended to use -h/--help as the only argument in an action block.
//

//
// App
//

// simplified one line global usage
static void usage_line(App *app, FILE *fp)
{
    fprintf(fp, "Usage: %s [GRAMMAR] FILE | [--ACTION [OPTION | FILE]... ]...\n", app->argv[0]);
}

void usage(App *app, FILE *fp)
{
    usage_line(app, fp);
    fprintf(fp, "Try '%s --help' for more information.\n", app->argv[0]);
}

void help(App *app, FILE *fp)
{
    usage_line(app, fp);
    fprintf(fp, "\n");
    fprintf(fp, "Global options:\n");
    fprintf(fp, "  -g GRAMMAR      compile grammar from file GRAMMAR\n");
    fprintf(fp, "  -G STRING       compile grammar specified as STRING\n");
    fprintf(fp, "  -p FILE         parse FILE, using grammar from -g/-G, if present\n");
    fprintf(fp, "  -P STRING       parse STRING, using grammar from -g/-G, if present\n");
    fprintf(fp, "\n");
    fprintf(fp, "Global debugging and verbosity options:\n");
    fprintf(fp, "  -sN             set AST simplification to N: 0=off, 1=unwrap, 2=simplify(default)\n");
    fprintf(fp, "  -d              increase debug level\n");
    fprintf(fp, "  -dN             set debug level to N (use 0 to disable)\n");
    fprintf(fp, "  -v              increase verbosity\n");
    fprintf(fp, "  -vN             set verbosity to N (use 0 to disable)\n");
    fprintf(fp, "\n");

#if CHPEG_VM_TRACE || CHPEG_VM_PRINT_TREE
    fprintf(fp, "Tracing options:\n");
#endif

#if CHPEG_VM_TRACE
    fprintf(fp, "  -t              increase VM trace level\n");
    fprintf(fp, "  -tN             set VM trace level to N (0=off, 1=on)\n");
#endif

#if CHPEG_VM_PRINT_TREE
    fprintf(fp, "  -tp             increase VM print tree level\n");
    fprintf(fp, "  -tpN            set VM print tree level to N (0=off, 1=on)\n");
#endif

#if CHPEG_VM_TRACE || CHPEG_VM_PRINT_TREE
    fprintf(fp, "\n");
#endif

#if CHPEG_VM_PROFILE
    fprintf(fp, "Profiling options:\n");
    fprintf(fp, "  -prof           increase VM profiling level\n");
    fprintf(fp, "  -profN          set VM profiling level to N (0=off,1=on)\n");
    fprintf(fp, "\n");
#endif

#if CHPEG_PACKRAT
    fprintf(fp, "Packrat options:\n");
    fprintf(fp, "  -packrat        enable packrat\n");
    fprintf(fp, "  -packratN       set packrat to N (0=off,1=on)\n");
    fprintf(fp, "  -window=N       set packrat window size to N (default is input size)\n");
    fprintf(fp, "\n");
#endif

    fprintf(fp, "Actions: (in form --ACTION)\n");
    for (int i = 0; i < app->num_actions; i++) {
        const Action *action = app->actions[i];
        if (action->name && action->description) {
            fprintf(fp, "  --%-13s %s\n", action->name, action->description);
        }
    }
    fprintf(fp, "\n");
    fprintf(fp, "Help:\n");
    fprintf(fp, "   %s --help             print global help\n", app->argv[0]);
    fprintf(fp, "   %s --ACTION --help    print help for ACTION\n", app->argv[0]);
}

int init(App *app, int argc, const char **argv,
    size_t num_actions, const Action **actions)
{
    assert(argc > 0 && argv);
    if (!(argc > 0 && argv)) return -1;

    assert(num_actions > 0 && actions);
    if (!(num_actions > 0 && actions)) return -1;

    memset(app, 0, sizeof(App));
    app->argc = argc;
    app->argv = argv;
    app->num_actions = num_actions;
    app->actions = actions;
    app->simplification = 2;

    return 0;
}

void cleanup_grammars(App *app)
{
    for (size_t z = 0; z < app->grammars.size; z++) {
        assert(&app->grammars.data[z]);
        Grammar_cleanup(&app->grammars.data[z]);
    }
    GrammarArray_cleanup(&app->grammars);
}

void cleanup_parses(App *app)
{
    for (size_t z = 0; z < app->parses.size; z++) {
        assert(&app->parses.data[z]);
        Parse_cleanup(&app->parses.data[z]);
    }
    ParseArray_cleanup(&app->parses);
}

void cleanup_files(App *app)
{
    StringArray_cleanup(&app->files);
}

void cleanup(App *app)
{
    cleanup_grammars(app);
    cleanup_parses(app);
    cleanup_files(app);
}

// print App state for debugging (can be called mid-processing any time with -state)
void print_state(App *app, FILE *fp)
{
    fprintf(fp, "STATE: verbose=%d, debug=%d, error=%d, simplification=%d | "
        "action_start=%d, action_default=%d, help_printed=%d | "
        "grammars.size=%zu, grammars.capacity=%zu | "
        "parses.size=%zu, parses.capacity=%zu | "
        "files.size=%zu, files.capacity=%zu | arg=%d, argc=%d\n",
        app->verbose, app->debug, app->error, app->simplification,
        app->action_start, app->action_default, app->help_printed,
        app->grammars.size, app->grammars.capacity,
        app->parses.size, app->parses.capacity,
        app->files.size, app->files.capacity,
        app->arg, app->argc
        );
    char *esc = NULL;
    for (size_t z = 0; z < app->grammars.size; z++) {
        switch (app->grammars.data[z].type) {
            case GRAMMAR_FILE:
                fprintf(stderr, "GRAMMAR[%zu]: filename=\"%s\", bc=%p\n",
                    z, app->grammars.data[z].src.filename,
                    (void *)app->grammars.data[z].bc);
                break;
            case GRAMMAR_STRING:
                esc = chpeg_esc_bytes((unsigned char *)app->grammars.data[z].src.str,
                    strlen(app->grammars.data[z].src.str), 40);
                fprintf(stderr, "GRAMMAR[%zu]: str=\"%s\", bc=%p\n",
                    z, esc, (void *)app->grammars.data[z].bc);
                CHPEG_FREE(esc);
                break;
        }
    }
    for (size_t z = 0; z < app->parses.size; z++) {
        switch (app->parses.data[z].type) {
            case PARSE_FILE:
                fprintf(stderr, "PARSE[%zu]: filename=\"%s\", parser=%p\n",
                    z, app->parses.data[z].src.filename,
                    (void *)app->parses.data[z].parser);
                break;
            case PARSE_STRING:
                esc = chpeg_esc_bytes((unsigned char *)app->parses.data[z].src.str,
                    strlen(app->parses.data[z].src.str), 40);
                fprintf(stderr, "PARSE[%zu]: str=\"%s\", parser=%p\n",
                    z, esc, (void *)app->parses.data[z].parser);
                CHPEG_FREE(esc);
                break;
        }
    }
    for (size_t z = 0; z < app->files.size; z++) {
        fprintf(stderr, "FILE[%zu]: filename=\"%s\"\n",
            z, app->files.data[z]);
    }
}

// compile a grammar file to bytecode and push to grammar array
int push_grammar_file(App *app, const char *filename)
{
    Grammar g;
    Grammar_init_file(&g, filename);

    if (Grammar_compile(&g, app) != 0) {
        Grammar_cleanup(&g);
        return 1;
    }

    GrammarArray_push_back(&app->grammars, g);

    return 0;
}

// compile a grammar string to bytecode and push to grammar array
int push_grammar_str(App *app, const char *str)
{
    Grammar g;
    Grammar_init_str(&g, str);

    if (Grammar_compile(&g, app) != 0) {
        Grammar_cleanup(&g);
        return 1;
    }

    GrammarArray_push_back(&app->grammars, g);

    return 0;
}

// parse a file and push to parse array
int push_parse_file(App *app, const char *filename)
{
    Parse p;
    Parse_init_file(&p, filename);

    if (Parse_parse(&p, app) != 0) {
        Parse_cleanup(&p);
        return 1;
    }

    ParseArray_push_back(&app->parses, p);

    return 0;
}

// parse a string and push to parse array
int push_parse_str(App *app, const char *str)
{
    Parse p;
    Parse_init_str(&p, str);

    if (Parse_parse(&p, app) != 0) {
        Parse_cleanup(&p);
        return 1;
    }

    ParseArray_push_back(&app->parses, p);

    return 0;
}

// push a filename to file array
void push_file(App *app, const char *filename)
{
    StringArray_push_back(&app->files, filename);
}

// get an action by name
const Action *get_action(App *app, const char *name)
{
    for (int i = 0; i < app->num_actions; i++) {
        assert(app->actions[i]);
        if (!app->actions[i]) continue;
        if (strcmp(name, app->actions[i]->name) == 0) {
            return app->actions[i];
        }
    }
    return NULL;
}

// if current arg is an action arg (--action), return action index (>= 0);
// returns -1 otherwise
static inline int arg_action(App *app)
{
    const char *arg = app->argv[app->arg];
    if (strlen(arg) <= 2 || strncmp(arg, "--", 2) != 0) return -1;
    for (int i = 0; i < app->num_actions; i++) {
        if (strcmp(&arg[2], app->actions[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}

// Integer argument processing helper. Matches "<prefix><int>" in current (single) argument
// If match and conversion is successful, returns int in *value
static inline int arg_int(App *app, const char *prefix, int *value)
{
    const char *arg = app->argv[app->arg];
    char *end = NULL;
    size_t plen = strlen(prefix);

    if (strlen(arg) <= plen || strncmp(arg, prefix, plen) != 0) return 1;
    arg += plen;

    long lval = strtol(arg, &end, 0);
    if (*end != '\0') return 2;
    if (lval < INT_MIN || lval > INT_MAX) return 3;
    if (value) {
        *value = lval;
    }
    return 0;
}

// helper to check if there is at least one additional argument available
static int have_arg(App *app)
{
    if (app->arg + 1 >= app->argc) {
        fprintf(stderr, "%s: option '%s' requires an argument (action: %s, arg: %d)\n",
            app->argv[0], app->argv[app->arg], app->action->name, app->arg);
        return 1;
    }
    return 0;
}

// init action (create/init)
static int init_action(App *app)
{
    int err = 0;

    assert(app->action);

    if (app->action->create) {
        app->action_data = app->action->create();
    }
    else {
        app->action_data = NULL;
    }

    if (app->action->init) {
        if ((err = app->action->init(app->action_data, app)) != 0) {
            goto done;
        }
    }

done:
    return err;
}

// helper to clean up after an action has run
static int cleanup_action(App *app)
{
    if (app->action->cleanup) {
        app->action->cleanup(app->action_data, app);
    }
    if (app->action_data) {
        // if action has created data, it should also free it
        assert(app->action->free);
        if (app->action->free) {
            app->action->free(app->action_data);
        }
        app->action_data = NULL;
    }

    // Clear file list
    cleanup_files(app);
    return 0;
}


// run app
static int run(App *app)
{
    int err = 0, value = 0;

    // reset arg processing / run state
    app->action =  NULL;
    app->action_data = NULL;
    app->arg = app->action_start = app->action_default =
        app->help_printed = app->error = 0;

    //
    // init arg processing / action run state
    //

    // if the initial argument is an action
    app->arg = 1;
    if (app->arg < app->argc && (value = arg_action(app)) >= 0) {
        app->action = app->actions[value];
        app->action_start = app->arg;
        app->action_default = 0;
        ++app->arg;
    }
    // else, use default action
    else {
        assert(app->num_actions > 0);
        app->action = app->actions[0];
        app->action_start = app->arg;
        app->action_default = 1;
    }

    // initialize action data
    if ((err = init_action(app)) != 0) {
        goto done;
    }

    //
    // arg processing / run loop
    //

    for (; app->arg < app->argc; ++app->arg) {
        assert(app->action);
        assert(app->arg > 0);

        // verbose (-v)
        if (strcmp(app->argv[app->arg], "-v") == 0) {
            app->verbose++;
        }
        // verbose (-vN)
        else if (arg_int(app, "-v", &value) == 0) {
            app->verbose = value;
        }
        // debug (-d)
        else if (strcmp(app->argv[app->arg], "-d") == 0) {
            app->debug++;
        }
        // debug (-dN)
        else if (arg_int(app, "-d", &value) == 0) {
            app->debug = value;
        }
        // AST simplification (-sN)
        else if (arg_int(app, "-s", &value) == 0) {
            app->simplification = value;
        }
        // dump state for debugging purposes (-state)
        else if (strcmp(app->argv[app->arg], "-state") == 0) {
            print_state(app, stderr);
        }
        // help (-h, --help)
        else if (strcmp(app->argv[app->arg], "-h") == 0 ||
            strcmp(app->argv[app->arg], "--help") == 0)
        {
            // Warning: action will not run if help has been printed;
            // however, side effects of argument processing will still occur

            if (!app->help_printed) { // Ignore additional -h/--help for current action
                if (app->action_default) {
                    help(app, stdout);
                }
                else if (app->action->help) {
                    app->action->help(app, stdout);
                }
                else if (app->action->usage) {
                    app->action->usage(app, stdout);
                }
                else {
                    fprintf(stderr, "%s: --help: no help found for action: %s\n",
                        app->argv[0], app->action->name);
                }
                app->help_printed = 1;
            }
        }

#if CHPEG_VM_TRACE
        // vm trace can be turned on (-t/-t1) and off (-t0) as necessary, mid-processing

        // vm trace (-t)
        else if (strcmp(app->argv[app->arg], "-t") == 0) {
            app->vm_trace++;
        }
        // vm trace (-tN)
        else if (arg_int(app, "-t", &value) == 0) {
            app->vm_trace = value;
        }
#endif

#if CHPEG_VM_PRINT_TREE
        // vm print tree can be turned on (-tp/-tp1) and off (-tp0) as necessary, mid-processing

        // this option shows the parse tree as being built
        // vm print tree (-tp)
        else if (strcmp(app->argv[app->arg], "-tp") == 0) {
            app->vm_print_tree++;
        }
        // vm print tree (-tpN)
        else if (arg_int(app, "-tp", &value) == 0) {
            app->vm_print_tree = value;
        }
#endif

#if CHPEG_VM_PROFILE
        // vm profiling can be turned on (-prof/-prof1) and off (-prof0) as necessary, mid-processing

        // vm profile (-prof)
        else if (strcmp(app->argv[app->arg], "-prof") == 0) {
            app->vm_profile++;
        }
        // vm profile (-profN)
        else if (arg_int(app, "-prof", &value) == 0) {
            app->vm_profile = value;
        }
#endif

#if CHPEG_PACKRAT
        // turn packrat on (-packrat/-packrat1) and off (-packrat0) as necessary, mid-processing

        // packrat (-packrat)
        else if (strcmp(app->argv[app->arg], "-packrat") == 0) {
            app->packrat++;
        }
        // packrat (-packratN)
        else if (arg_int(app, "-packrat", &value) == 0) {
            app->packrat = value;
        }
        // packrat (-window=N)
        else if (arg_int(app, "-window=", &value) == 0) {
            app->packrat_window_size = value;
        }
#endif

        // grammar, from file (-g FILE)
        else if (strcmp(app->argv[app->arg], "-g") == 0) {
            if ((err = have_arg(app)) != 0) {
                goto done;
            }
            app->arg++;
            if ((err = push_grammar_file(app, app->argv[app->arg])) != 0) {
                goto done;
            }
        }
        // grammar, from string (-G STRING)
        else if (strcmp(app->argv[app->arg], "-G") == 0) {
            if ((err = have_arg(app)) != 0) {
                goto done;
            }
            app->arg++;
            if ((err = push_grammar_str(app, app->argv[app->arg])) != 0) {
                goto done;
            }
        }
        // parse file (-p FILE)
        else if (strcmp(app->argv[app->arg], "-p") == 0) {
            if ((err = have_arg(app)) != 0) {
                goto done;
            }
            app->arg++;
            if ((err = push_parse_file(app, app->argv[app->arg])) != 0) {
                goto done;
            }
        }
        // parse string (-P STRING)
        else if (strcmp(app->argv[app->arg], "-P") == 0) {
            if ((err = have_arg(app)) != 0) {
                goto done;
            }
            app->arg++;
            if ((err = push_parse_str(app, app->argv[app->arg])) != 0) {
                goto done;
            }
        }
        // send arg to action to see if it wants it
        else if (app->action->arg != NULL &&
            app->action->arg(app->action_data, app) == 0)
        {
            // if action set app->error, exit run loop
            if ((err = app->error) != 0) {
                goto done;
            }
        }
        // action argument (--action)
        // end current action block; run action; start new action block
        else if ((value = arg_action(app)) >= 0) {

            // Warning: action will not run if help has been printed;
            // however, side effects of argument processing will still occur

            // Run action
            if (!app->help_printed) {
                if (app->action->run) {
                    if ((err = app->action->run(app->action_data, app)) != 0) {
                        goto done;
                    }
                }
            }

            // Cleanup action data
            cleanup_action(app);

            // Begin next action
            app->action = app->actions[value];
            assert(app->action);
            assert(app->action_data == NULL);

            app->action_start = app->arg;
            app->action_default = 0;
            app->help_printed = 0;
            app->error = 0;

            // Init action data
            if ((err = init_action(app)) != 0) {
                goto done;
            }
        }
        // unhandled OPTION (prefix is '-')
        else if (strlen(app->argv[app->arg]) > 1 && app->argv[app->arg][0] == '-') {
            fprintf(stderr, "%s: invalid option '%s' (action: %s, arg: %d)\n",
                app->argv[0], app->argv[app->arg], app->action->name, app->arg);
            usage(app, stderr);
            err = 1; goto done;
        }
        // empty arg (any reason to not allow this?)
        /*
        else if (strlen(app->argv[app->arg]) == 0) {
            fprintf(stderr, "%s: argument is empty (action: %s, arg: %d)\n",
                app->argv[0], app->action->name, app->arg);
            usage(app, stderr);
            err = 1; goto done;
        }
        */
        // anything else considered a FILE
        else {
            push_file(app, app->argv[app->arg]);
        }
    }

    // Warning: action will not run if help printed;
    // however, side effects of arg processing will still occur
    if (!app->help_printed) {
        if (app->action->run != NULL) {
            if ((err = app->action->run(app->action_data, app)) != 0) {
                goto done;
            }
        }
    }

    err = 0;

done:
    cleanup_action(app);
    cleanup(app);
    return err;
}

//
// null action: does nothing, but arguments are still processed.
//
// It can be used to terminate an action, or when argument side-effects are
// desired without running an action. Also it is useful as a test to make sure
// nothing but name is required for an Action.
//

static Action null_action = {
    "null", // name
    NULL, // description
    NULL, // create
    NULL, // free
    NULL, // init
    NULL, // cleanup
    NULL, // arg
    NULL, // run
    NULL, // help
    NULL, // usage
};


int main(int argc, const char *argv[])
{
    App app;
    int err = 0;

    const Action *actions[] = {
        &default_action,
        &bytecode_action,
        &bytecodec_action,

#if CHPEG_VM_PROFILE
        &profile_action,
#endif

        &test_action,
        &null_action,
    };

    size_t num_actions = sizeof(actions) / sizeof(Action *);

    if ((err = init(&app, argc, argv, num_actions, actions)) != 0) {
        return err;
    }

    return run(&app);
}

