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

#include "peg_cbyte_parser.h"
#include "peg_cbyte_compiler.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd = -1;
    unsigned char *input = NULL; 
    ByteCode *byte_code = NULL; 

#if DEBUG_MEM
    mtrace();
#endif

    if (argc != 2) {
        printf("usage: %s <grammar_file>\n", argv[0]);
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

    byte_code = Compiler_compile(input, st.st_size, NULL, 0);
    ByteCode_print(byte_code);

    int cmp = ByteCode_compare(&peg_byte_code, byte_code);
    printf("ByteCode_compare(&peg_byte_code, byte_code) = %d\n", cmp);

    ret = 0;
done:

    if (byte_code) {
        ByteCode_free(byte_code);
    }
    if (input) {
        free(input);
    }
    if (fd >= 0) {
        close(fd);
    }

#if DEBUG_MEM
    muntrace();
#endif

    return ret;
}
