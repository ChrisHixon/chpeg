//
// HelpAction: unused for now (help is inline via -h / --help)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "app.h"
#include "actions/help.h"

int HelpAction_run(void *_, App *app)
{
    if (app->files.size == 0) {
        help(app, stdout);
        return 0;
    }
    for (int i = 0; i < app->files.size; i++) {
        const char *name = app->files.data[i];
        assert(name); if (!name) { return 1; }
        const Action *action = get_action(app, name);
        if (!action) {
            fprintf(stderr, "%s: --%s: action not found: %s\n",
                app->argv[0], app->action->name, name);
            return 1;
        }
        if (!action->help) {
            fprintf(stderr, "%s: --%s: no help found for action: %s\n",
                app->argv[0], app->action->name, name);
            return 1;
        }
        action->help(app, stdout);
    }
    return 0;
}

Action help_action = {
    .name = "help",
    .run = HelpAction_run,
};

