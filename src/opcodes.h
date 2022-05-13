#ifndef CHPEG_OPCODES_H
#define CHPEG_OPCODES_H

enum OpCodes
{
    GOTO,
    IDENT,
    ISUCC,
    IFAIL,
    RSBEG,
    RQBEG,
    CHOICE,
    CISUCC,
    CFAIL,
    CIFAIL,
    RPBEG,
    RPMAT,
    RPDONE,
    RSMAT,
    RSDONE,
    RQDONE,
    RQMAT,
    PRED,
    PMATCHF,
    PNOMATF,
    PMATCHS,
    PNOMATS,
    CHRCLS,
    LIT,
    DOT,
    SUCC,
    FAIL,
    NUM_OPS
};

extern const char *OpNames[NUM_OPS];

static inline const char *op_name(int op)
{
    return (op >= 0 && op < NUM_OPS) ? OpNames[op] : "N/A";
}

#endif // #ifndef CHPEG_OPCODES_H
