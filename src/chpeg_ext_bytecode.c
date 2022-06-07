#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

CHPEG_DEF const ChpegByteCode chpeg_bytecode = {
  .num_defs = 24,
  .def_names = (char*[24]) {"Grammar", "Definition", "Choice", "Sequence", "Element", "Predicate", "Repeat", "Primary", "Mark", "Reference", "Trim", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"},
  .def_flags = (int[24]) {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE},
  .def_addrs = (int[24]) {3, 16, 43, 56, 63, 79, 90, 101, 157, 173, 184, 196, 203, 212, 248, 264, 285, 301, 307, 332, 340, 344, 348, 352},
  .num_instructions = 384,
  .instructions = (int[384]) {
  /*      0 */ CHPEG_INST(CHPEG_OP_IDENT,           0), // Grammar
  /*      1 */ CHPEG_INST(CHPEG_OP_FAIL,            0),
  /*      2 */ CHPEG_INST(CHPEG_OP_SUCC,            0),
  /*      3 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
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
  /*     16 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*     17 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // Options
  /*     26 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     27 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     28 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     29 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*     30 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     31 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     32 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     33 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     34 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     35 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*     36 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     37 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
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
  /*     48 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
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
  /*     66 */ CHPEG_INST(CHPEG_OP_CISUCC,         77),
  /*     67 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     68 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Mark
  /*     69 */ CHPEG_INST(CHPEG_OP_GOTO,           71),
  /*     70 */ CHPEG_INST(CHPEG_OP_CISUCC,         77),
  /*     71 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     72 */ CHPEG_INST(CHPEG_OP_IDENT,          10), // Trim
  /*     73 */ CHPEG_INST(CHPEG_OP_GOTO,           75),
  /*     74 */ CHPEG_INST(CHPEG_OP_CISUCC,         77),
  /*     75 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*     76 */ CHPEG_INST(CHPEG_OP_CFAIL,          78),
  /*     77 */ CHPEG_INST(CHPEG_OP_ISUCC,           4), // Element
  /*     78 */ CHPEG_INST(CHPEG_OP_IFAIL,           4), // Element
  /*     79 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     80 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // PredOp
  /*     81 */ CHPEG_INST(CHPEG_OP_GOTO,           85),
  /*     82 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     83 */ CHPEG_INST(CHPEG_OP_GOTO,           85),
  /*     84 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     85 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     86 */ CHPEG_INST(CHPEG_OP_IDENT,           6), // Repeat
  /*     87 */ CHPEG_INST(CHPEG_OP_GOTO,           89),
  /*     88 */ CHPEG_INST(CHPEG_OP_ISUCC,           5), // Predicate
  /*     89 */ CHPEG_INST(CHPEG_OP_IFAIL,           5), // Predicate
  /*     90 */ CHPEG_INST(CHPEG_OP_IDENT,           7), // Primary
  /*     91 */ CHPEG_INST(CHPEG_OP_GOTO,          100),
  /*     92 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     93 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // RepOp
  /*     94 */ CHPEG_INST(CHPEG_OP_GOTO,           98),
  /*     95 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*     96 */ CHPEG_INST(CHPEG_OP_GOTO,           98),
  /*     97 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     98 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     99 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Repeat
  /*    100 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Repeat
  /*    101 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    102 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    103 */ CHPEG_INST(CHPEG_OP_GOTO,          117),
  /*    104 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    105 */ CHPEG_INST(CHPEG_OP_GOTO,          117),
  /*    106 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    107 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*    108 */ CHPEG_INST(CHPEG_OP_GOTO,          110),
  /*    109 */ CHPEG_INST(CHPEG_OP_PMATCHF,       117),
  /*    110 */ CHPEG_INST(CHPEG_OP_PNOMATS,       117),
  /*    111 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    112 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*    113 */ CHPEG_INST(CHPEG_OP_GOTO,          115),
  /*    114 */ CHPEG_INST(CHPEG_OP_PMATCHF,       117),
  /*    115 */ CHPEG_INST(CHPEG_OP_PNOMATS,       117),
  /*    116 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    117 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    118 */ CHPEG_INST(CHPEG_OP_LIT,             4), // "("
  /*    119 */ CHPEG_INST(CHPEG_OP_GOTO,          129),
  /*    120 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    121 */ CHPEG_INST(CHPEG_OP_GOTO,          129),
  /*    122 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    123 */ CHPEG_INST(CHPEG_OP_GOTO,          129),
  /*    124 */ CHPEG_INST(CHPEG_OP_LIT,             5), // ")"
  /*    125 */ CHPEG_INST(CHPEG_OP_GOTO,          129),
  /*    126 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    127 */ CHPEG_INST(CHPEG_OP_GOTO,          129),
  /*    128 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    129 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    130 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Reference
  /*    131 */ CHPEG_INST(CHPEG_OP_GOTO,          135),
  /*    132 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    133 */ CHPEG_INST(CHPEG_OP_GOTO,          135),
  /*    134 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    135 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    136 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Literal
  /*    137 */ CHPEG_INST(CHPEG_OP_GOTO,          141),
  /*    138 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    139 */ CHPEG_INST(CHPEG_OP_GOTO,          141),
  /*    140 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    141 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    142 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // CharClass
  /*    143 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    144 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    145 */ CHPEG_INST(CHPEG_OP_GOTO,          147),
  /*    146 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    147 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    148 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // Dot
  /*    149 */ CHPEG_INST(CHPEG_OP_GOTO,          153),
  /*    150 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    151 */ CHPEG_INST(CHPEG_OP_GOTO,          153),
  /*    152 */ CHPEG_INST(CHPEG_OP_CISUCC,        155),
  /*    153 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    154 */ CHPEG_INST(CHPEG_OP_CFAIL,         156),
  /*    155 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Primary
  /*    156 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Primary
  /*    157 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$"
  /*    158 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    159 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    160 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    161 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    162 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    163 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    164 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    165 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    166 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    167 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
  /*    168 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    169 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    170 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    171 */ CHPEG_INST(CHPEG_OP_ISUCC,           8), // Mark
  /*    172 */ CHPEG_INST(CHPEG_OP_IFAIL,           8), // Mark
  /*    173 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$"
  /*    174 */ CHPEG_INST(CHPEG_OP_GOTO,          183),
  /*    175 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    176 */ CHPEG_INST(CHPEG_OP_GOTO,          183),
  /*    177 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    178 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    179 */ CHPEG_INST(CHPEG_OP_GOTO,          181),
  /*    180 */ CHPEG_INST(CHPEG_OP_PMATCHF,       183),
  /*    181 */ CHPEG_INST(CHPEG_OP_PNOMATS,       183),
  /*    182 */ CHPEG_INST(CHPEG_OP_ISUCC,           9), // Reference
  /*    183 */ CHPEG_INST(CHPEG_OP_IFAIL,           9), // Reference
  /*    184 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    185 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    186 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    187 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    188 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    189 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    190 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
  /*    191 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    192 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // S
  /*    193 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    194 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // Trim
  /*    195 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // Trim
  /*    196 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    197 */ CHPEG_INST(CHPEG_OP_CHRCLS,          9), // [A-Za-z]
  /*    198 */ CHPEG_INST(CHPEG_OP_GOTO,          200),
  /*    199 */ CHPEG_INST(CHPEG_OP_RSMAT,         197),
  /*    200 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    201 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // Options
  /*    202 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // Options
  /*    203 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [a-zA-Z_]
  /*    204 */ CHPEG_INST(CHPEG_OP_GOTO,          211),
  /*    205 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    206 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [a-zA-Z_0-9]
  /*    207 */ CHPEG_INST(CHPEG_OP_GOTO,          209),
  /*    208 */ CHPEG_INST(CHPEG_OP_RSMAT,         206),
  /*    209 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    210 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // Identifier
  /*    211 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // Identifier
  /*    212 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    213 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    214 */ CHPEG_INST(CHPEG_OP_GOTO,          228),
  /*    215 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    216 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    217 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    218 */ CHPEG_INST(CHPEG_OP_GOTO,          220),
  /*    219 */ CHPEG_INST(CHPEG_OP_PMATCHF,       224),
  /*    220 */ CHPEG_INST(CHPEG_OP_PNOMATS,       224),
  /*    221 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // Char
  /*    222 */ CHPEG_INST(CHPEG_OP_GOTO,          224),
  /*    223 */ CHPEG_INST(CHPEG_OP_RSMAT,         216),
  /*    224 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    225 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    226 */ CHPEG_INST(CHPEG_OP_GOTO,          228),
  /*    227 */ CHPEG_INST(CHPEG_OP_CISUCC,        246),
  /*    228 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    229 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    230 */ CHPEG_INST(CHPEG_OP_GOTO,          244),
  /*    231 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    232 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    233 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    234 */ CHPEG_INST(CHPEG_OP_GOTO,          236),
  /*    235 */ CHPEG_INST(CHPEG_OP_PMATCHF,       240),
  /*    236 */ CHPEG_INST(CHPEG_OP_PNOMATS,       240),
  /*    237 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // Char
  /*    238 */ CHPEG_INST(CHPEG_OP_GOTO,          240),
  /*    239 */ CHPEG_INST(CHPEG_OP_RSMAT,         232),
  /*    240 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    241 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    242 */ CHPEG_INST(CHPEG_OP_GOTO,          244),
  /*    243 */ CHPEG_INST(CHPEG_OP_CISUCC,        246),
  /*    244 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    245 */ CHPEG_INST(CHPEG_OP_CFAIL,         247),
  /*    246 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // Literal
  /*    247 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // Literal
  /*    248 */ CHPEG_INST(CHPEG_OP_LIT,            14), // "["
  /*    249 */ CHPEG_INST(CHPEG_OP_GOTO,          263),
  /*    250 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    251 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    252 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "]"
  /*    253 */ CHPEG_INST(CHPEG_OP_GOTO,          255),
  /*    254 */ CHPEG_INST(CHPEG_OP_PMATCHF,       259),
  /*    255 */ CHPEG_INST(CHPEG_OP_PNOMATS,       259),
  /*    256 */ CHPEG_INST(CHPEG_OP_IDENT,          15), // CharRange
  /*    257 */ CHPEG_INST(CHPEG_OP_GOTO,          259),
  /*    258 */ CHPEG_INST(CHPEG_OP_RPMAT,         251),
  /*    259 */ CHPEG_INST(CHPEG_OP_RPDONE,        263),
  /*    260 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "]"
  /*    261 */ CHPEG_INST(CHPEG_OP_GOTO,          263),
  /*    262 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // CharClass
  /*    263 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // CharClass
  /*    264 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    265 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // Char
  /*    266 */ CHPEG_INST(CHPEG_OP_GOTO,          277),
  /*    267 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "-"
  /*    268 */ CHPEG_INST(CHPEG_OP_GOTO,          277),
  /*    269 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    270 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "]"
  /*    271 */ CHPEG_INST(CHPEG_OP_GOTO,          273),
  /*    272 */ CHPEG_INST(CHPEG_OP_PMATCHF,       277),
  /*    273 */ CHPEG_INST(CHPEG_OP_PNOMATS,       277),
  /*    274 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // Char
  /*    275 */ CHPEG_INST(CHPEG_OP_GOTO,          277),
  /*    276 */ CHPEG_INST(CHPEG_OP_CISUCC,        283),
  /*    277 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    278 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // Char
  /*    279 */ CHPEG_INST(CHPEG_OP_GOTO,          281),
  /*    280 */ CHPEG_INST(CHPEG_OP_CISUCC,        283),
  /*    281 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    282 */ CHPEG_INST(CHPEG_OP_CFAIL,         284),
  /*    283 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // CharRange
  /*    284 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // CharRange
  /*    285 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    286 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // EscChar
  /*    287 */ CHPEG_INST(CHPEG_OP_GOTO,          289),
  /*    288 */ CHPEG_INST(CHPEG_OP_CISUCC,        299),
  /*    289 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    290 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // OctChar
  /*    291 */ CHPEG_INST(CHPEG_OP_GOTO,          293),
  /*    292 */ CHPEG_INST(CHPEG_OP_CISUCC,        299),
  /*    293 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    294 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // PlainChar
  /*    295 */ CHPEG_INST(CHPEG_OP_GOTO,          297),
  /*    296 */ CHPEG_INST(CHPEG_OP_CISUCC,        299),
  /*    297 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    298 */ CHPEG_INST(CHPEG_OP_CFAIL,         300),
  /*    299 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // Char
  /*    300 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // Char
  /*    301 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    302 */ CHPEG_INST(CHPEG_OP_GOTO,          306),
  /*    303 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [nrt'\"[]\\]
  /*    304 */ CHPEG_INST(CHPEG_OP_GOTO,          306),
  /*    305 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // EscChar
  /*    306 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // EscChar
  /*    307 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    308 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    309 */ CHPEG_INST(CHPEG_OP_GOTO,          317),
  /*    310 */ CHPEG_INST(CHPEG_OP_CHRCLS,         19), // [0-3]
  /*    311 */ CHPEG_INST(CHPEG_OP_GOTO,          317),
  /*    312 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    313 */ CHPEG_INST(CHPEG_OP_GOTO,          317),
  /*    314 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    315 */ CHPEG_INST(CHPEG_OP_GOTO,          317),
  /*    316 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    317 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    318 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    319 */ CHPEG_INST(CHPEG_OP_GOTO,          328),
  /*    320 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    321 */ CHPEG_INST(CHPEG_OP_GOTO,          328),
  /*    322 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    323 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    324 */ CHPEG_INST(CHPEG_OP_GOTO,          326),
  /*    325 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    326 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    327 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    328 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    329 */ CHPEG_INST(CHPEG_OP_CFAIL,         331),
  /*    330 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // OctChar
  /*    331 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // OctChar
  /*    332 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    333 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    334 */ CHPEG_INST(CHPEG_OP_GOTO,          336),
  /*    335 */ CHPEG_INST(CHPEG_OP_PMATCHF,       339),
  /*    336 */ CHPEG_INST(CHPEG_OP_PNOMATS,       339),
  /*    337 */ CHPEG_INST(CHPEG_OP_DOT,           339),
  /*    338 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // PlainChar
  /*    339 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // PlainChar
  /*    340 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [&!]
  /*    341 */ CHPEG_INST(CHPEG_OP_GOTO,          343),
  /*    342 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // PredOp
  /*    343 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // PredOp
  /*    344 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [*+?]
  /*    345 */ CHPEG_INST(CHPEG_OP_GOTO,          347),
  /*    346 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // RepOp
  /*    347 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // RepOp
  /*    348 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "."
  /*    349 */ CHPEG_INST(CHPEG_OP_GOTO,          351),
  /*    350 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // Dot
  /*    351 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // Dot
  /*    352 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    353 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    354 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    355 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [ \t\r\n]
  /*    356 */ CHPEG_INST(CHPEG_OP_GOTO,          358),
  /*    357 */ CHPEG_INST(CHPEG_OP_RPMAT,         355),
  /*    358 */ CHPEG_INST(CHPEG_OP_RPDONE,        360),
  /*    359 */ CHPEG_INST(CHPEG_OP_CISUCC,        380),
  /*    360 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    361 */ CHPEG_INST(CHPEG_OP_LIT,            25), // "#"
  /*    362 */ CHPEG_INST(CHPEG_OP_GOTO,          378),
  /*    363 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    364 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    365 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [\r\n]
  /*    366 */ CHPEG_INST(CHPEG_OP_GOTO,          368),
  /*    367 */ CHPEG_INST(CHPEG_OP_PMATCHF,       371),
  /*    368 */ CHPEG_INST(CHPEG_OP_PNOMATS,       371),
  /*    369 */ CHPEG_INST(CHPEG_OP_DOT,           371),
  /*    370 */ CHPEG_INST(CHPEG_OP_RSMAT,         364),
  /*    371 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    372 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    373 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [\r\n]
  /*    374 */ CHPEG_INST(CHPEG_OP_GOTO,          376),
  /*    375 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    376 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    377 */ CHPEG_INST(CHPEG_OP_CISUCC,        380),
  /*    378 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    379 */ CHPEG_INST(CHPEG_OP_CFAIL,         381),
  /*    380 */ CHPEG_INST(CHPEG_OP_RSMAT,         353),
  /*    381 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    382 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // S
  /*    383 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // S
  },
  .num_strings = 27,
  .strings = (unsigned char**)(char*[27]) {"{", "}", "<-", "/", "(", ")", "$", "<", ">", "A-Za-z", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"},
  .str_len = (int[27]) {1, 1, 2, 1, 1, 1, 1, 1, 1, 6, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2},
};
