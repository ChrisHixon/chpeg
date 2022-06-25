#ifndef CHPEG_CHPEG_EXT_BYTECODE_C
#define CHPEG_CHPEG_EXT_BYTECODE_C

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

const char *chpeg_ext_bytecode_def_names[29] = {"Grammar", "Definition", "Choice", "Sequence", "Element", "Predicate", "Repeat", "Primary", "Mark", "Reference", "Trim", "MinMax", "MinMaxVal", "Options", "Identifier", "Literal", "CharClass", "NCharClass", "CharRange", "Char", "EscChar", "OctChar", "HexChar", "PlainChar", "PredOp", "RepOp", "Dot", "Comma", "S"};

int chpeg_ext_bytecode_def_flags[29] = {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_LEAF, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE};

int chpeg_ext_bytecode_def_addrs[29] = {3, 16, 43, 56, 63, 75, 86, 115, 187, 201, 212, 224, 262, 279, 286, 295, 331, 352, 368, 389, 409, 415, 440, 451, 459, 463, 467, 471, 475};

int chpeg_ext_bytecode_instructions[507] = {
  /*      0 */ CHPEG_INST(CHPEG_OP_IDENT,           0), // Grammar
  /*      1 */ CHPEG_INST(CHPEG_OP_FAIL,            0),
  /*      2 */ CHPEG_INST(CHPEG_OP_SUCC,            0),
  /*      3 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*      4 */ CHPEG_INST(CHPEG_OP_GOTO,           15),
  /*      5 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*      6 */ CHPEG_INST(CHPEG_OP_IDENT,           1), // Definition
  /*      7 */ CHPEG_INST(CHPEG_OP_GOTO,            9),
  /*      8 */ CHPEG_INST(CHPEG_OP_RPMAT,           6),
  /*      9 */ CHPEG_INST(CHPEG_OP_RPDONE,         15),
  /*     10 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*     11 */ CHPEG_INST(CHPEG_OP_DOT,            13),
  /*     12 */ CHPEG_INST(CHPEG_OP_PMATCHF,        15),
  /*     13 */ CHPEG_INST(CHPEG_OP_PNOMATS,        15),
  /*     14 */ CHPEG_INST(CHPEG_OP_ISUCC,           0), // Grammar
  /*     15 */ CHPEG_INST(CHPEG_OP_IFAIL,           0), // Grammar
  /*     16 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // Identifier
  /*     17 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Options
  /*     26 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     27 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     28 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     29 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*     30 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     31 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     32 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     33 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     34 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     35 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*     36 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     37 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     38 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     39 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*     40 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     41 */ CHPEG_INST(CHPEG_OP_ISUCC,           1), // Definition
  /*     42 */ CHPEG_INST(CHPEG_OP_IFAIL,           1), // Definition
  /*     43 */ CHPEG_INST(CHPEG_OP_IDENT,           3), // Sequence
  /*     44 */ CHPEG_INST(CHPEG_OP_GOTO,           55),
  /*     45 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*     46 */ CHPEG_INST(CHPEG_OP_LIT,             3), // "/"
  /*     47 */ CHPEG_INST(CHPEG_OP_GOTO,           53),
  /*     48 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     49 */ CHPEG_INST(CHPEG_OP_GOTO,           53),
  /*     50 */ CHPEG_INST(CHPEG_OP_IDENT,           3), // Sequence
  /*     51 */ CHPEG_INST(CHPEG_OP_GOTO,           53),
  /*     52 */ CHPEG_INST(CHPEG_OP_RSMAT,          46),
  /*     53 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*     54 */ CHPEG_INST(CHPEG_OP_ISUCC,           2), // Choice
  /*     55 */ CHPEG_INST(CHPEG_OP_IFAIL,           2), // Choice
  /*     56 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*     57 */ CHPEG_INST(CHPEG_OP_IDENT,           4), // Element
  /*     58 */ CHPEG_INST(CHPEG_OP_GOTO,           60),
  /*     59 */ CHPEG_INST(CHPEG_OP_RPMAT,          57),
  /*     60 */ CHPEG_INST(CHPEG_OP_RPDONE,         62),
  /*     61 */ CHPEG_INST(CHPEG_OP_ISUCC,           3), // Sequence
  /*     62 */ CHPEG_INST(CHPEG_OP_IFAIL,           3), // Sequence
  /*     63 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*     64 */ CHPEG_INST(CHPEG_OP_IDENT,           5), // Predicate
  /*     65 */ CHPEG_INST(CHPEG_OP_GOTO,           67),
  /*     66 */ CHPEG_INST(CHPEG_OP_CISUCC,         73),
  /*     67 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     68 */ CHPEG_INST(CHPEG_OP_IDENT,          10), // Trim
  /*     69 */ CHPEG_INST(CHPEG_OP_GOTO,           71),
  /*     70 */ CHPEG_INST(CHPEG_OP_CISUCC,         73),
  /*     71 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     72 */ CHPEG_INST(CHPEG_OP_CFAIL,          74),
  /*     73 */ CHPEG_INST(CHPEG_OP_ISUCC,           4), // Element
  /*     74 */ CHPEG_INST(CHPEG_OP_IFAIL,           4), // Element
  /*     75 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     76 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // PredOp
  /*     77 */ CHPEG_INST(CHPEG_OP_GOTO,           81),
  /*     78 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     79 */ CHPEG_INST(CHPEG_OP_GOTO,           81),
  /*     80 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     81 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     82 */ CHPEG_INST(CHPEG_OP_IDENT,           6), // Repeat
  /*     83 */ CHPEG_INST(CHPEG_OP_GOTO,           85),
  /*     84 */ CHPEG_INST(CHPEG_OP_ISUCC,           5), // Predicate
  /*     85 */ CHPEG_INST(CHPEG_OP_IFAIL,           5), // Predicate
  /*     86 */ CHPEG_INST(CHPEG_OP_IDENT,           7), // Primary
  /*     87 */ CHPEG_INST(CHPEG_OP_GOTO,          114),
  /*     88 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     89 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*     90 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // RepOp
  /*     91 */ CHPEG_INST(CHPEG_OP_GOTO,           95),
  /*     92 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     93 */ CHPEG_INST(CHPEG_OP_GOTO,           95),
  /*     94 */ CHPEG_INST(CHPEG_OP_CISUCC,        111),
  /*     95 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     96 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     97 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*     98 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*     99 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*    100 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // MinMax
  /*    101 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*    102 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    103 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*    104 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*    105 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*    106 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    107 */ CHPEG_INST(CHPEG_OP_GOTO,          109),
  /*    108 */ CHPEG_INST(CHPEG_OP_CISUCC,        111),
  /*    109 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    110 */ CHPEG_INST(CHPEG_OP_CFAIL,         112),
  /*    111 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    112 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    113 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Repeat
  /*    114 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Repeat
  /*    115 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    116 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // Identifier
  /*    117 */ CHPEG_INST(CHPEG_OP_GOTO,          135),
  /*    118 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    119 */ CHPEG_INST(CHPEG_OP_GOTO,          135),
  /*    120 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    121 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*    122 */ CHPEG_INST(CHPEG_OP_GOTO,          128),
  /*    123 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    124 */ CHPEG_INST(CHPEG_OP_GOTO,          128),
  /*    125 */ CHPEG_INST(CHPEG_OP_CHRCLS,          4), // [A-Z]
  /*    126 */ CHPEG_INST(CHPEG_OP_GOTO,          128),
  /*    127 */ CHPEG_INST(CHPEG_OP_PMATCHF,       135),
  /*    128 */ CHPEG_INST(CHPEG_OP_PNOMATS,       135),
  /*    129 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    130 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*    131 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    132 */ CHPEG_INST(CHPEG_OP_PMATCHF,       135),
  /*    133 */ CHPEG_INST(CHPEG_OP_PNOMATS,       135),
  /*    134 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    135 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    136 */ CHPEG_INST(CHPEG_OP_LIT,             5), // "("
  /*    137 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    138 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    139 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    140 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    141 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    142 */ CHPEG_INST(CHPEG_OP_LIT,             6), // ")"
  /*    143 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    144 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    145 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    146 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    147 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    148 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Reference
  /*    149 */ CHPEG_INST(CHPEG_OP_GOTO,          153),
  /*    150 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    151 */ CHPEG_INST(CHPEG_OP_GOTO,          153),
  /*    152 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    153 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    154 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Mark
  /*    155 */ CHPEG_INST(CHPEG_OP_GOTO,          159),
  /*    156 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    157 */ CHPEG_INST(CHPEG_OP_GOTO,          159),
  /*    158 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    159 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    160 */ CHPEG_INST(CHPEG_OP_IDENT,          15), // Literal
  /*    161 */ CHPEG_INST(CHPEG_OP_GOTO,          165),
  /*    162 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    163 */ CHPEG_INST(CHPEG_OP_GOTO,          165),
  /*    164 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    165 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    166 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // CharClass
  /*    167 */ CHPEG_INST(CHPEG_OP_GOTO,          171),
  /*    168 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    169 */ CHPEG_INST(CHPEG_OP_GOTO,          171),
  /*    170 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    171 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    172 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // NCharClass
  /*    173 */ CHPEG_INST(CHPEG_OP_GOTO,          177),
  /*    174 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    175 */ CHPEG_INST(CHPEG_OP_GOTO,          177),
  /*    176 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    177 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    178 */ CHPEG_INST(CHPEG_OP_IDENT,          26), // Dot
  /*    179 */ CHPEG_INST(CHPEG_OP_GOTO,          183),
  /*    180 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    181 */ CHPEG_INST(CHPEG_OP_GOTO,          183),
  /*    182 */ CHPEG_INST(CHPEG_OP_CISUCC,        185),
  /*    183 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    184 */ CHPEG_INST(CHPEG_OP_CFAIL,         186),
  /*    185 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Primary
  /*    186 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Primary
  /*    187 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "$"
  /*    188 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    189 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // Identifier
  /*    190 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    191 */ CHPEG_INST(CHPEG_OP_LIT,             8), // "<"
  /*    192 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    193 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    194 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    195 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    196 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    197 */ CHPEG_INST(CHPEG_OP_LIT,             9), // ">"
  /*    198 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    199 */ CHPEG_INST(CHPEG_OP_ISUCC,           8), // Mark
  /*    200 */ CHPEG_INST(CHPEG_OP_IFAIL,           8), // Mark
  /*    201 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "$"
  /*    202 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    203 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // Identifier
  /*    204 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    205 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    206 */ CHPEG_INST(CHPEG_OP_LIT,             8), // "<"
  /*    207 */ CHPEG_INST(CHPEG_OP_GOTO,          209),
  /*    208 */ CHPEG_INST(CHPEG_OP_PMATCHF,       211),
  /*    209 */ CHPEG_INST(CHPEG_OP_PNOMATS,       211),
  /*    210 */ CHPEG_INST(CHPEG_OP_ISUCC,           9), // Reference
  /*    211 */ CHPEG_INST(CHPEG_OP_IFAIL,           9), // Reference
  /*    212 */ CHPEG_INST(CHPEG_OP_LIT,             8), // "<"
  /*    213 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    214 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    215 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    216 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    217 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    218 */ CHPEG_INST(CHPEG_OP_LIT,             9), // ">"
  /*    219 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    220 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    221 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    222 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // Trim
  /*    223 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // Trim
  /*    224 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    225 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    226 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    227 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    228 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    229 */ CHPEG_INST(CHPEG_OP_IDENT,          27), // Comma
  /*    230 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    231 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    232 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    233 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    234 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    235 */ CHPEG_INST(CHPEG_OP_CISUCC,        260),
  /*    236 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    237 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    238 */ CHPEG_INST(CHPEG_OP_GOTO,          246),
  /*    239 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    240 */ CHPEG_INST(CHPEG_OP_GOTO,          246),
  /*    241 */ CHPEG_INST(CHPEG_OP_IDENT,          27), // Comma
  /*    242 */ CHPEG_INST(CHPEG_OP_GOTO,          246),
  /*    243 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    244 */ CHPEG_INST(CHPEG_OP_GOTO,          246),
  /*    245 */ CHPEG_INST(CHPEG_OP_CISUCC,        260),
  /*    246 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    247 */ CHPEG_INST(CHPEG_OP_IDENT,          27), // Comma
  /*    248 */ CHPEG_INST(CHPEG_OP_GOTO,          254),
  /*    249 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    250 */ CHPEG_INST(CHPEG_OP_GOTO,          254),
  /*    251 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    252 */ CHPEG_INST(CHPEG_OP_GOTO,          254),
  /*    253 */ CHPEG_INST(CHPEG_OP_CISUCC,        260),
  /*    254 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    255 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    256 */ CHPEG_INST(CHPEG_OP_GOTO,          258),
  /*    257 */ CHPEG_INST(CHPEG_OP_CISUCC,        260),
  /*    258 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    259 */ CHPEG_INST(CHPEG_OP_CFAIL,         261),
  /*    260 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // MinMax
  /*    261 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // MinMax
  /*    262 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    263 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [1-9]
  /*    264 */ CHPEG_INST(CHPEG_OP_GOTO,          271),
  /*    265 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    266 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [0-9]
  /*    267 */ CHPEG_INST(CHPEG_OP_GOTO,          269),
  /*    268 */ CHPEG_INST(CHPEG_OP_RSMAT,         266),
  /*    269 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    270 */ CHPEG_INST(CHPEG_OP_CISUCC,        277),
  /*    271 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    272 */ CHPEG_INST(CHPEG_OP_LIT,            12), // "0"
  /*    273 */ CHPEG_INST(CHPEG_OP_GOTO,          275),
  /*    274 */ CHPEG_INST(CHPEG_OP_CISUCC,        277),
  /*    275 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    276 */ CHPEG_INST(CHPEG_OP_CFAIL,         278),
  /*    277 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // MinMaxVal
  /*    278 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // MinMaxVal
  /*    279 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    280 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [CSILTPR]
  /*    281 */ CHPEG_INST(CHPEG_OP_GOTO,          283),
  /*    282 */ CHPEG_INST(CHPEG_OP_RPMAT,         280),
  /*    283 */ CHPEG_INST(CHPEG_OP_RPDONE,        285),
  /*    284 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // Options
  /*    285 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // Options
  /*    286 */ CHPEG_INST(CHPEG_OP_CHRCLS,         14), // [a-zA-Z_]
  /*    287 */ CHPEG_INST(CHPEG_OP_GOTO,          294),
  /*    288 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    289 */ CHPEG_INST(CHPEG_OP_CHRCLS,         15), // [a-zA-Z_0-9]
  /*    290 */ CHPEG_INST(CHPEG_OP_GOTO,          292),
  /*    291 */ CHPEG_INST(CHPEG_OP_RSMAT,         289),
  /*    292 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    293 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // Identifier
  /*    294 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // Identifier
  /*    295 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    296 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [']
  /*    297 */ CHPEG_INST(CHPEG_OP_GOTO,          311),
  /*    298 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    299 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    300 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [']
  /*    301 */ CHPEG_INST(CHPEG_OP_GOTO,          303),
  /*    302 */ CHPEG_INST(CHPEG_OP_PMATCHF,       307),
  /*    303 */ CHPEG_INST(CHPEG_OP_PNOMATS,       307),
  /*    304 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    305 */ CHPEG_INST(CHPEG_OP_GOTO,          307),
  /*    306 */ CHPEG_INST(CHPEG_OP_RSMAT,         299),
  /*    307 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    308 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [']
  /*    309 */ CHPEG_INST(CHPEG_OP_GOTO,          311),
  /*    310 */ CHPEG_INST(CHPEG_OP_CISUCC,        329),
  /*    311 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    312 */ CHPEG_INST(CHPEG_OP_CHRCLS,         17), // [\"]
  /*    313 */ CHPEG_INST(CHPEG_OP_GOTO,          327),
  /*    314 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    315 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    316 */ CHPEG_INST(CHPEG_OP_CHRCLS,         17), // [\"]
  /*    317 */ CHPEG_INST(CHPEG_OP_GOTO,          319),
  /*    318 */ CHPEG_INST(CHPEG_OP_PMATCHF,       323),
  /*    319 */ CHPEG_INST(CHPEG_OP_PNOMATS,       323),
  /*    320 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    321 */ CHPEG_INST(CHPEG_OP_GOTO,          323),
  /*    322 */ CHPEG_INST(CHPEG_OP_RSMAT,         315),
  /*    323 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    324 */ CHPEG_INST(CHPEG_OP_CHRCLS,         17), // [\"]
  /*    325 */ CHPEG_INST(CHPEG_OP_GOTO,          327),
  /*    326 */ CHPEG_INST(CHPEG_OP_CISUCC,        329),
  /*    327 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    328 */ CHPEG_INST(CHPEG_OP_CFAIL,         330),
  /*    329 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // Literal
  /*    330 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // Literal
  /*    331 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "["
  /*    332 */ CHPEG_INST(CHPEG_OP_GOTO,          351),
  /*    333 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    334 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "^"
  /*    335 */ CHPEG_INST(CHPEG_OP_GOTO,          337),
  /*    336 */ CHPEG_INST(CHPEG_OP_PMATCHF,       351),
  /*    337 */ CHPEG_INST(CHPEG_OP_PNOMATS,       351),
  /*    338 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    339 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    340 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "]"
  /*    341 */ CHPEG_INST(CHPEG_OP_GOTO,          343),
  /*    342 */ CHPEG_INST(CHPEG_OP_PMATCHF,       347),
  /*    343 */ CHPEG_INST(CHPEG_OP_PNOMATS,       347),
  /*    344 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // CharRange
  /*    345 */ CHPEG_INST(CHPEG_OP_GOTO,          347),
  /*    346 */ CHPEG_INST(CHPEG_OP_RPMAT,         339),
  /*    347 */ CHPEG_INST(CHPEG_OP_RPDONE,        351),
  /*    348 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "]"
  /*    349 */ CHPEG_INST(CHPEG_OP_GOTO,          351),
  /*    350 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // CharClass
  /*    351 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // CharClass
  /*    352 */ CHPEG_INST(CHPEG_OP_LIT,            21), // "[^"
  /*    353 */ CHPEG_INST(CHPEG_OP_GOTO,          367),
  /*    354 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    355 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    356 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "]"
  /*    357 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    358 */ CHPEG_INST(CHPEG_OP_PMATCHF,       363),
  /*    359 */ CHPEG_INST(CHPEG_OP_PNOMATS,       363),
  /*    360 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // CharRange
  /*    361 */ CHPEG_INST(CHPEG_OP_GOTO,          363),
  /*    362 */ CHPEG_INST(CHPEG_OP_RPMAT,         355),
  /*    363 */ CHPEG_INST(CHPEG_OP_RPDONE,        367),
  /*    364 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "]"
  /*    365 */ CHPEG_INST(CHPEG_OP_GOTO,          367),
  /*    366 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // NCharClass
  /*    367 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // NCharClass
  /*    368 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    369 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    370 */ CHPEG_INST(CHPEG_OP_GOTO,          381),
  /*    371 */ CHPEG_INST(CHPEG_OP_LIT,            22), // "-"
  /*    372 */ CHPEG_INST(CHPEG_OP_GOTO,          381),
  /*    373 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    374 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "]"
  /*    375 */ CHPEG_INST(CHPEG_OP_GOTO,          377),
  /*    376 */ CHPEG_INST(CHPEG_OP_PMATCHF,       381),
  /*    377 */ CHPEG_INST(CHPEG_OP_PNOMATS,       381),
  /*    378 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    379 */ CHPEG_INST(CHPEG_OP_GOTO,          381),
  /*    380 */ CHPEG_INST(CHPEG_OP_CISUCC,        387),
  /*    381 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    382 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    383 */ CHPEG_INST(CHPEG_OP_GOTO,          385),
  /*    384 */ CHPEG_INST(CHPEG_OP_CISUCC,        387),
  /*    385 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    386 */ CHPEG_INST(CHPEG_OP_CFAIL,         388),
  /*    387 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // CharRange
  /*    388 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // CharRange
  /*    389 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    390 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // PlainChar
  /*    391 */ CHPEG_INST(CHPEG_OP_GOTO,          393),
  /*    392 */ CHPEG_INST(CHPEG_OP_CISUCC,        407),
  /*    393 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    394 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // HexChar
  /*    395 */ CHPEG_INST(CHPEG_OP_GOTO,          397),
  /*    396 */ CHPEG_INST(CHPEG_OP_CISUCC,        407),
  /*    397 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    398 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // EscChar
  /*    399 */ CHPEG_INST(CHPEG_OP_GOTO,          401),
  /*    400 */ CHPEG_INST(CHPEG_OP_CISUCC,        407),
  /*    401 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    402 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // OctChar
  /*    403 */ CHPEG_INST(CHPEG_OP_GOTO,          405),
  /*    404 */ CHPEG_INST(CHPEG_OP_CISUCC,        407),
  /*    405 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    406 */ CHPEG_INST(CHPEG_OP_CFAIL,         408),
  /*    407 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // Char
  /*    408 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // Char
  /*    409 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "\\"
  /*    410 */ CHPEG_INST(CHPEG_OP_GOTO,          414),
  /*    411 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [ntr'\"[]\\fvab]
  /*    412 */ CHPEG_INST(CHPEG_OP_GOTO,          414),
  /*    413 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // EscChar
  /*    414 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // EscChar
  /*    415 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    416 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "\\"
  /*    417 */ CHPEG_INST(CHPEG_OP_GOTO,          425),
  /*    418 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [0-3]
  /*    419 */ CHPEG_INST(CHPEG_OP_GOTO,          425),
  /*    420 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [0-7]
  /*    421 */ CHPEG_INST(CHPEG_OP_GOTO,          425),
  /*    422 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [0-7]
  /*    423 */ CHPEG_INST(CHPEG_OP_GOTO,          425),
  /*    424 */ CHPEG_INST(CHPEG_OP_CISUCC,        438),
  /*    425 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    426 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "\\"
  /*    427 */ CHPEG_INST(CHPEG_OP_GOTO,          436),
  /*    428 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [0-7]
  /*    429 */ CHPEG_INST(CHPEG_OP_GOTO,          436),
  /*    430 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    431 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [0-7]
  /*    432 */ CHPEG_INST(CHPEG_OP_GOTO,          434),
  /*    433 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    434 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    435 */ CHPEG_INST(CHPEG_OP_CISUCC,        438),
  /*    436 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    437 */ CHPEG_INST(CHPEG_OP_CFAIL,         439),
  /*    438 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // OctChar
  /*    439 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // OctChar
  /*    440 */ CHPEG_INST(CHPEG_OP_LIT,            27), // "\\x"
  /*    441 */ CHPEG_INST(CHPEG_OP_GOTO,          450),
  /*    442 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [0-9a-fA-F]
  /*    443 */ CHPEG_INST(CHPEG_OP_GOTO,          450),
  /*    444 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    445 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [0-9a-fA-F]
  /*    446 */ CHPEG_INST(CHPEG_OP_GOTO,          448),
  /*    447 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    448 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    449 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // HexChar
  /*    450 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // HexChar
  /*    451 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    452 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "\\"
  /*    453 */ CHPEG_INST(CHPEG_OP_GOTO,          455),
  /*    454 */ CHPEG_INST(CHPEG_OP_PMATCHF,       458),
  /*    455 */ CHPEG_INST(CHPEG_OP_PNOMATS,       458),
  /*    456 */ CHPEG_INST(CHPEG_OP_DOT,           458),
  /*    457 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // PlainChar
  /*    458 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // PlainChar
  /*    459 */ CHPEG_INST(CHPEG_OP_CHRCLS,         29), // [&!]
  /*    460 */ CHPEG_INST(CHPEG_OP_GOTO,          462),
  /*    461 */ CHPEG_INST(CHPEG_OP_ISUCC,          24), // PredOp
  /*    462 */ CHPEG_INST(CHPEG_OP_IFAIL,          24), // PredOp
  /*    463 */ CHPEG_INST(CHPEG_OP_CHRCLS,         30), // [*+?]
  /*    464 */ CHPEG_INST(CHPEG_OP_GOTO,          466),
  /*    465 */ CHPEG_INST(CHPEG_OP_ISUCC,          25), // RepOp
  /*    466 */ CHPEG_INST(CHPEG_OP_IFAIL,          25), // RepOp
  /*    467 */ CHPEG_INST(CHPEG_OP_LIT,            31), // "."
  /*    468 */ CHPEG_INST(CHPEG_OP_GOTO,          470),
  /*    469 */ CHPEG_INST(CHPEG_OP_ISUCC,          26), // Dot
  /*    470 */ CHPEG_INST(CHPEG_OP_IFAIL,          26), // Dot
  /*    471 */ CHPEG_INST(CHPEG_OP_LIT,            32), // ","
  /*    472 */ CHPEG_INST(CHPEG_OP_GOTO,          474),
  /*    473 */ CHPEG_INST(CHPEG_OP_ISUCC,          27), // Comma
  /*    474 */ CHPEG_INST(CHPEG_OP_IFAIL,          27), // Comma
  /*    475 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    476 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    477 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    478 */ CHPEG_INST(CHPEG_OP_CHRCLS,         33), // [ \t\r\n]
  /*    479 */ CHPEG_INST(CHPEG_OP_GOTO,          481),
  /*    480 */ CHPEG_INST(CHPEG_OP_RPMAT,         478),
  /*    481 */ CHPEG_INST(CHPEG_OP_RPDONE,        483),
  /*    482 */ CHPEG_INST(CHPEG_OP_CISUCC,        503),
  /*    483 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    484 */ CHPEG_INST(CHPEG_OP_LIT,            34), // "#"
  /*    485 */ CHPEG_INST(CHPEG_OP_GOTO,          501),
  /*    486 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    487 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    488 */ CHPEG_INST(CHPEG_OP_CHRCLS,         35), // [\r\n]
  /*    489 */ CHPEG_INST(CHPEG_OP_GOTO,          491),
  /*    490 */ CHPEG_INST(CHPEG_OP_PMATCHF,       494),
  /*    491 */ CHPEG_INST(CHPEG_OP_PNOMATS,       494),
  /*    492 */ CHPEG_INST(CHPEG_OP_DOT,           494),
  /*    493 */ CHPEG_INST(CHPEG_OP_RSMAT,         487),
  /*    494 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    495 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    496 */ CHPEG_INST(CHPEG_OP_CHRCLS,         35), // [\r\n]
  /*    497 */ CHPEG_INST(CHPEG_OP_GOTO,          499),
  /*    498 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    499 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    500 */ CHPEG_INST(CHPEG_OP_CISUCC,        503),
  /*    501 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    502 */ CHPEG_INST(CHPEG_OP_CFAIL,         504),
  /*    503 */ CHPEG_INST(CHPEG_OP_RSMAT,         476),
  /*    504 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    505 */ CHPEG_INST(CHPEG_OP_ISUCC,          28), // S
  /*    506 */ CHPEG_INST(CHPEG_OP_IFAIL,          28), // S
  };

