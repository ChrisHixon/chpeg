#include <stdio.h>

#if USE_EXTENSIONS
#include "chpeg/chpeg_ext_bytecode.h"
#else
#include "chpeg/chpeg_bytecode.h"
#endif

// output default bytecode as test_bytecode.c & test_bytecode.h files

int main(void)
{
    FILE *fp;

    fp = fopen("generated/test_bytecode.c", "w");
    if (!fp) {
        perror("test_bytecode.c");
        return 1;
    }
    ChpegByteCode_output_c(
#if USE_EXTENSIONS
        &chpeg_ext_bytecode,
#else
        &chpeg_bytecode,
#endif
        fp, "test_bytecode", NULL, "test", NULL);
    fclose(fp);

    fp = fopen("generated/test_bytecode.h", "w");
    if (!fp) {
        perror("test_bytecode.h");
        return 1;
    }
    ChpegByteCode_output_h(
#if USE_EXTENSIONS
        &chpeg_ext_bytecode,
#else
        &chpeg_bytecode,
#endif
        fp, "test_bytecode", NULL, "test", NULL);
    fclose(fp);
}

