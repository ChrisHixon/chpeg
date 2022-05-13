// test to see if internal compiler byte code matches byte code compiled from a file
// originally used to compare the Ruby generated byte code with byte code created with C version

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

#include "mem.h"
#include "parser.h"
#include "compiler.h"

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

    input = (unsigned char*)CHPEG_MALLOC(st.st_size);
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

    ret = ByteCode_compare(Compiler_bytecode(), byte_code);
    printf("ByteCode_compare(Compiler_bytecode(), byte_code) = %d\n", ret);

done:

    if (byte_code) {
        ByteCode_free(byte_code);
    }
    if (input) {
        CHPEG_FREE(input);
    }
    if (fd >= 0) {
        close(fd);
    }

#if DEBUG_MEM
    muntrace();
#endif

    return ret;
}
