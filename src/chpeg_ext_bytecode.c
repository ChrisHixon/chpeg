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

int chpeg_ext_bytecode_def_addrs[29] = {3, 16, 43, 56, 63, 75, 86, 115, 187, 201, 212, 224, 254, 271, 278, 287, 323, 344, 360, 381, 401, 407, 432, 443, 451, 455, 459, 463, 467};

int chpeg_ext_bytecode_instructions[499] = {
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
  /*    125 */ CHPEG_INST(CHPEG_OP_CHRCLS,          4), // [CSILTPR]
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
  /*    226 */ CHPEG_INST(CHPEG_OP_GOTO,          242),
  /*    227 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    228 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    229 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    230 */ CHPEG_INST(CHPEG_OP_IDENT,          27), // Comma
  /*    231 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    232 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    233 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    234 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    235 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    236 */ CHPEG_INST(CHPEG_OP_GOTO,          238),
  /*    237 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    238 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    239 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    240 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    241 */ CHPEG_INST(CHPEG_OP_CISUCC,        252),
  /*    242 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    243 */ CHPEG_INST(CHPEG_OP_IDENT,          27), // Comma
  /*    244 */ CHPEG_INST(CHPEG_OP_GOTO,          250),
  /*    245 */ CHPEG_INST(CHPEG_OP_IDENT,          28), // S
  /*    246 */ CHPEG_INST(CHPEG_OP_GOTO,          250),
  /*    247 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // MinMaxVal
  /*    248 */ CHPEG_INST(CHPEG_OP_GOTO,          250),
  /*    249 */ CHPEG_INST(CHPEG_OP_CISUCC,        252),
  /*    250 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    251 */ CHPEG_INST(CHPEG_OP_CFAIL,         253),
  /*    252 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // MinMax
  /*    253 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // MinMax
  /*    254 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    255 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [1-9]
  /*    256 */ CHPEG_INST(CHPEG_OP_GOTO,          263),
  /*    257 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    258 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [0-9]
  /*    259 */ CHPEG_INST(CHPEG_OP_GOTO,          261),
  /*    260 */ CHPEG_INST(CHPEG_OP_RSMAT,         258),
  /*    261 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    262 */ CHPEG_INST(CHPEG_OP_CISUCC,        269),
  /*    263 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    264 */ CHPEG_INST(CHPEG_OP_LIT,            12), // "0"
  /*    265 */ CHPEG_INST(CHPEG_OP_GOTO,          267),
  /*    266 */ CHPEG_INST(CHPEG_OP_CISUCC,        269),
  /*    267 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    268 */ CHPEG_INST(CHPEG_OP_CFAIL,         270),
  /*    269 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // MinMaxVal
  /*    270 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // MinMaxVal
  /*    271 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    272 */ CHPEG_INST(CHPEG_OP_CHRCLS,          4), // [CSILTPR]
  /*    273 */ CHPEG_INST(CHPEG_OP_GOTO,          275),
  /*    274 */ CHPEG_INST(CHPEG_OP_RPMAT,         272),
  /*    275 */ CHPEG_INST(CHPEG_OP_RPDONE,        277),
  /*    276 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // Options
  /*    277 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // Options
  /*    278 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [a-zA-Z_]
  /*    279 */ CHPEG_INST(CHPEG_OP_GOTO,          286),
  /*    280 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    281 */ CHPEG_INST(CHPEG_OP_CHRCLS,         14), // [a-zA-Z_0-9]
  /*    282 */ CHPEG_INST(CHPEG_OP_GOTO,          284),
  /*    283 */ CHPEG_INST(CHPEG_OP_RSMAT,         281),
  /*    284 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    285 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // Identifier
  /*    286 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // Identifier
  /*    287 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    288 */ CHPEG_INST(CHPEG_OP_CHRCLS,         15), // [']
  /*    289 */ CHPEG_INST(CHPEG_OP_GOTO,          303),
  /*    290 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    291 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    292 */ CHPEG_INST(CHPEG_OP_CHRCLS,         15), // [']
  /*    293 */ CHPEG_INST(CHPEG_OP_GOTO,          295),
  /*    294 */ CHPEG_INST(CHPEG_OP_PMATCHF,       299),
  /*    295 */ CHPEG_INST(CHPEG_OP_PNOMATS,       299),
  /*    296 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    297 */ CHPEG_INST(CHPEG_OP_GOTO,          299),
  /*    298 */ CHPEG_INST(CHPEG_OP_RSMAT,         291),
  /*    299 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    300 */ CHPEG_INST(CHPEG_OP_CHRCLS,         15), // [']
  /*    301 */ CHPEG_INST(CHPEG_OP_GOTO,          303),
  /*    302 */ CHPEG_INST(CHPEG_OP_CISUCC,        321),
  /*    303 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    304 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [\"]
  /*    305 */ CHPEG_INST(CHPEG_OP_GOTO,          319),
  /*    306 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    307 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    308 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [\"]
  /*    309 */ CHPEG_INST(CHPEG_OP_GOTO,          311),
  /*    310 */ CHPEG_INST(CHPEG_OP_PMATCHF,       315),
  /*    311 */ CHPEG_INST(CHPEG_OP_PNOMATS,       315),
  /*    312 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    313 */ CHPEG_INST(CHPEG_OP_GOTO,          315),
  /*    314 */ CHPEG_INST(CHPEG_OP_RSMAT,         307),
  /*    315 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    316 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [\"]
  /*    317 */ CHPEG_INST(CHPEG_OP_GOTO,          319),
  /*    318 */ CHPEG_INST(CHPEG_OP_CISUCC,        321),
  /*    319 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    320 */ CHPEG_INST(CHPEG_OP_CFAIL,         322),
  /*    321 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // Literal
  /*    322 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // Literal
  /*    323 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "["
  /*    324 */ CHPEG_INST(CHPEG_OP_GOTO,          343),
  /*    325 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    326 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "^"
  /*    327 */ CHPEG_INST(CHPEG_OP_GOTO,          329),
  /*    328 */ CHPEG_INST(CHPEG_OP_PMATCHF,       343),
  /*    329 */ CHPEG_INST(CHPEG_OP_PNOMATS,       343),
  /*    330 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    331 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    332 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "]"
  /*    333 */ CHPEG_INST(CHPEG_OP_GOTO,          335),
  /*    334 */ CHPEG_INST(CHPEG_OP_PMATCHF,       339),
  /*    335 */ CHPEG_INST(CHPEG_OP_PNOMATS,       339),
  /*    336 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // CharRange
  /*    337 */ CHPEG_INST(CHPEG_OP_GOTO,          339),
  /*    338 */ CHPEG_INST(CHPEG_OP_RPMAT,         331),
  /*    339 */ CHPEG_INST(CHPEG_OP_RPDONE,        343),
  /*    340 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "]"
  /*    341 */ CHPEG_INST(CHPEG_OP_GOTO,          343),
  /*    342 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // CharClass
  /*    343 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // CharClass
  /*    344 */ CHPEG_INST(CHPEG_OP_LIT,            20), // "[^"
  /*    345 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    346 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    347 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    348 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "]"
  /*    349 */ CHPEG_INST(CHPEG_OP_GOTO,          351),
  /*    350 */ CHPEG_INST(CHPEG_OP_PMATCHF,       355),
  /*    351 */ CHPEG_INST(CHPEG_OP_PNOMATS,       355),
  /*    352 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // CharRange
  /*    353 */ CHPEG_INST(CHPEG_OP_GOTO,          355),
  /*    354 */ CHPEG_INST(CHPEG_OP_RPMAT,         347),
  /*    355 */ CHPEG_INST(CHPEG_OP_RPDONE,        359),
  /*    356 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "]"
  /*    357 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    358 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // NCharClass
  /*    359 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // NCharClass
  /*    360 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    361 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    362 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    363 */ CHPEG_INST(CHPEG_OP_LIT,            21), // "-"
  /*    364 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    365 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    366 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "]"
  /*    367 */ CHPEG_INST(CHPEG_OP_GOTO,          369),
  /*    368 */ CHPEG_INST(CHPEG_OP_PMATCHF,       373),
  /*    369 */ CHPEG_INST(CHPEG_OP_PNOMATS,       373),
  /*    370 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    371 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    372 */ CHPEG_INST(CHPEG_OP_CISUCC,        379),
  /*    373 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    374 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // Char
  /*    375 */ CHPEG_INST(CHPEG_OP_GOTO,          377),
  /*    376 */ CHPEG_INST(CHPEG_OP_CISUCC,        379),
  /*    377 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    378 */ CHPEG_INST(CHPEG_OP_CFAIL,         380),
  /*    379 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // CharRange
  /*    380 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // CharRange
  /*    381 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    382 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // PlainChar
  /*    383 */ CHPEG_INST(CHPEG_OP_GOTO,          385),
  /*    384 */ CHPEG_INST(CHPEG_OP_CISUCC,        399),
  /*    385 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    386 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // HexChar
  /*    387 */ CHPEG_INST(CHPEG_OP_GOTO,          389),
  /*    388 */ CHPEG_INST(CHPEG_OP_CISUCC,        399),
  /*    389 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    390 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // EscChar
  /*    391 */ CHPEG_INST(CHPEG_OP_GOTO,          393),
  /*    392 */ CHPEG_INST(CHPEG_OP_CISUCC,        399),
  /*    393 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    394 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // OctChar
  /*    395 */ CHPEG_INST(CHPEG_OP_GOTO,          397),
  /*    396 */ CHPEG_INST(CHPEG_OP_CISUCC,        399),
  /*    397 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    398 */ CHPEG_INST(CHPEG_OP_CFAIL,         400),
  /*    399 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // Char
  /*    400 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // Char
  /*    401 */ CHPEG_INST(CHPEG_OP_LIT,            22), // "\\"
  /*    402 */ CHPEG_INST(CHPEG_OP_GOTO,          406),
  /*    403 */ CHPEG_INST(CHPEG_OP_CHRCLS,         23), // [ntr'\"[]\\fvab]
  /*    404 */ CHPEG_INST(CHPEG_OP_GOTO,          406),
  /*    405 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // EscChar
  /*    406 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // EscChar
  /*    407 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    408 */ CHPEG_INST(CHPEG_OP_LIT,            22), // "\\"
  /*    409 */ CHPEG_INST(CHPEG_OP_GOTO,          417),
  /*    410 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [0-3]
  /*    411 */ CHPEG_INST(CHPEG_OP_GOTO,          417),
  /*    412 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [0-7]
  /*    413 */ CHPEG_INST(CHPEG_OP_GOTO,          417),
  /*    414 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [0-7]
  /*    415 */ CHPEG_INST(CHPEG_OP_GOTO,          417),
  /*    416 */ CHPEG_INST(CHPEG_OP_CISUCC,        430),
  /*    417 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    418 */ CHPEG_INST(CHPEG_OP_LIT,            22), // "\\"
  /*    419 */ CHPEG_INST(CHPEG_OP_GOTO,          428),
  /*    420 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [0-7]
  /*    421 */ CHPEG_INST(CHPEG_OP_GOTO,          428),
  /*    422 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    423 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [0-7]
  /*    424 */ CHPEG_INST(CHPEG_OP_GOTO,          426),
  /*    425 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    426 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    427 */ CHPEG_INST(CHPEG_OP_CISUCC,        430),
  /*    428 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    429 */ CHPEG_INST(CHPEG_OP_CFAIL,         431),
  /*    430 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // OctChar
  /*    431 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // OctChar
  /*    432 */ CHPEG_INST(CHPEG_OP_LIT,            26), // "\\x"
  /*    433 */ CHPEG_INST(CHPEG_OP_GOTO,          442),
  /*    434 */ CHPEG_INST(CHPEG_OP_CHRCLS,         27), // [0-9a-fA-F]
  /*    435 */ CHPEG_INST(CHPEG_OP_GOTO,          442),
  /*    436 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    437 */ CHPEG_INST(CHPEG_OP_CHRCLS,         27), // [0-9a-fA-F]
  /*    438 */ CHPEG_INST(CHPEG_OP_GOTO,          440),
  /*    439 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    440 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    441 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // HexChar
  /*    442 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // HexChar
  /*    443 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    444 */ CHPEG_INST(CHPEG_OP_LIT,            22), // "\\"
  /*    445 */ CHPEG_INST(CHPEG_OP_GOTO,          447),
  /*    446 */ CHPEG_INST(CHPEG_OP_PMATCHF,       450),
  /*    447 */ CHPEG_INST(CHPEG_OP_PNOMATS,       450),
  /*    448 */ CHPEG_INST(CHPEG_OP_DOT,           450),
  /*    449 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // PlainChar
  /*    450 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // PlainChar
  /*    451 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [&!]
  /*    452 */ CHPEG_INST(CHPEG_OP_GOTO,          454),
  /*    453 */ CHPEG_INST(CHPEG_OP_ISUCC,          24), // PredOp
  /*    454 */ CHPEG_INST(CHPEG_OP_IFAIL,          24), // PredOp
  /*    455 */ CHPEG_INST(CHPEG_OP_CHRCLS,         29), // [*+?]
  /*    456 */ CHPEG_INST(CHPEG_OP_GOTO,          458),
  /*    457 */ CHPEG_INST(CHPEG_OP_ISUCC,          25), // RepOp
  /*    458 */ CHPEG_INST(CHPEG_OP_IFAIL,          25), // RepOp
  /*    459 */ CHPEG_INST(CHPEG_OP_LIT,            30), // "."
  /*    460 */ CHPEG_INST(CHPEG_OP_GOTO,          462),
  /*    461 */ CHPEG_INST(CHPEG_OP_ISUCC,          26), // Dot
  /*    462 */ CHPEG_INST(CHPEG_OP_IFAIL,          26), // Dot
  /*    463 */ CHPEG_INST(CHPEG_OP_LIT,            31), // ","
  /*    464 */ CHPEG_INST(CHPEG_OP_GOTO,          466),
  /*    465 */ CHPEG_INST(CHPEG_OP_ISUCC,          27), // Comma
  /*    466 */ CHPEG_INST(CHPEG_OP_IFAIL,          27), // Comma
  /*    467 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    468 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    469 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    470 */ CHPEG_INST(CHPEG_OP_CHRCLS,         32), // [ \t\r\n]
  /*    471 */ CHPEG_INST(CHPEG_OP_GOTO,          473),
  /*    472 */ CHPEG_INST(CHPEG_OP_RPMAT,         470),
  /*    473 */ CHPEG_INST(CHPEG_OP_RPDONE,        475),
  /*    474 */ CHPEG_INST(CHPEG_OP_CISUCC,        495),
  /*    475 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    476 */ CHPEG_INST(CHPEG_OP_LIT,            33), // "#"
  /*    477 */ CHPEG_INST(CHPEG_OP_GOTO,          493),
  /*    478 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    479 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    480 */ CHPEG_INST(CHPEG_OP_CHRCLS,         34), // [\r\n]
  /*    481 */ CHPEG_INST(CHPEG_OP_GOTO,          483),
  /*    482 */ CHPEG_INST(CHPEG_OP_PMATCHF,       486),
  /*    483 */ CHPEG_INST(CHPEG_OP_PNOMATS,       486),
  /*    484 */ CHPEG_INST(CHPEG_OP_DOT,           486),
  /*    485 */ CHPEG_INST(CHPEG_OP_RSMAT,         479),
  /*    486 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    487 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    488 */ CHPEG_INST(CHPEG_OP_CHRCLS,         34), // [\r\n]
  /*    489 */ CHPEG_INST(CHPEG_OP_GOTO,          491),
  /*    490 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    491 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    492 */ CHPEG_INST(CHPEG_OP_CISUCC,        495),
  /*    493 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    494 */ CHPEG_INST(CHPEG_OP_CFAIL,         496),
  /*    495 */ CHPEG_INST(CHPEG_OP_RSMAT,         468),
  /*    496 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    497 */ CHPEG_INST(CHPEG_OP_ISUCC,          28), // S
  /*    498 */ CHPEG_INST(CHPEG_OP_IFAIL,          28), // S
  };

const char *chpeg_ext_bytecode_strings[35] = {"{", "}", "<-", "/", "CSILTPR", "(", ")", "$", "<", ">", "1-9", "0-9", "0", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "^", "]", "[^", "-", "\\", "ntr'\"[]\\fvab", "0-3", "0-7", "\\x", "0-9a-fA-F", "&!", "*+?", ".", ",", " \t\r\n", "#", "\r\n"};

int chpeg_ext_bytecode_str_len[35] = {1, 1, 2, 1, 7, 1, 1, 1, 1, 1, 3, 3, 1, 7, 10, 1, 1, 1, 1, 1, 2, 1, 1, 12, 3, 3, 2, 9, 2, 3, 1, 1, 4, 1, 2};

const ChpegByteCode chpeg_ext_bytecode = {
  29, // num_defs
  (char **)chpeg_ext_bytecode_def_names,
  chpeg_ext_bytecode_def_flags,
  chpeg_ext_bytecode_def_addrs,
  499, // num_instructions
  chpeg_ext_bytecode_instructions,
  35, // num_strings
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
