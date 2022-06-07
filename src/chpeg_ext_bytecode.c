#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

CHPEG_DEF const ChpegByteCode chpeg_bytecode = {
  .num_defs = 25,
  .def_names = (char*[25]) {"Grammar", "Definition", "Choice", "Sequence", "Element", "Predicate", "Repeat", "Primary", "RefScope", "Mark", "Reference", "Trim", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"},
  .def_flags = (int[25]) {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE},
  .def_addrs = (int[25]) {3, 16, 43, 56, 63, 83, 94, 105, 161, 173, 189, 200, 212, 219, 228, 264, 280, 301, 317, 323, 348, 356, 360, 364, 368},
  .num_instructions = 400,
  .instructions = (int[400]) {
  /*      0 */ CHPEG_INST(CHPEG_OP_IDENT,           0), // Grammar
  /*      1 */ CHPEG_INST(CHPEG_OP_FAIL,            0),
  /*      2 */ CHPEG_INST(CHPEG_OP_SUCC,            0),
  /*      3 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
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
  /*     16 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Identifier
  /*     17 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Options
  /*     26 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     27 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     28 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     29 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*     30 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     31 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     32 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     33 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     34 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     35 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*     36 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     37 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
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
  /*     48 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
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
  /*     66 */ CHPEG_INST(CHPEG_OP_CISUCC,         81),
  /*     67 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     68 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // RefScope
  /*     69 */ CHPEG_INST(CHPEG_OP_GOTO,           71),
  /*     70 */ CHPEG_INST(CHPEG_OP_CISUCC,         81),
  /*     71 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     72 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Mark
  /*     73 */ CHPEG_INST(CHPEG_OP_GOTO,           75),
  /*     74 */ CHPEG_INST(CHPEG_OP_CISUCC,         81),
  /*     75 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     76 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // Trim
  /*     77 */ CHPEG_INST(CHPEG_OP_GOTO,           79),
  /*     78 */ CHPEG_INST(CHPEG_OP_CISUCC,         81),
  /*     79 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     80 */ CHPEG_INST(CHPEG_OP_CFAIL,          82),
  /*     81 */ CHPEG_INST(CHPEG_OP_ISUCC,           4), // Element
  /*     82 */ CHPEG_INST(CHPEG_OP_IFAIL,           4), // Element
  /*     83 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     84 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // PredOp
  /*     85 */ CHPEG_INST(CHPEG_OP_GOTO,           89),
  /*     86 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     87 */ CHPEG_INST(CHPEG_OP_GOTO,           89),
  /*     88 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     89 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     90 */ CHPEG_INST(CHPEG_OP_IDENT,           6), // Repeat
  /*     91 */ CHPEG_INST(CHPEG_OP_GOTO,           93),
  /*     92 */ CHPEG_INST(CHPEG_OP_ISUCC,           5), // Predicate
  /*     93 */ CHPEG_INST(CHPEG_OP_IFAIL,           5), // Predicate
  /*     94 */ CHPEG_INST(CHPEG_OP_IDENT,           7), // Primary
  /*     95 */ CHPEG_INST(CHPEG_OP_GOTO,          104),
  /*     96 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     97 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // RepOp
  /*     98 */ CHPEG_INST(CHPEG_OP_GOTO,          102),
  /*     99 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    100 */ CHPEG_INST(CHPEG_OP_GOTO,          102),
  /*    101 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    102 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    103 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Repeat
  /*    104 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Repeat
  /*    105 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    106 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Identifier
  /*    107 */ CHPEG_INST(CHPEG_OP_GOTO,          121),
  /*    108 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    109 */ CHPEG_INST(CHPEG_OP_GOTO,          121),
  /*    110 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    111 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*    112 */ CHPEG_INST(CHPEG_OP_GOTO,          114),
  /*    113 */ CHPEG_INST(CHPEG_OP_PMATCHF,       121),
  /*    114 */ CHPEG_INST(CHPEG_OP_PNOMATS,       121),
  /*    115 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    116 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*    117 */ CHPEG_INST(CHPEG_OP_GOTO,          119),
  /*    118 */ CHPEG_INST(CHPEG_OP_PMATCHF,       121),
  /*    119 */ CHPEG_INST(CHPEG_OP_PNOMATS,       121),
  /*    120 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    121 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    122 */ CHPEG_INST(CHPEG_OP_LIT,             4), // "("
  /*    123 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    124 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    125 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    126 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    127 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    128 */ CHPEG_INST(CHPEG_OP_LIT,             5), // ")"
  /*    129 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    130 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    131 */ CHPEG_INST(CHPEG_OP_GOTO,          133),
  /*    132 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    133 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    134 */ CHPEG_INST(CHPEG_OP_IDENT,          10), // Reference
  /*    135 */ CHPEG_INST(CHPEG_OP_GOTO,          139),
  /*    136 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    137 */ CHPEG_INST(CHPEG_OP_GOTO,          139),
  /*    138 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    139 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    140 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // Literal
  /*    141 */ CHPEG_INST(CHPEG_OP_GOTO,          145),
  /*    142 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    143 */ CHPEG_INST(CHPEG_OP_GOTO,          145),
  /*    144 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    145 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    146 */ CHPEG_INST(CHPEG_OP_IDENT,          15), // CharClass
  /*    147 */ CHPEG_INST(CHPEG_OP_GOTO,          151),
  /*    148 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    149 */ CHPEG_INST(CHPEG_OP_GOTO,          151),
  /*    150 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    151 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    152 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // Dot
  /*    153 */ CHPEG_INST(CHPEG_OP_GOTO,          157),
  /*    154 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    155 */ CHPEG_INST(CHPEG_OP_GOTO,          157),
  /*    156 */ CHPEG_INST(CHPEG_OP_CISUCC,        159),
  /*    157 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    158 */ CHPEG_INST(CHPEG_OP_CFAIL,         160),
  /*    159 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Primary
  /*    160 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Primary
  /*    161 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$<"
  /*    162 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    163 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    164 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    165 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    166 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    167 */ CHPEG_INST(CHPEG_OP_LIT,             7), // ">"
  /*    168 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    169 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    170 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    171 */ CHPEG_INST(CHPEG_OP_ISUCC,           8), // RefScope
  /*    172 */ CHPEG_INST(CHPEG_OP_IFAIL,           8), // RefScope
  /*    173 */ CHPEG_INST(CHPEG_OP_LIT,             8), // "$"
  /*    174 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    175 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Identifier
  /*    176 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    177 */ CHPEG_INST(CHPEG_OP_LIT,             9), // "<"
  /*    178 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    179 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    180 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    181 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    182 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    183 */ CHPEG_INST(CHPEG_OP_LIT,             7), // ">"
  /*    184 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    185 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    186 */ CHPEG_INST(CHPEG_OP_GOTO,          188),
  /*    187 */ CHPEG_INST(CHPEG_OP_ISUCC,           9), // Mark
  /*    188 */ CHPEG_INST(CHPEG_OP_IFAIL,           9), // Mark
  /*    189 */ CHPEG_INST(CHPEG_OP_LIT,             8), // "$"
  /*    190 */ CHPEG_INST(CHPEG_OP_GOTO,          199),
  /*    191 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Identifier
  /*    192 */ CHPEG_INST(CHPEG_OP_GOTO,          199),
  /*    193 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    194 */ CHPEG_INST(CHPEG_OP_LIT,             9), // "<"
  /*    195 */ CHPEG_INST(CHPEG_OP_GOTO,          197),
  /*    196 */ CHPEG_INST(CHPEG_OP_PMATCHF,       199),
  /*    197 */ CHPEG_INST(CHPEG_OP_PNOMATS,       199),
  /*    198 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // Reference
  /*    199 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // Reference
  /*    200 */ CHPEG_INST(CHPEG_OP_LIT,             9), // "<"
  /*    201 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    202 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    203 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    204 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    205 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    206 */ CHPEG_INST(CHPEG_OP_LIT,             7), // ">"
  /*    207 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    208 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    209 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    210 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // Trim
  /*    211 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // Trim
  /*    212 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    213 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [A-Za-z]
  /*    214 */ CHPEG_INST(CHPEG_OP_GOTO,          216),
  /*    215 */ CHPEG_INST(CHPEG_OP_RSMAT,         213),
  /*    216 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    217 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // Options
  /*    218 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // Options
  /*    219 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [a-zA-Z_]
  /*    220 */ CHPEG_INST(CHPEG_OP_GOTO,          227),
  /*    221 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    222 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [a-zA-Z_0-9]
  /*    223 */ CHPEG_INST(CHPEG_OP_GOTO,          225),
  /*    224 */ CHPEG_INST(CHPEG_OP_RSMAT,         222),
  /*    225 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    226 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // Identifier
  /*    227 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // Identifier
  /*    228 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    229 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [']
  /*    230 */ CHPEG_INST(CHPEG_OP_GOTO,          244),
  /*    231 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    232 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    233 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [']
  /*    234 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    235 */ CHPEG_INST(CHPEG_OP_PMATCHF,       240),
  /*    236 */ CHPEG_INST(CHPEG_OP_PNOMATS,       240),
  /*    237 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    238 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    239 */ CHPEG_INST(CHPEG_OP_RSMAT,         232),
  /*    240 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    241 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [']
  /*    242 */ CHPEG_INST(CHPEG_OP_GOTO,          244),
  /*    243 */ CHPEG_INST(CHPEG_OP_CISUCC,        262),
  /*    244 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    245 */ CHPEG_INST(CHPEG_OP_CHRCLS,         14), // [\"]
  /*    246 */ CHPEG_INST(CHPEG_OP_GOTO,          260),
  /*    247 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    248 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    249 */ CHPEG_INST(CHPEG_OP_CHRCLS,         14), // [\"]
  /*    250 */ CHPEG_INST(CHPEG_OP_GOTO,          252),
  /*    251 */ CHPEG_INST(CHPEG_OP_PMATCHF,       256),
  /*    252 */ CHPEG_INST(CHPEG_OP_PNOMATS,       256),
  /*    253 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    254 */ CHPEG_INST(CHPEG_OP_GOTO,          256),
  /*    255 */ CHPEG_INST(CHPEG_OP_RSMAT,         248),
  /*    256 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    257 */ CHPEG_INST(CHPEG_OP_CHRCLS,         14), // [\"]
  /*    258 */ CHPEG_INST(CHPEG_OP_GOTO,          260),
  /*    259 */ CHPEG_INST(CHPEG_OP_CISUCC,        262),
  /*    260 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    261 */ CHPEG_INST(CHPEG_OP_CFAIL,         263),
  /*    262 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // Literal
  /*    263 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // Literal
  /*    264 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "["
  /*    265 */ CHPEG_INST(CHPEG_OP_GOTO,          279),
  /*    266 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    267 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    268 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    269 */ CHPEG_INST(CHPEG_OP_GOTO,          271),
  /*    270 */ CHPEG_INST(CHPEG_OP_PMATCHF,       275),
  /*    271 */ CHPEG_INST(CHPEG_OP_PNOMATS,       275),
  /*    272 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // CharRange
  /*    273 */ CHPEG_INST(CHPEG_OP_GOTO,          275),
  /*    274 */ CHPEG_INST(CHPEG_OP_RPMAT,         267),
  /*    275 */ CHPEG_INST(CHPEG_OP_RPDONE,        279),
  /*    276 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    277 */ CHPEG_INST(CHPEG_OP_GOTO,          279),
  /*    278 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // CharClass
  /*    279 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // CharClass
  /*    280 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    281 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    282 */ CHPEG_INST(CHPEG_OP_GOTO,          293),
  /*    283 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "-"
  /*    284 */ CHPEG_INST(CHPEG_OP_GOTO,          293),
  /*    285 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    286 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    287 */ CHPEG_INST(CHPEG_OP_GOTO,          289),
  /*    288 */ CHPEG_INST(CHPEG_OP_PMATCHF,       293),
  /*    289 */ CHPEG_INST(CHPEG_OP_PNOMATS,       293),
  /*    290 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    291 */ CHPEG_INST(CHPEG_OP_GOTO,          293),
  /*    292 */ CHPEG_INST(CHPEG_OP_CISUCC,        299),
  /*    293 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    294 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    295 */ CHPEG_INST(CHPEG_OP_GOTO,          297),
  /*    296 */ CHPEG_INST(CHPEG_OP_CISUCC,        299),
  /*    297 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    298 */ CHPEG_INST(CHPEG_OP_CFAIL,         300),
  /*    299 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // CharRange
  /*    300 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // CharRange
  /*    301 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    302 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // EscChar
  /*    303 */ CHPEG_INST(CHPEG_OP_GOTO,          305),
  /*    304 */ CHPEG_INST(CHPEG_OP_CISUCC,        315),
  /*    305 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    306 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // OctChar
  /*    307 */ CHPEG_INST(CHPEG_OP_GOTO,          309),
  /*    308 */ CHPEG_INST(CHPEG_OP_CISUCC,        315),
  /*    309 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    310 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // PlainChar
  /*    311 */ CHPEG_INST(CHPEG_OP_GOTO,          313),
  /*    312 */ CHPEG_INST(CHPEG_OP_CISUCC,        315),
  /*    313 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    314 */ CHPEG_INST(CHPEG_OP_CFAIL,         316),
  /*    315 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // Char
  /*    316 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // Char
  /*    317 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "\\"
  /*    318 */ CHPEG_INST(CHPEG_OP_GOTO,          322),
  /*    319 */ CHPEG_INST(CHPEG_OP_CHRCLS,         19), // [nrt'\"[]\\]
  /*    320 */ CHPEG_INST(CHPEG_OP_GOTO,          322),
  /*    321 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // EscChar
  /*    322 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // EscChar
  /*    323 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    324 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "\\"
  /*    325 */ CHPEG_INST(CHPEG_OP_GOTO,          333),
  /*    326 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-3]
  /*    327 */ CHPEG_INST(CHPEG_OP_GOTO,          333),
  /*    328 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [0-7]
  /*    329 */ CHPEG_INST(CHPEG_OP_GOTO,          333),
  /*    330 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [0-7]
  /*    331 */ CHPEG_INST(CHPEG_OP_GOTO,          333),
  /*    332 */ CHPEG_INST(CHPEG_OP_CISUCC,        346),
  /*    333 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    334 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "\\"
  /*    335 */ CHPEG_INST(CHPEG_OP_GOTO,          344),
  /*    336 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [0-7]
  /*    337 */ CHPEG_INST(CHPEG_OP_GOTO,          344),
  /*    338 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    339 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [0-7]
  /*    340 */ CHPEG_INST(CHPEG_OP_GOTO,          342),
  /*    341 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    342 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    343 */ CHPEG_INST(CHPEG_OP_CISUCC,        346),
  /*    344 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    345 */ CHPEG_INST(CHPEG_OP_CFAIL,         347),
  /*    346 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // OctChar
  /*    347 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // OctChar
  /*    348 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    349 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "\\"
  /*    350 */ CHPEG_INST(CHPEG_OP_GOTO,          352),
  /*    351 */ CHPEG_INST(CHPEG_OP_PMATCHF,       355),
  /*    352 */ CHPEG_INST(CHPEG_OP_PNOMATS,       355),
  /*    353 */ CHPEG_INST(CHPEG_OP_DOT,           355),
  /*    354 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // PlainChar
  /*    355 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // PlainChar
  /*    356 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [&!]
  /*    357 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    358 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // PredOp
  /*    359 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // PredOp
  /*    360 */ CHPEG_INST(CHPEG_OP_CHRCLS,         23), // [*+?]
  /*    361 */ CHPEG_INST(CHPEG_OP_GOTO,          363),
  /*    362 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // RepOp
  /*    363 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // RepOp
  /*    364 */ CHPEG_INST(CHPEG_OP_LIT,            24), // "."
  /*    365 */ CHPEG_INST(CHPEG_OP_GOTO,          367),
  /*    366 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // Dot
  /*    367 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // Dot
  /*    368 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    369 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    370 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    371 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [ \t\r\n]
  /*    372 */ CHPEG_INST(CHPEG_OP_GOTO,          374),
  /*    373 */ CHPEG_INST(CHPEG_OP_RPMAT,         371),
  /*    374 */ CHPEG_INST(CHPEG_OP_RPDONE,        376),
  /*    375 */ CHPEG_INST(CHPEG_OP_CISUCC,        396),
  /*    376 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    377 */ CHPEG_INST(CHPEG_OP_LIT,            26), // "#"
  /*    378 */ CHPEG_INST(CHPEG_OP_GOTO,          394),
  /*    379 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    380 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    381 */ CHPEG_INST(CHPEG_OP_CHRCLS,         27), // [\r\n]
  /*    382 */ CHPEG_INST(CHPEG_OP_GOTO,          384),
  /*    383 */ CHPEG_INST(CHPEG_OP_PMATCHF,       387),
  /*    384 */ CHPEG_INST(CHPEG_OP_PNOMATS,       387),
  /*    385 */ CHPEG_INST(CHPEG_OP_DOT,           387),
  /*    386 */ CHPEG_INST(CHPEG_OP_RSMAT,         380),
  /*    387 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    388 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    389 */ CHPEG_INST(CHPEG_OP_CHRCLS,         27), // [\r\n]
  /*    390 */ CHPEG_INST(CHPEG_OP_GOTO,          392),
  /*    391 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    392 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    393 */ CHPEG_INST(CHPEG_OP_CISUCC,        396),
  /*    394 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    395 */ CHPEG_INST(CHPEG_OP_CFAIL,         397),
  /*    396 */ CHPEG_INST(CHPEG_OP_RSMAT,         369),
  /*    397 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    398 */ CHPEG_INST(CHPEG_OP_ISUCC,          24), // S
  /*    399 */ CHPEG_INST(CHPEG_OP_IFAIL,          24), // S
  },
  .num_strings = 28,
  .strings = (unsigned char**)(char*[28]) {"{", "}", "<-", "/", "(", ")", "$<", ">", "$", "<", "A-Za-z", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"},
  .str_len = (int[28]) {1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 6, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2},
};
