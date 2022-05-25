#ifndef PARSE_H
#define PARSE_H

#include "chpeg/parser.h"

#ifndef APP_TYPEDEF
#define APP_TYPEDEF
typedef struct _App App;
#endif

typedef struct _Parse {
    int which;
    union {
        const char *filename;
        const char *str;
    };
    ChpegParser *parser;
    unsigned char *data;
    size_t length;
    int parse_result;
} Parse;

extern void Parse_init_file(Parse *p, const char *filename);
extern void Parse_init_str(Parse *p, const char *str);
extern int Parse_cleanup(Parse *p);
extern int Parse_parse(Parse *p, App *app);

#endif // #ifndef PARSE_H
