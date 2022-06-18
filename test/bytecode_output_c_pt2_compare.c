#include <stdio.h>

#if USE_EXTENSIONS
#include "chpeg/chpeg_ext_bytecode.h"
#else
#include "chpeg/chpeg_bytecode.h"
#endif

#include "generated/test_bytecode.h"

// compare chpeg_bytecode with generated test_bytecode
int main(void)
{
    return ChpegByteCode_compare(
#if USE_EXTENSIONS
        &chpeg_ext_bytecode,
#else
        &chpeg_bytecode,
#endif
        &test_bytecode);
}

