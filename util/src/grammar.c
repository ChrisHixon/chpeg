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
    memset(g, 0, sizeof(Grammar));
    g->type = GRAMMAR_FILE;
    g->src.filename = filename;
}

void Grammar_init_str(Grammar *g, const char *str)
{
    memset(g, 0, sizeof(Grammar));
    g->type = GRAMMAR_STRING;
    g->src.str = str;
}

int Grammar_cleanup(Grammar *g)
{
    if (g) {
        if (g->bc) {
            ChpegByteCode_free(g->bc);
        }
        if (g->type == GRAMMAR_FILE) {
            free(g->data);
        }
        memset(g, 0, sizeof(Grammar));
    }
    return 0;
}

int Grammar_compile(Grammar *g, App *app)
{
    int err = 0;
    int parse_result = -1;
    unsigned char *file_data = NULL;
    unsigned char *data = NULL;
    size_t length = 0;

    assert(g->type >= GRAMMAR_FILE && g->type <= GRAMMAR_STRING);
    if (g->type == GRAMMAR_FILE) {
        if (app->verbose >= 1) {
            fprintf(stderr, "Compiling grammar file: '%s'\n", g->src.filename);
        }
        if (chpeg_read_file(g->src.filename, &file_data, &length) != 0) {
            // chpeg_read_file() will print any errors
            err = 1;
            goto done;
        }
        data = file_data;
    }
    else if (g->type == GRAMMAR_STRING) {
        data = (unsigned char *)g->src.str;
        length = strlen(g->src.str);
        char *esc = chpeg_esc_bytes((const unsigned char *)g->src.str,
            strlen(g->src.str), 40);
        if (app->verbose >= 1) {
            fprintf(stderr, "Compiling grammar string: \"%s\"\n", esc);
        }
        free(esc);
    }

    // TODO: in progress, trying to nail down verbosity levels
    static int vmap[4] = { 0, 0, 2, 2 };
    parse_result = chpeg_compile(data, length, &g->bc,
        app->verbose < 2 ? 0 : (app->verbose > 3 ? 2 : vmap[app->verbose]));

    if (parse_result != 0) {
        if (g->type == GRAMMAR_FILE) {
            fprintf(stderr, "Error compiling grammar file '%s'. "
                "Parser returned: %d\n", g->src.filename, parse_result);
        }
        else if (g->type == GRAMMAR_STRING) {
            char *esc = chpeg_esc_bytes((const unsigned char *)g->src.str,
                strlen(g->src.str), 40);
            fprintf(stderr, "Error compiling grammar string \"%s\". "
                "Parser returned: %d\n", esc, parse_result);
            free(esc);
        }

        err = 1;
        goto done;
    }
    else {
        if (app->verbose) {
            if (g->type == GRAMMAR_FILE) {
                fprintf(stderr, "Grammar file '%s' compiled successfully. "
                    "Parser returned: %d\n", g->src.filename, parse_result);
            }
            else if (g->type == GRAMMAR_STRING) {
                char *esc = chpeg_esc_bytes((const unsigned char *)g->src.str,
                    strlen(g->src.str), 40);
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

