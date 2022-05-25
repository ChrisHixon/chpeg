//
// TestAction: example action that implements all Action methods
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "actions/test.h"

// data structure
typedef struct _TestAction {
    int value;
} TestAction;

void *TestAction_create(void)
{
    printf("TestAction_create()\n");
    return malloc(sizeof(TestAction));
}

int TestAction_init(void *data, App *app)
{
    printf("TestAction_init()\n");
    TestAction *self = (TestAction *)data;
    self->value = 0;
    return 0;
}

int TestAction_arg(void *data, App *app)
{
    printf("TestAction_arg()\n");
    TestAction *self = (TestAction *)data;
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
        printf("TestAction_arg(): set value=%d\n", self->value);
        // return 0 if accepting arg
        return 0;
    }
    else if (strcmp(arg, "-error") == 0) {
        fprintf(stderr, "got -error!\n");
        // set app->error = 1 and return 0 for error
        app->error = 1;
        return 0;
    }
    else if (strcmp(arg, "-skip") == 0) {
        fprintf(stderr, "got -skip!\n");
        // skip remaining arg processing by modifying app->arg
        app->arg = app->argc;
        // return 0 if accepting arg
        return 0;
    }
    return 1; // return 1 if arg unwanted
}

int TestAction_run(void *data, App *app)
{
    TestAction *self = (TestAction *)data;
    printf("TestAction_run(): value=%d\n", self->value);
    print_state(app, stderr);
    return 0;
}

int TestAction_cleanup(void *data, App *app)
{
    printf("TestAction_cleanup()\n");
    return 0;
}

void TestAction_free(void *data)
{
    printf("TestAction_free()\n");
    free(data);
}

Action test_action = {
    .name = "test",
    .create = TestAction_create,
    .init = TestAction_init,
    .arg = TestAction_arg,
    .run = TestAction_run,
    .cleanup = TestAction_cleanup,
    .free = TestAction_free,
};

