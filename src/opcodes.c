//
// chpeg: opcodes.c {
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/opcodes.h"
#endif

const char *Chpeg_op_names[CHPEG_NUM_OPS] =
{
    "GOTO",
    "IDENT",
    "ISUCC",
    "IFAIL",
    "RSBEG",
    "RQBEG",
    "CHOICE",
    "CISUCC",
    "CFAIL",
    "CIFAIL",
    "RPBEG",
    "RPMAT",
    "RPDONE",
    "RSMAT",
    "RSDONE",
    "RQDONE",
    "RQMAT",
    "PREDA",
    "PREDN",
    "PMATCHF",
    "PNOMATF",
    "PMATCHS",
    "PNOMATS",
    "CHRCLS",
    "LIT",
    "DOT",
    "SUCC",
    "FAIL",

#if CHPEG_EXTENSION_TRIM
    "TRIM",
    "TRIMS",
    "TRIMF",
#endif

#if CHPEG_EXTENSION_REFS
    "MARK",
    "MARKS",
    "MARKF",
    "REF",
#endif

#if CHPEG_EXTENSION_NCHRCLS
    "NCHRCLS",
#endif

#if CHPEG_EXTENSION_MINMAX
    "RMMBEG",
    "RMMMAT",
    "RMMDONE",
#endif

};

#ifdef __cplusplus
} // extern "C"
#endif

// } chpeg: opcodes.c

