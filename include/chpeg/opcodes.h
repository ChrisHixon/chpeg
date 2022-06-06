//
// chpeg: opcodes.h {
//

#ifndef CHPEG_OPCODES_H
#define CHPEG_OPCODES_H

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_api.h"
#endif

enum ChpegOp
{
    CHPEG_OP_GOTO,
    CHPEG_OP_IDENT,
    CHPEG_OP_ISUCC,
    CHPEG_OP_IFAIL,
    CHPEG_OP_RSBEG,
    CHPEG_OP_RQBEG,
    CHPEG_OP_CHOICE,
    CHPEG_OP_CISUCC,
    CHPEG_OP_CFAIL,
    CHPEG_OP_CIFAIL,
    CHPEG_OP_RPBEG,
    CHPEG_OP_RPMAT,
    CHPEG_OP_RPDONE,
    CHPEG_OP_RSMAT,
    CHPEG_OP_RSDONE,
    CHPEG_OP_RQDONE,
    CHPEG_OP_RQMAT,
    CHPEG_OP_PREDA,
    CHPEG_OP_PREDN,
    CHPEG_OP_PMATCHF,
    CHPEG_OP_PNOMATF,
    CHPEG_OP_PMATCHS,
    CHPEG_OP_PNOMATS,
    CHPEG_OP_CHRCLS,
    CHPEG_OP_LIT,
    CHPEG_OP_DOT,
    CHPEG_OP_SUCC,
    CHPEG_OP_FAIL,

#ifdef CHPEG_EXTENSION_TRIM
    CHPEG_OP_TRIM,
    CHPEG_OP_TRIMS,
    CHPEG_OP_TRIMF,
#endif

#ifdef CHPEG_EXTENSION_REFS
    CHPEG_OP_MARK,
    CHPEG_OP_MARKS,
    CHPEG_OP_MARKF,
    CHPEG_OP_REF,
#endif

    CHPEG_NUM_OPS
};

CHPEG_API const char *Chpeg_op_names[CHPEG_NUM_OPS];

static inline const char *Chpeg_op_name(int op)
{
    return (op >= 0 && op < CHPEG_NUM_OPS) ? Chpeg_op_names[op] : "N/A";
}

#endif // #ifndef CHPEG_OPCODES_H

// } chpeg: opcodes.h

