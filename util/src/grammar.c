#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "chpeg/compiler.h"
#include "chpeg/util.h"

#include "app.h"

//
// Grammar
//

void Grammar_init_file(Grammar *g, const char *filename)
{
    *g = (Grammar) {
        .which = 1,
        .filename = filename,
    };
}

void Grammar_init_str(Grammar *g, const char *str)
{
    *g = (Grammar) {
        .which = 2,
        .str = str,
    };
}

int Grammar_cleanup(Grammar *g)
{
    if (g->bc) {
        ChpegByteCode_free(g->bc);
    }
    if (g->which == 1) { // file
        free(g->data);
    }
    *g = (Grammar) { 0 };
    return 0;
}

int Grammar_compile(Grammar *g, App *app)
{
    int err = 0;
    unsigned char *file_data = NULL;
    unsigned char *data = NULL;
    size_t length = 0;

    assert(g->which >= 1 && g->which <= 2);
    if (g->which == 1) { // file
        if (app->verbose >= 1) {
            fprintf(stderr, "Compiling grammar file: '%s'\n", g->filename);
        }
        if (chpeg_read_file(g->filename, &file_data, &length) != 0) {
            // chpeg_read_file() will print any errors
            err = 1;
            goto done;
        }
        data = file_data;
    }
    else if (g->which == 2) { // string
        data = (unsigned char *)g->str;
        length = strlen(g->str);
        char *esc = chpeg_esc_bytes((const unsigned char *)g->str, strlen(g->str), 40);
        if (app->verbose >= 1) {
            fprintf(stderr, "Compiling grammar string: \"%s\"\n", esc);
        }
        free(esc);
    }

    // TODO: in progress, trying to nail down verbosity levels
    static int vmap[4] = { 0, 0, 2, 2 };
    int parse_result = chpeg_compile(data, length, &g->bc,
        app->verbose < 2 ? 0 : (app->verbose > 3 ? 2 : vmap[app->verbose]));

    if (parse_result != 0) {
        if (g->which == 1) { // file
            fprintf(stderr, "Error compiling grammar file '%s'. "
                "Parser returned: %d\n", g->filename, parse_result);
        }
        else if (g->which == 2) { // string
            char *esc = chpeg_esc_bytes((const unsigned char *)g->str, strlen(g->str), 40);
            fprintf(stderr, "Error compiling grammar string \"%s\". "
                "Parser returned: %d\n", esc, parse_result);
            free(esc);
        }

        err = 1;
        goto done;
    }
    else {
        if (app->verbose) {
            if (g->which == 1) { // file
                fprintf(stderr, "Grammar file '%s' compiled successfully. "
                    "Parser returned: %d\n", g->filename, parse_result);
            }
            else if (g->which == 2) { // string
                char *esc = chpeg_esc_bytes((const unsigned char *)g->str,
                    strlen(g->str), 40);
                fprintf(stderr, "Grammar string \"%s\" compiled successfully. "
                    "Parser returned: %d\n", esc, parse_result);
                free(esc);
            }
        }
        err = 0;
    }
    assert(g->bc != NULL);
done:
    if (file_data) {
        free(file_data);
    }
    return err;
}

