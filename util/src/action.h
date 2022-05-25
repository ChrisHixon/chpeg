#ifndef ACTION_H
#define ACTION_H

#include <stdio.h>

struct _App;

typedef struct _Action {
    // everything in Action is optional except for name
    const char *name;        // action name (to be invoked as --name)
    const char *description; // short description used in help

    // if the action needs to allocate data, use these:
    void *(*create) (void);
    void (*free) (void *data);

    // if the action needs to init or clean up anything:
    int (*init) (void *data, struct _App *app);
    int (*cleanup) (void *data, struct _App *app);

    // If template is non-NULL, the app will allocate data of size 'template_size' in bytes,
    // and initialize it with a copy of of 'template'. The data will be automatically freed.
    // The create, init, cleanup, and free functions will not be called.
    void *template; // pre-initialized data template
    size_t template_size; // size of template in bytes

    int (*arg) (void *data, struct _App *app); // handle arguments
    int (*run) (void *data, struct _App *app); // run action

    void (*help) (struct _App *, FILE *fp);  // print help to fp
    void (*usage) (struct _App *, FILE *fp); // print one line usage to fp (used if help is NULL)
} Action;

#endif // #ifndef ACTION_H
