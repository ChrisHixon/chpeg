#include <stdio.h>

#include "chpeg_bytecode.h"

// output chpeg_bytecode as test_bytecode.c & test_bytecode.h files

int main(void)
{
    FILE *fp;

    fp = fopen("generated/test_bytecode.c", "w");
    if (!fp) {
        perror("test_bytecode.c");
        return 1;
    }
    ChpegByteCode_output_c(&chpeg_bytecode, fp, "test_bytecode", NULL);
    fclose(fp);

    fp = fopen("generated/test_bytecode.h", "w");
    if (!fp) {
        perror("test_bytecode.h");
        return 1;
    }
    ChpegByteCode_output_h(&chpeg_bytecode, fp, "test_bytecode", NULL, "chpeg", NULL);
    fclose(fp);
}