const char *chpeg_ext_bytecode_strings[36] = {"{", "}", "<-", "/", "A-Z", "(", ")", "$", "<", ">", "1-9", "0-9", "0", "CSILTPR", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "^", "]", "[^", "-", "\\", "ntr'\"[]\\fvab", "0-3", "0-7", "\\x", "0-9a-fA-F", "&!", "*+?", ".", ",", " \t\r\n", "#", "\r\n"};

int chpeg_ext_bytecode_str_len[36] = {1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 3, 3, 1, 7, 7, 10, 1, 1, 1, 1, 1, 2, 1, 1, 12, 3, 3, 2, 9, 2, 3, 1, 1, 4, 1, 2};

const ChpegByteCode chpeg_ext_bytecode = {
  29, // num_defs
  (char **)chpeg_ext_bytecode_def_names,
  chpeg_ext_bytecode_def_flags,
  chpeg_ext_bytecode_def_addrs,
  507, // num_instructions
  chpeg_ext_bytecode_instructions,
  36, // num_strings
  (unsigned char **)chpeg_ext_bytecode_strings,
  chpeg_ext_bytecode_str_len,
#if CHPEG_EXTENSION_REFS
  0, // num_refs
  NULL, // refs,
#endif
};

#ifdef __cplusplus
}
#endif

#endif // #ifndef CHPEG_CHPEG_EXT_BYTECODE_C
