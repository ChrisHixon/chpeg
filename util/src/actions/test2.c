//
// Test2Action: same as TestAction but with template instead of create/init/cleanup/free
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "actions/test2.h"

// data structure
typedef struct _Test2Action {
    int value;
} Test2Action;

int Test2Action_arg(void *data, App *app)
{
    printf("Test2Action_arg()\n");
    Test2Action *self = (Test2Action *)data;
    const char *arg = app->argv[app->arg];
    if (strcmp(arg, "-sv") == 0) {
        if (++app->arg >= app->argc) {
            fprintf(stderr, "%s: option '-sv' requires a value (action: %s, arg: %d)\n",
                app->argv[0], app->action->name, app->arg);
            // set app->error = 1 and return 0 for error
            app->error = 1;
            return 0;
        }
        arg = app->argv[app->arg];
        self->value = atoi(arg);
        printf("Test2Action_arg(): set value=%d\n", self->value);
        // return 0 to accept arg
        return 0;
    }
    else if (strcmp(arg, "-error") == 0) {
        fprintf(stderr, "got -error!\n");
        // set app->error = 1 and return 0 for error
        app->error = 1;
        return 0;
    }
    else if (strcmp(arg, "-end") == 0) {
        fprintf(stderr, "got -end\n");
        // skip remaining arg processing by modifying app->arg
        app->arg = app->argc;
        // return 0 if accepting arg
        return 0;
    }
    return 1; // return 1 if arg unwanted
}

int Test2Action_run(void *data, App *app)
{
    Test2Action *self = (Test2Action *)data;
    printf("Test2Action_run(): value=%d\n", self->value);
    print_state(app, stderr);
    return 0;
}

static Test2Action template = { 0 };

Action test2_action = {
    .name = "test2",
    .template = &template,
    .template_size = sizeof(template),
    .arg = Test2Action_arg,
    .run = Test2Action_run,
};

