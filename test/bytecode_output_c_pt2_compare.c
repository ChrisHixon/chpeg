#include <stdio.h>

#if CHPEG_USES_EXTENSIONS
#include "chpeg/chpeg_ext_bytecode.h"
#else
#include "chpeg/chpeg_bytecode.h"
#endif

#include "generated/test_bytecode.h"

// compare chpeg_bytecode with generated test_bytecode
int main(void)
{
    return ChpegByteCode_compare(&chpeg_bytecode, &test_bytecode);
}

