//
// chpeg: opcodes.c {
//

#ifndef CHPEG_AMALGAMATION
#include "chpeg/opcodes.h"
#endif

CHPEG_DEF const char *Chpeg_op_names[CHPEG_NUM_OPS] =
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

#ifdef CHPEG_EXTENSION_TRIM
    "TRIM",
    "TRIMS",
    "TRIMF",
#endif

#ifdef CHPEG_EXTENSION_REFS
    "RSCOPE",
    "RSCOPES",
    "RSCOPEF",
    "MARK",
    "MARKS",
    "MARKF",
    "REF",
#endif

};

// } chpeg: opcodes.c

