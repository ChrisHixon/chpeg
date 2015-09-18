#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DEBUG_MEM 1
#if DEBUG_MEM
    #include <mcheck.h>
#endif

#include "cbyte.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd = -1;
    unsigned char *input = NULL; 
    Parser *parser = NULL; 

#if DEBUG_MEM
    mtrace();
#endif

    if (argc != 2) {
        printf("usage: %s <file>\n", argv[0]);
        ret = 2;
        goto done;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror(argv[1]);
        ret = 3;
        goto done;
    }

    input = (unsigned char*)malloc(st.st_size);
    if (!input) {
        ret = 255;
        goto done;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(argv[1]);
        ret = 4;
        goto done;
    }

    int size = read(fd, input, st.st_size);
    if (size != st.st_size) {
        perror(argv[1]);
        ret = 5;
        goto done;
    }
    close(fd);
    fd = -1;

    parser = Parser_new(&test_cbyte_byte_code);
    int result = Parser_parse(parser, input, size);

    if (result >= 0) {
        if (result == size) {
            printf("parse ok.\n");
        }
        else {
            printf("parse succeeded but consumed %d bytes out of %d\n", result, size);
        }
        Parser_print_tree(parser, input);
        ret = 0;
    }
    else {
        printf("parse failed with result: %d\n", result);
        Parser_print_error(parser, input);
        ret = 1;
    }

done:

    if (parser) {
        Parser_free(parser);
    }
    if (fd >= 0) {
        close(fd);
    }
    if (input) {
        free(input);
    }

#if DEBUG_MEM
    muntrace();
#endif

    return ret;
}
