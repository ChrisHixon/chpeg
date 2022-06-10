#ifndef APP_H
#define APP_H

#include "action.h"
#include "arrays.h"

#ifndef APP_TYPEDEF
#define APP_TYPEDEF
typedef struct _App App;
#endif

struct _App {
    int argc;               // argc from main
    const char **argv;      // argv from main
    const Action **actions;
    int num_actions;

    int verbose;
    int debug;
    int error;
    int simplification;

    // arg processing / action run state
    const Action *action;   // current action
    void *action_data;      // action data
    int arg;                // arg number currently being processed
    int action_start;       // arg number of action start
    int action_default;     // action is running as default action (bool)
    int help_printed;       // help has been printed during this action (bool)

#if CHPEG_VM_TRACE
    int vm_trace;
#endif
#if CHPEG_VM_PRINT_TREE
    int vm_print_tree;
#endif
#if CHPEG_VM_PROFILE
    int vm_profile;
#endif
#if CHPEG_PACKRAT
    int packrat;
    int packrat_window_size;
#endif

    // Grammar list/stack
    GrammarArray grammars;

    // Parse list/stack
    ParseArray parses;

    // filename array
    StringArray files;
};

extern void help(App *app, FILE *fp);
extern void usage(App *app, FILE *fp);
/*
extern void cleanup_grammars(App *app);
extern void cleanup_parses(App *app);
extern void cleanup_files(App *app);
extern void cleanup(App *app);
*/
extern void print_state(App *app, FILE *fp);
extern int push_grammar_file(App *app, const char *filename);
extern int push_grammar_str(App *app, const char *str);
extern int push_parse_file(App *app, const char *filename);
extern int push_parse_str(App *app, const char *str);
extern void push_file(App *app, const char *filename);
extern const Action *get_action(App *app, const char *name);

#endif // #ifndef APP_H
