#ifndef CHPEG_CHPEG_EXT_BYTECODE_C
#define CHPEG_CHPEG_EXT_BYTECODE_C

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

const char *chpeg_ext_bytecode_def_names[25] = {"Grammar", "Definition", "Choice", "Sequence", "Element", "Predicate", "Repeat", "Primary", "Mark", "Reference", "Trim", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "HexChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"};

int chpeg_ext_bytecode_def_flags[25] = {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE};

int chpeg_ext_bytecode_def_addrs[25] = {3, 16, 43, 56, 63, 75, 86, 97, 159, 173, 184, 196, 203, 212, 248, 264, 285, 305, 311, 336, 347, 355, 359, 363, 367};

int chpeg_ext_bytecode_instructions[399] = {
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
  /*     16 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*     17 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // Options
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
  /*     76 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // PredOp
  /*     77 */ CHPEG_INST(CHPEG_OP_GOTO,           81),
  /*     78 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     79 */ CHPEG_INST(CHPEG_OP_GOTO,           81),
  /*     80 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     81 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     82 */ CHPEG_INST(CHPEG_OP_IDENT,           6), // Repeat
  /*     83 */ CHPEG_INST(CHPEG_OP_GOTO,           85),
  /*     84 */ CHPEG_INST(CHPEG_OP_ISUCC,           5), // Predicate
  /*     85 */ CHPEG_INST(CHPEG_OP_IFAIL,           5), // Predicate
  /*     86 */ CHPEG_INST(CHPEG_OP_IDENT,           7), // Primary
  /*     87 */ CHPEG_INST(CHPEG_OP_GOTO,           96),
  /*     88 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     89 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // RepOp
  /*     90 */ CHPEG_INST(CHPEG_OP_GOTO,           94),
  /*     91 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*     92 */ CHPEG_INST(CHPEG_OP_GOTO,           94),
  /*     93 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     94 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     95 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Repeat
  /*     96 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Repeat
  /*     97 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*     98 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*     99 */ CHPEG_INST(CHPEG_OP_GOTO,          113),
  /*    100 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    101 */ CHPEG_INST(CHPEG_OP_GOTO,          113),
  /*    102 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    103 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*    104 */ CHPEG_INST(CHPEG_OP_GOTO,          106),
  /*    105 */ CHPEG_INST(CHPEG_OP_PMATCHF,       113),
  /*    106 */ CHPEG_INST(CHPEG_OP_PNOMATS,       113),
  /*    107 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    108 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*    109 */ CHPEG_INST(CHPEG_OP_GOTO,          111),
  /*    110 */ CHPEG_INST(CHPEG_OP_PMATCHF,       113),
  /*    111 */ CHPEG_INST(CHPEG_OP_PNOMATS,       113),
  /*    112 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    113 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    114 */ CHPEG_INST(CHPEG_OP_LIT,             4), // "("
  /*    115 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    116 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    117 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    118 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    119 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    120 */ CHPEG_INST(CHPEG_OP_LIT,             5), // ")"
  /*    121 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    122 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    123 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    124 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    125 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    126 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Reference
  /*    127 */ CHPEG_INST(CHPEG_OP_GOTO,          131),
  /*    128 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    129 */ CHPEG_INST(CHPEG_OP_GOTO,          131),
  /*    130 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    131 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    132 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Mark
  /*    133 */ CHPEG_INST(CHPEG_OP_GOTO,          137),
  /*    134 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    135 */ CHPEG_INST(CHPEG_OP_GOTO,          137),
  /*    136 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    137 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    138 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Literal
  /*    139 */ CHPEG_INST(CHPEG_OP_GOTO,          143),
  /*    140 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    141 */ CHPEG_INST(CHPEG_OP_GOTO,          143),
  /*    142 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    143 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    144 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // CharClass
  /*    145 */ CHPEG_INST(CHPEG_OP_GOTO,          149),
  /*    146 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    147 */ CHPEG_INST(CHPEG_OP_GOTO,          149),
  /*    148 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    149 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    150 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // Dot
  /*    151 */ CHPEG_INST(CHPEG_OP_GOTO,          155),
  /*    152 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    153 */ CHPEG_INST(CHPEG_OP_GOTO,          155),
  /*    154 */ CHPEG_INST(CHPEG_OP_CISUCC,        157),
  /*    155 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    156 */ CHPEG_INST(CHPEG_OP_CFAIL,         158),
  /*    157 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Primary
  /*    158 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Primary
  /*    159 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$"
  /*    160 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    161 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    162 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    163 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    164 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    165 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    166 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    167 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    168 */ CHPEG_INST(CHPEG_OP_GOTO,          172),
  /*    169 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
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
  /*    186 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    187 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    188 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    189 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    190 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
  /*    191 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    192 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // S
  /*    193 */ CHPEG_INST(CHPEG_OP_GOTO,          195),
  /*    194 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // Trim
  /*    195 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // Trim
  /*    196 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    197 */ CHPEG_INST(CHPEG_OP_CHRCLS,          9), // [CSILTPR]
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
  /*    288 */ CHPEG_INST(CHPEG_OP_CISUCC,        303),
  /*    289 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    290 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // HexChar
  /*    291 */ CHPEG_INST(CHPEG_OP_GOTO,          293),
  /*    292 */ CHPEG_INST(CHPEG_OP_CISUCC,        303),
  /*    293 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    294 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // OctChar
  /*    295 */ CHPEG_INST(CHPEG_OP_GOTO,          297),
  /*    296 */ CHPEG_INST(CHPEG_OP_CISUCC,        303),
  /*    297 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    298 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // PlainChar
  /*    299 */ CHPEG_INST(CHPEG_OP_GOTO,          301),
  /*    300 */ CHPEG_INST(CHPEG_OP_CISUCC,        303),
  /*    301 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    302 */ CHPEG_INST(CHPEG_OP_CFAIL,         304),
  /*    303 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // Char
  /*    304 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // Char
  /*    305 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    306 */ CHPEG_INST(CHPEG_OP_GOTO,          310),
  /*    307 */ CHPEG_INST(CHPEG_OP_CHRCLS,         18), // [abfvnrt'\"[]\\]
  /*    308 */ CHPEG_INST(CHPEG_OP_GOTO,          310),
  /*    309 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // EscChar
  /*    310 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // EscChar
  /*    311 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    312 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    313 */ CHPEG_INST(CHPEG_OP_GOTO,          321),
  /*    314 */ CHPEG_INST(CHPEG_OP_CHRCLS,         19), // [0-3]
  /*    315 */ CHPEG_INST(CHPEG_OP_GOTO,          321),
  /*    316 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    317 */ CHPEG_INST(CHPEG_OP_GOTO,          321),
  /*    318 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    319 */ CHPEG_INST(CHPEG_OP_GOTO,          321),
  /*    320 */ CHPEG_INST(CHPEG_OP_CISUCC,        334),
  /*    321 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    322 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    323 */ CHPEG_INST(CHPEG_OP_GOTO,          332),
  /*    324 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    325 */ CHPEG_INST(CHPEG_OP_GOTO,          332),
  /*    326 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    327 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [0-7]
  /*    328 */ CHPEG_INST(CHPEG_OP_GOTO,          330),
  /*    329 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    330 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    331 */ CHPEG_INST(CHPEG_OP_CISUCC,        334),
  /*    332 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    333 */ CHPEG_INST(CHPEG_OP_CFAIL,         335),
  /*    334 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // OctChar
  /*    335 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // OctChar
  /*    336 */ CHPEG_INST(CHPEG_OP_LIT,            21), // "\\x"
  /*    337 */ CHPEG_INST(CHPEG_OP_GOTO,          346),
  /*    338 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-9a-fA-F]
  /*    339 */ CHPEG_INST(CHPEG_OP_GOTO,          346),
  /*    340 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    341 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-9a-fA-F]
  /*    342 */ CHPEG_INST(CHPEG_OP_GOTO,          344),
  /*    343 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    344 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    345 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // HexChar
  /*    346 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // HexChar
  /*    347 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    348 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "\\"
  /*    349 */ CHPEG_INST(CHPEG_OP_GOTO,          351),
  /*    350 */ CHPEG_INST(CHPEG_OP_PMATCHF,       354),
  /*    351 */ CHPEG_INST(CHPEG_OP_PNOMATS,       354),
  /*    352 */ CHPEG_INST(CHPEG_OP_DOT,           354),
  /*    353 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // PlainChar
  /*    354 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // PlainChar
  /*    355 */ CHPEG_INST(CHPEG_OP_CHRCLS,         23), // [&!]
  /*    356 */ CHPEG_INST(CHPEG_OP_GOTO,          358),
  /*    357 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // PredOp
  /*    358 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // PredOp
  /*    359 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [*+?]
  /*    360 */ CHPEG_INST(CHPEG_OP_GOTO,          362),
  /*    361 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // RepOp
  /*    362 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // RepOp
  /*    363 */ CHPEG_INST(CHPEG_OP_LIT,            25), // "."
  /*    364 */ CHPEG_INST(CHPEG_OP_GOTO,          366),
  /*    365 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // Dot
  /*    366 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // Dot
  /*    367 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    368 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    369 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    370 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [ \t\r\n]
  /*    371 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    372 */ CHPEG_INST(CHPEG_OP_RPMAT,         370),
  /*    373 */ CHPEG_INST(CHPEG_OP_RPDONE,        375),
  /*    374 */ CHPEG_INST(CHPEG_OP_CISUCC,        395),
  /*    375 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    376 */ CHPEG_INST(CHPEG_OP_LIT,            27), // "#"
  /*    377 */ CHPEG_INST(CHPEG_OP_GOTO,          393),
  /*    378 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    379 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    380 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [\r\n]
  /*    381 */ CHPEG_INST(CHPEG_OP_GOTO,          383),
  /*    382 */ CHPEG_INST(CHPEG_OP_PMATCHF,       386),
  /*    383 */ CHPEG_INST(CHPEG_OP_PNOMATS,       386),
  /*    384 */ CHPEG_INST(CHPEG_OP_DOT,           386),
  /*    385 */ CHPEG_INST(CHPEG_OP_RSMAT,         379),
  /*    386 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    387 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    388 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [\r\n]
  /*    389 */ CHPEG_INST(CHPEG_OP_GOTO,          391),
  /*    390 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    391 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    392 */ CHPEG_INST(CHPEG_OP_CISUCC,        395),
  /*    393 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    394 */ CHPEG_INST(CHPEG_OP_CFAIL,         396),
  /*    395 */ CHPEG_INST(CHPEG_OP_RSMAT,         368),
  /*    396 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    397 */ CHPEG_INST(CHPEG_OP_ISUCC,          24), // S
  /*    398 */ CHPEG_INST(CHPEG_OP_IFAIL,          24), // S
  };

const char *chpeg_ext_bytecode_strings[29] = {"{", "}", "<-", "/", "(", ")", "$", "<", ">", "CSILTPR", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "abfvnrt'\"[]\\", "0-3", "0-7", "\\x", "0-9a-fA-F", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"};

int chpeg_ext_bytecode_str_len[29] = {1, 1, 2, 1, 1, 1, 1, 1, 1, 7, 7, 10, 1, 1, 1, 1, 1, 1, 12, 3, 3, 2, 9, 2, 3, 1, 4, 1, 2};

const ChpegByteCode chpeg_ext_bytecode = {
  25, // num_defs
  (char **)chpeg_ext_bytecode_def_names,
  chpeg_ext_bytecode_def_flags,
  chpeg_ext_bytecode_def_addrs,
  399, // num_instructions
  chpeg_ext_bytecode_instructions,
  29, // num_strings
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
