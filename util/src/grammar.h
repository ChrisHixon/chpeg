#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "chpeg/bytecode.h"

#ifndef APP_TYPEDEF
#define APP_TYPEDEF
typedef struct _App App;
#endif

typedef struct _Grammar {
    int which;
    union {
        const char *filename;
        const char *str;
    };
    ChpegByteCode *bc;
    unsigned char *data;
    size_t length;
} Grammar;

extern void Grammar_init_file(Grammar *g, const char *filename);
extern void Grammar_init_str(Grammar *g, const char *str);
extern int Grammar_cleanup(Grammar *g);
extern int Grammar_compile(Grammar *g, App *s);

#endif // #ifndef GRAMMAR_H
