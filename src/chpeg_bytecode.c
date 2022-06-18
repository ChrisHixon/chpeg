#ifndef CHPEG_CHPEG_BYTECODE_C
#define CHPEG_CHPEG_BYTECODE_C

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_bytecode.h"
#endif

const char *chpeg_bytecode_def_names[20] = {"Grammar", "Definition", "Choice", "Sequence", "Predicate", "Repeat", "Primary", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"};

int chpeg_bytecode_def_flags[20] = {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE};

int chpeg_bytecode_def_addrs[20] = {3, 16, 43, 56, 63, 74, 85, 130, 137, 146, 182, 198, 219, 235, 241, 266, 274, 278, 282, 286};

int chpeg_bytecode_instructions[318] = {
  /*      0 */ CHPEG_INST(CHPEG_OP_IDENT,           0), // Grammar
  /*      1 */ CHPEG_INST(CHPEG_OP_FAIL,            0),
  /*      2 */ CHPEG_INST(CHPEG_OP_SUCC,            0),
  /*      3 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
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
  /*     16 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Identifier
  /*     17 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,           7), // Options
  /*     26 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     27 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     28 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     29 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*     30 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     31 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     32 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     33 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     34 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     35 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*     36 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     37 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
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
  /*     48 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     49 */ CHPEG_INST(CHPEG_OP_GOTO,           53),
  /*     50 */ CHPEG_INST(CHPEG_OP_IDENT,           3), // Sequence
  /*     51 */ CHPEG_INST(CHPEG_OP_GOTO,           53),
  /*     52 */ CHPEG_INST(CHPEG_OP_RSMAT,          46),
  /*     53 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*     54 */ CHPEG_INST(CHPEG_OP_ISUCC,           2), // Choice
  /*     55 */ CHPEG_INST(CHPEG_OP_IFAIL,           2), // Choice
  /*     56 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*     57 */ CHPEG_INST(CHPEG_OP_IDENT,           4), // Predicate
  /*     58 */ CHPEG_INST(CHPEG_OP_GOTO,           60),
  /*     59 */ CHPEG_INST(CHPEG_OP_RPMAT,          57),
  /*     60 */ CHPEG_INST(CHPEG_OP_RPDONE,         62),
  /*     61 */ CHPEG_INST(CHPEG_OP_ISUCC,           3), // Sequence
  /*     62 */ CHPEG_INST(CHPEG_OP_IFAIL,           3), // Sequence
  /*     63 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     64 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // PredOp
  /*     65 */ CHPEG_INST(CHPEG_OP_GOTO,           69),
  /*     66 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     67 */ CHPEG_INST(CHPEG_OP_GOTO,           69),
  /*     68 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     69 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     70 */ CHPEG_INST(CHPEG_OP_IDENT,           5), // Repeat
  /*     71 */ CHPEG_INST(CHPEG_OP_GOTO,           73),
  /*     72 */ CHPEG_INST(CHPEG_OP_ISUCC,           4), // Predicate
  /*     73 */ CHPEG_INST(CHPEG_OP_IFAIL,           4), // Predicate
  /*     74 */ CHPEG_INST(CHPEG_OP_IDENT,           6), // Primary
  /*     75 */ CHPEG_INST(CHPEG_OP_GOTO,           84),
  /*     76 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     77 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // RepOp
  /*     78 */ CHPEG_INST(CHPEG_OP_GOTO,           82),
  /*     79 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     80 */ CHPEG_INST(CHPEG_OP_GOTO,           82),
  /*     81 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     82 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     83 */ CHPEG_INST(CHPEG_OP_ISUCC,           5), // Repeat
  /*     84 */ CHPEG_INST(CHPEG_OP_IFAIL,           5), // Repeat
  /*     85 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*     86 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Identifier
  /*     87 */ CHPEG_INST(CHPEG_OP_GOTO,           96),
  /*     88 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*     89 */ CHPEG_INST(CHPEG_OP_GOTO,           96),
  /*     90 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*     91 */ CHPEG_INST(CHPEG_OP_CHRCLS,          4), // [{<]
  /*     92 */ CHPEG_INST(CHPEG_OP_GOTO,           94),
  /*     93 */ CHPEG_INST(CHPEG_OP_PMATCHF,        96),
  /*     94 */ CHPEG_INST(CHPEG_OP_PNOMATS,        96),
  /*     95 */ CHPEG_INST(CHPEG_OP_CISUCC,        128),
  /*     96 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     97 */ CHPEG_INST(CHPEG_OP_LIT,             5), // "("
  /*     98 */ CHPEG_INST(CHPEG_OP_GOTO,          108),
  /*     99 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*    100 */ CHPEG_INST(CHPEG_OP_GOTO,          108),
  /*    101 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    102 */ CHPEG_INST(CHPEG_OP_GOTO,          108),
  /*    103 */ CHPEG_INST(CHPEG_OP_LIT,             6), // ")"
  /*    104 */ CHPEG_INST(CHPEG_OP_GOTO,          108),
  /*    105 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*    106 */ CHPEG_INST(CHPEG_OP_GOTO,          108),
  /*    107 */ CHPEG_INST(CHPEG_OP_CISUCC,        128),
  /*    108 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    109 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Literal
  /*    110 */ CHPEG_INST(CHPEG_OP_GOTO,          114),
  /*    111 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*    112 */ CHPEG_INST(CHPEG_OP_GOTO,          114),
  /*    113 */ CHPEG_INST(CHPEG_OP_CISUCC,        128),
  /*    114 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    115 */ CHPEG_INST(CHPEG_OP_IDENT,          10), // CharClass
  /*    116 */ CHPEG_INST(CHPEG_OP_GOTO,          120),
  /*    117 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*    118 */ CHPEG_INST(CHPEG_OP_GOTO,          120),
  /*    119 */ CHPEG_INST(CHPEG_OP_CISUCC,        128),
  /*    120 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    121 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // Dot
  /*    122 */ CHPEG_INST(CHPEG_OP_GOTO,          126),
  /*    123 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // S
  /*    124 */ CHPEG_INST(CHPEG_OP_GOTO,          126),
  /*    125 */ CHPEG_INST(CHPEG_OP_CISUCC,        128),
  /*    126 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    127 */ CHPEG_INST(CHPEG_OP_CFAIL,         129),
  /*    128 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Primary
  /*    129 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Primary
  /*    130 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    131 */ CHPEG_INST(CHPEG_OP_CHRCLS,          7), // [A-Za-z]
  /*    132 */ CHPEG_INST(CHPEG_OP_GOTO,          134),
  /*    133 */ CHPEG_INST(CHPEG_OP_RSMAT,         131),
  /*    134 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    135 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Options
  /*    136 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Options
  /*    137 */ CHPEG_INST(CHPEG_OP_CHRCLS,          8), // [a-zA-Z_]
  /*    138 */ CHPEG_INST(CHPEG_OP_GOTO,          145),
  /*    139 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    140 */ CHPEG_INST(CHPEG_OP_CHRCLS,          9), // [a-zA-Z_0-9]
  /*    141 */ CHPEG_INST(CHPEG_OP_GOTO,          143),
  /*    142 */ CHPEG_INST(CHPEG_OP_RSMAT,         140),
  /*    143 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    144 */ CHPEG_INST(CHPEG_OP_ISUCC,           8), // Identifier
  /*    145 */ CHPEG_INST(CHPEG_OP_IFAIL,           8), // Identifier
  /*    146 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    147 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [']
  /*    148 */ CHPEG_INST(CHPEG_OP_GOTO,          162),
  /*    149 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    150 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    151 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [']
  /*    152 */ CHPEG_INST(CHPEG_OP_GOTO,          154),
  /*    153 */ CHPEG_INST(CHPEG_OP_PMATCHF,       158),
  /*    154 */ CHPEG_INST(CHPEG_OP_PNOMATS,       158),
  /*    155 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Char
  /*    156 */ CHPEG_INST(CHPEG_OP_GOTO,          158),
  /*    157 */ CHPEG_INST(CHPEG_OP_RSMAT,         150),
  /*    158 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    159 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [']
  /*    160 */ CHPEG_INST(CHPEG_OP_GOTO,          162),
  /*    161 */ CHPEG_INST(CHPEG_OP_CISUCC,        180),
  /*    162 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    163 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [\"]
  /*    164 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    165 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    166 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    167 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [\"]
  /*    168 */ CHPEG_INST(CHPEG_OP_GOTO,          170),
  /*    169 */ CHPEG_INST(CHPEG_OP_PMATCHF,       174),
  /*    170 */ CHPEG_INST(CHPEG_OP_PNOMATS,       174),
  /*    171 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Char
  /*    172 */ CHPEG_INST(CHPEG_OP_GOTO,          174),
  /*    173 */ CHPEG_INST(CHPEG_OP_RSMAT,         166),
  /*    174 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    175 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [\"]
  /*    176 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    177 */ CHPEG_INST(CHPEG_OP_CISUCC,        180),
  /*    178 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    179 */ CHPEG_INST(CHPEG_OP_CFAIL,         181),
  /*    180 */ CHPEG_INST(CHPEG_OP_ISUCC,           9), // Literal
  /*    181 */ CHPEG_INST(CHPEG_OP_IFAIL,           9), // Literal
  /*    182 */ CHPEG_INST(CHPEG_OP_LIT,            12), // "["
  /*    183 */ CHPEG_INST(CHPEG_OP_GOTO,          197),
  /*    184 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    185 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    186 */ CHPEG_INST(CHPEG_OP_LIT,            13), // "]"
  /*    187 */ CHPEG_INST(CHPEG_OP_GOTO,          189),
  /*    188 */ CHPEG_INST(CHPEG_OP_PMATCHF,       193),
  /*    189 */ CHPEG_INST(CHPEG_OP_PNOMATS,       193),
  /*    190 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // CharRange
  /*    191 */ CHPEG_INST(CHPEG_OP_GOTO,          193),
  /*    192 */ CHPEG_INST(CHPEG_OP_RPMAT,         185),
  /*    193 */ CHPEG_INST(CHPEG_OP_RPDONE,        197),
  /*    194 */ CHPEG_INST(CHPEG_OP_LIT,            13), // "]"
  /*    195 */ CHPEG_INST(CHPEG_OP_GOTO,          197),
  /*    196 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // CharClass
  /*    197 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // CharClass
  /*    198 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    199 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Char
  /*    200 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    201 */ CHPEG_INST(CHPEG_OP_LIT,            14), // "-"
  /*    202 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    203 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    204 */ CHPEG_INST(CHPEG_OP_LIT,            13), // "]"
  /*    205 */ CHPEG_INST(CHPEG_OP_GOTO,          207),
  /*    206 */ CHPEG_INST(CHPEG_OP_PMATCHF,       211),
  /*    207 */ CHPEG_INST(CHPEG_OP_PNOMATS,       211),
  /*    208 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Char
  /*    209 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    210 */ CHPEG_INST(CHPEG_OP_CISUCC,        217),
  /*    211 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    212 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Char
  /*    213 */ CHPEG_INST(CHPEG_OP_GOTO,          215),
  /*    214 */ CHPEG_INST(CHPEG_OP_CISUCC,        217),
  /*    215 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    216 */ CHPEG_INST(CHPEG_OP_CFAIL,         218),
  /*    217 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // CharRange
  /*    218 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // CharRange
  /*    219 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    220 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // EscChar
  /*    221 */ CHPEG_INST(CHPEG_OP_GOTO,          223),
  /*    222 */ CHPEG_INST(CHPEG_OP_CISUCC,        233),
  /*    223 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    224 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // OctChar
  /*    225 */ CHPEG_INST(CHPEG_OP_GOTO,          227),
  /*    226 */ CHPEG_INST(CHPEG_OP_CISUCC,        233),
  /*    227 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    228 */ CHPEG_INST(CHPEG_OP_IDENT,          15), // PlainChar
  /*    229 */ CHPEG_INST(CHPEG_OP_GOTO,          231),
  /*    230 */ CHPEG_INST(CHPEG_OP_CISUCC,        233),
  /*    231 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    232 */ CHPEG_INST(CHPEG_OP_CFAIL,         234),
  /*    233 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // Char
  /*    234 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // Char
  /*    235 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "\\"
  /*    236 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    237 */ CHPEG_INST(CHPEG_OP_CHRCLS,         16), // [nrt'\"[]\\]
  /*    238 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    239 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // EscChar
  /*    240 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // EscChar
  /*    241 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    242 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "\\"
  /*    243 */ CHPEG_INST(CHPEG_OP_GOTO,          251),
  /*    244 */ CHPEG_INST(CHPEG_OP_CHRCLS,         17), // [0-3]
  /*    245 */ CHPEG_INST(CHPEG_OP_GOTO,          251),
  /*    246 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [0-7]
  /*    247 */ CHPEG_INST(CHPEG_OP_GOTO,          251),
  /*    248 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [0-7]
  /*    249 */ CHPEG_INST(CHPEG_OP_GOTO,          251),
  /*    250 */ CHPEG_INST(CHPEG_OP_CISUCC,        264),
  /*    251 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    252 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "\\"
  /*    253 */ CHPEG_INST(CHPEG_OP_GOTO,          262),
  /*    254 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [0-7]
  /*    255 */ CHPEG_INST(CHPEG_OP_GOTO,          262),
  /*    256 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    257 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [0-7]
  /*    258 */ CHPEG_INST(CHPEG_OP_GOTO,          260),
  /*    259 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    260 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    261 */ CHPEG_INST(CHPEG_OP_CISUCC,        264),
  /*    262 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    263 */ CHPEG_INST(CHPEG_OP_CFAIL,         265),
  /*    264 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // OctChar
  /*    265 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // OctChar
  /*    266 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    267 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "\\"
  /*    268 */ CHPEG_INST(CHPEG_OP_GOTO,          270),
  /*    269 */ CHPEG_INST(CHPEG_OP_PMATCHF,       273),
  /*    270 */ CHPEG_INST(CHPEG_OP_PNOMATS,       273),
  /*    271 */ CHPEG_INST(CHPEG_OP_DOT,           273),
  /*    272 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // PlainChar
  /*    273 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // PlainChar
  /*    274 */ CHPEG_INST(CHPEG_OP_CHRCLS,         19), // [&!]
  /*    275 */ CHPEG_INST(CHPEG_OP_GOTO,          277),
  /*    276 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // PredOp
  /*    277 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // PredOp
  /*    278 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [*+?]
  /*    279 */ CHPEG_INST(CHPEG_OP_GOTO,          281),
  /*    280 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // RepOp
  /*    281 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // RepOp
  /*    282 */ CHPEG_INST(CHPEG_OP_LIT,            21), // "."
  /*    283 */ CHPEG_INST(CHPEG_OP_GOTO,          285),
  /*    284 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // Dot
  /*    285 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // Dot
  /*    286 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    287 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    288 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    289 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [ \t\r\n]
  /*    290 */ CHPEG_INST(CHPEG_OP_GOTO,          292),
  /*    291 */ CHPEG_INST(CHPEG_OP_RPMAT,         289),
  /*    292 */ CHPEG_INST(CHPEG_OP_RPDONE,        294),
  /*    293 */ CHPEG_INST(CHPEG_OP_CISUCC,        314),
  /*    294 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    295 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "#"
  /*    296 */ CHPEG_INST(CHPEG_OP_GOTO,          312),
  /*    297 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    298 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    299 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [\r\n]
  /*    300 */ CHPEG_INST(CHPEG_OP_GOTO,          302),
  /*    301 */ CHPEG_INST(CHPEG_OP_PMATCHF,       305),
  /*    302 */ CHPEG_INST(CHPEG_OP_PNOMATS,       305),
  /*    303 */ CHPEG_INST(CHPEG_OP_DOT,           305),
  /*    304 */ CHPEG_INST(CHPEG_OP_RSMAT,         298),
  /*    305 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    306 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    307 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [\r\n]
  /*    308 */ CHPEG_INST(CHPEG_OP_GOTO,          310),
  /*    309 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    310 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    311 */ CHPEG_INST(CHPEG_OP_CISUCC,        314),
  /*    312 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    313 */ CHPEG_INST(CHPEG_OP_CFAIL,         315),
  /*    314 */ CHPEG_INST(CHPEG_OP_RSMAT,         287),
  /*    315 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    316 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // S
  /*    317 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // S
  };

const char *chpeg_bytecode_strings[25] = {"{", "}", "<-", "/", "{<", "(", ")", "A-Za-z", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"};

int chpeg_bytecode_str_len[25] = {1, 1, 2, 1, 2, 1, 1, 6, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2};

const ChpegByteCode chpeg_bytecode = {
  20, // num_defs
  (char **)chpeg_bytecode_def_names,
  chpeg_bytecode_def_flags,
  chpeg_bytecode_def_addrs,
  318, // num_instructions
  chpeg_bytecode_instructions,
  25, // num_strings
  (unsigned char **)chpeg_bytecode_strings,
  chpeg_bytecode_str_len,
#if CHPEG_EXTENSION_REFS
  0, // num_refs
  NULL, // refs,
#endif

};

#ifdef __cplusplus
}
#endif

#endif // #ifndef CHPEG_CHPEG_BYTECODE_C
