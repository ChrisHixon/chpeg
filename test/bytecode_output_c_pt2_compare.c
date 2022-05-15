#include <stdio.h>

#include "chpeg_bytecode.h"
#include "generated/test_bytecode.h"

// compare chpeg_bytecode with generated test_bytecode
int main(void)
{
    return ChpegByteCode_compare(&chpeg_bytecode, &test_bytecode);
}

