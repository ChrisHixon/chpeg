#ifndef CHPEG_CHPEG_EXT_BYTECODE_C
#define CHPEG_CHPEG_EXT_BYTECODE_C

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

const char *chpeg_ext_bytecode_def_names[26] = {"Grammar", "Definition", "Choice", "Sequence", "Element", "Predicate", "Repeat", "Primary", "Mark", "Reference", "Trim", "Options", "Identifier", "Literal", "CharClass", "NCharClass", "CharRange", "Char", "EscChar", "OctChar", "HexChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"};

int chpeg_ext_bytecode_def_flags[26] = {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE};

int chpeg_ext_bytecode_def_addrs[26] = {3, 16, 43, 56, 63, 75, 86, 97, 165, 179, 190, 202, 209, 218, 254, 275, 291, 312, 332, 338, 363, 374, 382, 386, 390, 394};

int chpeg_ext_bytecode_instructions[426] = {
  /*      0 */ CHPEG_INST(CHPEG_OP_IDENT,           0), // Grammar
  /*      1 */ CHPEG_INST(CHPEG_OP_FAIL,            0),
  /*      2 */ CHPEG_INST(CHPEG_OP_SUCC,            0),
  /*      3 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
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
  /*     18 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*     19 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     20 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*     21 */ CHPEG_INST(CHPEG_OP_LIT,             0), // "{"
  /*     22 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     23 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*     24 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     25 */ CHPEG_INST(CHPEG_OP_IDENT,          11), // Options
  /*     26 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     27 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*     28 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     29 */ CHPEG_INST(CHPEG_OP_LIT,             1), // "}"
  /*     30 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     31 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*     32 */ CHPEG_INST(CHPEG_OP_GOTO,           34),
  /*     33 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     34 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     35 */ CHPEG_INST(CHPEG_OP_LIT,             2), // "<-"
  /*     36 */ CHPEG_INST(CHPEG_OP_GOTO,           42),
  /*     37 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
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
  /*     48 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
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
  /*     76 */ CHPEG_INST(CHPEG_OP_IDENT,          22), // PredOp
  /*     77 */ CHPEG_INST(CHPEG_OP_GOTO,           81),
  /*     78 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
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
  /*     89 */ CHPEG_INST(CHPEG_OP_IDENT,          23), // RepOp
  /*     90 */ CHPEG_INST(CHPEG_OP_GOTO,           94),
  /*     91 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*     92 */ CHPEG_INST(CHPEG_OP_GOTO,           94),
  /*     93 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*     94 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*     95 */ CHPEG_INST(CHPEG_OP_ISUCC,           6), // Repeat
  /*     96 */ CHPEG_INST(CHPEG_OP_IFAIL,           6), // Repeat
  /*     97 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*     98 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*     99 */ CHPEG_INST(CHPEG_OP_GOTO,          113),
  /*    100 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
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
  /*    112 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    113 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    114 */ CHPEG_INST(CHPEG_OP_LIT,             4), // "("
  /*    115 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    116 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    117 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    118 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    119 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    120 */ CHPEG_INST(CHPEG_OP_LIT,             5), // ")"
  /*    121 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    122 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    123 */ CHPEG_INST(CHPEG_OP_GOTO,          125),
  /*    124 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    125 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    126 */ CHPEG_INST(CHPEG_OP_IDENT,           9), // Reference
  /*    127 */ CHPEG_INST(CHPEG_OP_GOTO,          131),
  /*    128 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    129 */ CHPEG_INST(CHPEG_OP_GOTO,          131),
  /*    130 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    131 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    132 */ CHPEG_INST(CHPEG_OP_IDENT,           8), // Mark
  /*    133 */ CHPEG_INST(CHPEG_OP_GOTO,          137),
  /*    134 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    135 */ CHPEG_INST(CHPEG_OP_GOTO,          137),
  /*    136 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    137 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    138 */ CHPEG_INST(CHPEG_OP_IDENT,          13), // Literal
  /*    139 */ CHPEG_INST(CHPEG_OP_GOTO,          143),
  /*    140 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    141 */ CHPEG_INST(CHPEG_OP_GOTO,          143),
  /*    142 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    143 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    144 */ CHPEG_INST(CHPEG_OP_IDENT,          14), // CharClass
  /*    145 */ CHPEG_INST(CHPEG_OP_GOTO,          149),
  /*    146 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    147 */ CHPEG_INST(CHPEG_OP_GOTO,          149),
  /*    148 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    149 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    150 */ CHPEG_INST(CHPEG_OP_IDENT,          15), // NCharClass
  /*    151 */ CHPEG_INST(CHPEG_OP_GOTO,          155),
  /*    152 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    153 */ CHPEG_INST(CHPEG_OP_GOTO,          155),
  /*    154 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    155 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    156 */ CHPEG_INST(CHPEG_OP_IDENT,          24), // Dot
  /*    157 */ CHPEG_INST(CHPEG_OP_GOTO,          161),
  /*    158 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    159 */ CHPEG_INST(CHPEG_OP_GOTO,          161),
  /*    160 */ CHPEG_INST(CHPEG_OP_CISUCC,        163),
  /*    161 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    162 */ CHPEG_INST(CHPEG_OP_CFAIL,         164),
  /*    163 */ CHPEG_INST(CHPEG_OP_ISUCC,           7), // Primary
  /*    164 */ CHPEG_INST(CHPEG_OP_IFAIL,           7), // Primary
  /*    165 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$"
  /*    166 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    167 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    168 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    169 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    170 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    171 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    172 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    173 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    174 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    175 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
  /*    176 */ CHPEG_INST(CHPEG_OP_GOTO,          178),
  /*    177 */ CHPEG_INST(CHPEG_OP_ISUCC,           8), // Mark
  /*    178 */ CHPEG_INST(CHPEG_OP_IFAIL,           8), // Mark
  /*    179 */ CHPEG_INST(CHPEG_OP_LIT,             6), // "$"
  /*    180 */ CHPEG_INST(CHPEG_OP_GOTO,          189),
  /*    181 */ CHPEG_INST(CHPEG_OP_IDENT,          12), // Identifier
  /*    182 */ CHPEG_INST(CHPEG_OP_GOTO,          189),
  /*    183 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    184 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    185 */ CHPEG_INST(CHPEG_OP_GOTO,          187),
  /*    186 */ CHPEG_INST(CHPEG_OP_PMATCHF,       189),
  /*    187 */ CHPEG_INST(CHPEG_OP_PNOMATS,       189),
  /*    188 */ CHPEG_INST(CHPEG_OP_ISUCC,           9), // Reference
  /*    189 */ CHPEG_INST(CHPEG_OP_IFAIL,           9), // Reference
  /*    190 */ CHPEG_INST(CHPEG_OP_LIT,             7), // "<"
  /*    191 */ CHPEG_INST(CHPEG_OP_GOTO,          201),
  /*    192 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    193 */ CHPEG_INST(CHPEG_OP_GOTO,          201),
  /*    194 */ CHPEG_INST(CHPEG_OP_IDENT,           2), // Choice
  /*    195 */ CHPEG_INST(CHPEG_OP_GOTO,          201),
  /*    196 */ CHPEG_INST(CHPEG_OP_LIT,             8), // ">"
  /*    197 */ CHPEG_INST(CHPEG_OP_GOTO,          201),
  /*    198 */ CHPEG_INST(CHPEG_OP_IDENT,          25), // S
  /*    199 */ CHPEG_INST(CHPEG_OP_GOTO,          201),
  /*    200 */ CHPEG_INST(CHPEG_OP_ISUCC,          10), // Trim
  /*    201 */ CHPEG_INST(CHPEG_OP_IFAIL,          10), // Trim
  /*    202 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    203 */ CHPEG_INST(CHPEG_OP_CHRCLS,          9), // [CSILTPR]
  /*    204 */ CHPEG_INST(CHPEG_OP_GOTO,          206),
  /*    205 */ CHPEG_INST(CHPEG_OP_RSMAT,         203),
  /*    206 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    207 */ CHPEG_INST(CHPEG_OP_ISUCC,          11), // Options
  /*    208 */ CHPEG_INST(CHPEG_OP_IFAIL,          11), // Options
  /*    209 */ CHPEG_INST(CHPEG_OP_CHRCLS,         10), // [a-zA-Z_]
  /*    210 */ CHPEG_INST(CHPEG_OP_GOTO,          217),
  /*    211 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    212 */ CHPEG_INST(CHPEG_OP_CHRCLS,         11), // [a-zA-Z_0-9]
  /*    213 */ CHPEG_INST(CHPEG_OP_GOTO,          215),
  /*    214 */ CHPEG_INST(CHPEG_OP_RSMAT,         212),
  /*    215 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    216 */ CHPEG_INST(CHPEG_OP_ISUCC,          12), // Identifier
  /*    217 */ CHPEG_INST(CHPEG_OP_IFAIL,          12), // Identifier
  /*    218 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    219 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    220 */ CHPEG_INST(CHPEG_OP_GOTO,          234),
  /*    221 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    222 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    223 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    224 */ CHPEG_INST(CHPEG_OP_GOTO,          226),
  /*    225 */ CHPEG_INST(CHPEG_OP_PMATCHF,       230),
  /*    226 */ CHPEG_INST(CHPEG_OP_PNOMATS,       230),
  /*    227 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    228 */ CHPEG_INST(CHPEG_OP_GOTO,          230),
  /*    229 */ CHPEG_INST(CHPEG_OP_RSMAT,         222),
  /*    230 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    231 */ CHPEG_INST(CHPEG_OP_CHRCLS,         12), // [']
  /*    232 */ CHPEG_INST(CHPEG_OP_GOTO,          234),
  /*    233 */ CHPEG_INST(CHPEG_OP_CISUCC,        252),
  /*    234 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    235 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    236 */ CHPEG_INST(CHPEG_OP_GOTO,          250),
  /*    237 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    238 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    239 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    240 */ CHPEG_INST(CHPEG_OP_GOTO,          242),
  /*    241 */ CHPEG_INST(CHPEG_OP_PMATCHF,       246),
  /*    242 */ CHPEG_INST(CHPEG_OP_PNOMATS,       246),
  /*    243 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    244 */ CHPEG_INST(CHPEG_OP_GOTO,          246),
  /*    245 */ CHPEG_INST(CHPEG_OP_RSMAT,         238),
  /*    246 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    247 */ CHPEG_INST(CHPEG_OP_CHRCLS,         13), // [\"]
  /*    248 */ CHPEG_INST(CHPEG_OP_GOTO,          250),
  /*    249 */ CHPEG_INST(CHPEG_OP_CISUCC,        252),
  /*    250 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    251 */ CHPEG_INST(CHPEG_OP_CFAIL,         253),
  /*    252 */ CHPEG_INST(CHPEG_OP_ISUCC,          13), // Literal
  /*    253 */ CHPEG_INST(CHPEG_OP_IFAIL,          13), // Literal
  /*    254 */ CHPEG_INST(CHPEG_OP_LIT,            14), // "["
  /*    255 */ CHPEG_INST(CHPEG_OP_GOTO,          274),
  /*    256 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    257 */ CHPEG_INST(CHPEG_OP_LIT,            15), // "^"
  /*    258 */ CHPEG_INST(CHPEG_OP_GOTO,          260),
  /*    259 */ CHPEG_INST(CHPEG_OP_PMATCHF,       274),
  /*    260 */ CHPEG_INST(CHPEG_OP_PNOMATS,       274),
  /*    261 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    262 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    263 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    264 */ CHPEG_INST(CHPEG_OP_GOTO,          266),
  /*    265 */ CHPEG_INST(CHPEG_OP_PMATCHF,       270),
  /*    266 */ CHPEG_INST(CHPEG_OP_PNOMATS,       270),
  /*    267 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // CharRange
  /*    268 */ CHPEG_INST(CHPEG_OP_GOTO,          270),
  /*    269 */ CHPEG_INST(CHPEG_OP_RPMAT,         262),
  /*    270 */ CHPEG_INST(CHPEG_OP_RPDONE,        274),
  /*    271 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    272 */ CHPEG_INST(CHPEG_OP_GOTO,          274),
  /*    273 */ CHPEG_INST(CHPEG_OP_ISUCC,          14), // CharClass
  /*    274 */ CHPEG_INST(CHPEG_OP_IFAIL,          14), // CharClass
  /*    275 */ CHPEG_INST(CHPEG_OP_LIT,            17), // "[^"
  /*    276 */ CHPEG_INST(CHPEG_OP_GOTO,          290),
  /*    277 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    278 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    279 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    280 */ CHPEG_INST(CHPEG_OP_GOTO,          282),
  /*    281 */ CHPEG_INST(CHPEG_OP_PMATCHF,       286),
  /*    282 */ CHPEG_INST(CHPEG_OP_PNOMATS,       286),
  /*    283 */ CHPEG_INST(CHPEG_OP_IDENT,          16), // CharRange
  /*    284 */ CHPEG_INST(CHPEG_OP_GOTO,          286),
  /*    285 */ CHPEG_INST(CHPEG_OP_RPMAT,         278),
  /*    286 */ CHPEG_INST(CHPEG_OP_RPDONE,        290),
  /*    287 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    288 */ CHPEG_INST(CHPEG_OP_GOTO,          290),
  /*    289 */ CHPEG_INST(CHPEG_OP_ISUCC,          15), // NCharClass
  /*    290 */ CHPEG_INST(CHPEG_OP_IFAIL,          15), // NCharClass
  /*    291 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    292 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    293 */ CHPEG_INST(CHPEG_OP_GOTO,          304),
  /*    294 */ CHPEG_INST(CHPEG_OP_LIT,            18), // "-"
  /*    295 */ CHPEG_INST(CHPEG_OP_GOTO,          304),
  /*    296 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    297 */ CHPEG_INST(CHPEG_OP_LIT,            16), // "]"
  /*    298 */ CHPEG_INST(CHPEG_OP_GOTO,          300),
  /*    299 */ CHPEG_INST(CHPEG_OP_PMATCHF,       304),
  /*    300 */ CHPEG_INST(CHPEG_OP_PNOMATS,       304),
  /*    301 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    302 */ CHPEG_INST(CHPEG_OP_GOTO,          304),
  /*    303 */ CHPEG_INST(CHPEG_OP_CISUCC,        310),
  /*    304 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    305 */ CHPEG_INST(CHPEG_OP_IDENT,          17), // Char
  /*    306 */ CHPEG_INST(CHPEG_OP_GOTO,          308),
  /*    307 */ CHPEG_INST(CHPEG_OP_CISUCC,        310),
  /*    308 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    309 */ CHPEG_INST(CHPEG_OP_CFAIL,         311),
  /*    310 */ CHPEG_INST(CHPEG_OP_ISUCC,          16), // CharRange
  /*    311 */ CHPEG_INST(CHPEG_OP_IFAIL,          16), // CharRange
  /*    312 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    313 */ CHPEG_INST(CHPEG_OP_IDENT,          21), // PlainChar
  /*    314 */ CHPEG_INST(CHPEG_OP_GOTO,          316),
  /*    315 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    316 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    317 */ CHPEG_INST(CHPEG_OP_IDENT,          20), // HexChar
  /*    318 */ CHPEG_INST(CHPEG_OP_GOTO,          320),
  /*    319 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    320 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    321 */ CHPEG_INST(CHPEG_OP_IDENT,          18), // EscChar
  /*    322 */ CHPEG_INST(CHPEG_OP_GOTO,          324),
  /*    323 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    324 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    325 */ CHPEG_INST(CHPEG_OP_IDENT,          19), // OctChar
  /*    326 */ CHPEG_INST(CHPEG_OP_GOTO,          328),
  /*    327 */ CHPEG_INST(CHPEG_OP_CISUCC,        330),
  /*    328 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    329 */ CHPEG_INST(CHPEG_OP_CFAIL,         331),
  /*    330 */ CHPEG_INST(CHPEG_OP_ISUCC,          17), // Char
  /*    331 */ CHPEG_INST(CHPEG_OP_IFAIL,          17), // Char
  /*    332 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "\\"
  /*    333 */ CHPEG_INST(CHPEG_OP_GOTO,          337),
  /*    334 */ CHPEG_INST(CHPEG_OP_CHRCLS,         20), // [abfvnrt'\"[]\\]
  /*    335 */ CHPEG_INST(CHPEG_OP_GOTO,          337),
  /*    336 */ CHPEG_INST(CHPEG_OP_ISUCC,          18), // EscChar
  /*    337 */ CHPEG_INST(CHPEG_OP_IFAIL,          18), // EscChar
  /*    338 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    339 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "\\"
  /*    340 */ CHPEG_INST(CHPEG_OP_GOTO,          348),
  /*    341 */ CHPEG_INST(CHPEG_OP_CHRCLS,         21), // [0-3]
  /*    342 */ CHPEG_INST(CHPEG_OP_GOTO,          348),
  /*    343 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-7]
  /*    344 */ CHPEG_INST(CHPEG_OP_GOTO,          348),
  /*    345 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-7]
  /*    346 */ CHPEG_INST(CHPEG_OP_GOTO,          348),
  /*    347 */ CHPEG_INST(CHPEG_OP_CISUCC,        361),
  /*    348 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    349 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "\\"
  /*    350 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    351 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-7]
  /*    352 */ CHPEG_INST(CHPEG_OP_GOTO,          359),
  /*    353 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    354 */ CHPEG_INST(CHPEG_OP_CHRCLS,         22), // [0-7]
  /*    355 */ CHPEG_INST(CHPEG_OP_GOTO,          357),
  /*    356 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    357 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    358 */ CHPEG_INST(CHPEG_OP_CISUCC,        361),
  /*    359 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    360 */ CHPEG_INST(CHPEG_OP_CFAIL,         362),
  /*    361 */ CHPEG_INST(CHPEG_OP_ISUCC,          19), // OctChar
  /*    362 */ CHPEG_INST(CHPEG_OP_IFAIL,          19), // OctChar
  /*    363 */ CHPEG_INST(CHPEG_OP_LIT,            23), // "\\x"
  /*    364 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    365 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [0-9a-fA-F]
  /*    366 */ CHPEG_INST(CHPEG_OP_GOTO,          373),
  /*    367 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    368 */ CHPEG_INST(CHPEG_OP_CHRCLS,         24), // [0-9a-fA-F]
  /*    369 */ CHPEG_INST(CHPEG_OP_GOTO,          371),
  /*    370 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    371 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    372 */ CHPEG_INST(CHPEG_OP_ISUCC,          20), // HexChar
  /*    373 */ CHPEG_INST(CHPEG_OP_IFAIL,          20), // HexChar
  /*    374 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    375 */ CHPEG_INST(CHPEG_OP_LIT,            19), // "\\"
  /*    376 */ CHPEG_INST(CHPEG_OP_GOTO,          378),
  /*    377 */ CHPEG_INST(CHPEG_OP_PMATCHF,       381),
  /*    378 */ CHPEG_INST(CHPEG_OP_PNOMATS,       381),
  /*    379 */ CHPEG_INST(CHPEG_OP_DOT,           381),
  /*    380 */ CHPEG_INST(CHPEG_OP_ISUCC,          21), // PlainChar
  /*    381 */ CHPEG_INST(CHPEG_OP_IFAIL,          21), // PlainChar
  /*    382 */ CHPEG_INST(CHPEG_OP_CHRCLS,         25), // [&!]
  /*    383 */ CHPEG_INST(CHPEG_OP_GOTO,          385),
  /*    384 */ CHPEG_INST(CHPEG_OP_ISUCC,          22), // PredOp
  /*    385 */ CHPEG_INST(CHPEG_OP_IFAIL,          22), // PredOp
  /*    386 */ CHPEG_INST(CHPEG_OP_CHRCLS,         26), // [*+?]
  /*    387 */ CHPEG_INST(CHPEG_OP_GOTO,          389),
  /*    388 */ CHPEG_INST(CHPEG_OP_ISUCC,          23), // RepOp
  /*    389 */ CHPEG_INST(CHPEG_OP_IFAIL,          23), // RepOp
  /*    390 */ CHPEG_INST(CHPEG_OP_LIT,            27), // "."
  /*    391 */ CHPEG_INST(CHPEG_OP_GOTO,          393),
  /*    392 */ CHPEG_INST(CHPEG_OP_ISUCC,          24), // Dot
  /*    393 */ CHPEG_INST(CHPEG_OP_IFAIL,          24), // Dot
  /*    394 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    395 */ CHPEG_INST(CHPEG_OP_CHOICE,          0),
  /*    396 */ CHPEG_INST(CHPEG_OP_RPBEG,           0),
  /*    397 */ CHPEG_INST(CHPEG_OP_CHRCLS,         28), // [ \t\r\n]
  /*    398 */ CHPEG_INST(CHPEG_OP_GOTO,          400),
  /*    399 */ CHPEG_INST(CHPEG_OP_RPMAT,         397),
  /*    400 */ CHPEG_INST(CHPEG_OP_RPDONE,        402),
  /*    401 */ CHPEG_INST(CHPEG_OP_CISUCC,        422),
  /*    402 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    403 */ CHPEG_INST(CHPEG_OP_LIT,            29), // "#"
  /*    404 */ CHPEG_INST(CHPEG_OP_GOTO,          420),
  /*    405 */ CHPEG_INST(CHPEG_OP_RSBEG,           0),
  /*    406 */ CHPEG_INST(CHPEG_OP_PREDN,           0),
  /*    407 */ CHPEG_INST(CHPEG_OP_CHRCLS,         30), // [\r\n]
  /*    408 */ CHPEG_INST(CHPEG_OP_GOTO,          410),
  /*    409 */ CHPEG_INST(CHPEG_OP_PMATCHF,       413),
  /*    410 */ CHPEG_INST(CHPEG_OP_PNOMATS,       413),
  /*    411 */ CHPEG_INST(CHPEG_OP_DOT,           413),
  /*    412 */ CHPEG_INST(CHPEG_OP_RSMAT,         406),
  /*    413 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    414 */ CHPEG_INST(CHPEG_OP_RQBEG,           0),
  /*    415 */ CHPEG_INST(CHPEG_OP_CHRCLS,         30), // [\r\n]
  /*    416 */ CHPEG_INST(CHPEG_OP_GOTO,          418),
  /*    417 */ CHPEG_INST(CHPEG_OP_RQMAT,           0),
  /*    418 */ CHPEG_INST(CHPEG_OP_RQDONE,          0),
  /*    419 */ CHPEG_INST(CHPEG_OP_CISUCC,        422),
  /*    420 */ CHPEG_INST(CHPEG_OP_CIFAIL,          0),
  /*    421 */ CHPEG_INST(CHPEG_OP_CFAIL,         423),
  /*    422 */ CHPEG_INST(CHPEG_OP_RSMAT,         395),
  /*    423 */ CHPEG_INST(CHPEG_OP_RSDONE,          0),
  /*    424 */ CHPEG_INST(CHPEG_OP_ISUCC,          25), // S
  /*    425 */ CHPEG_INST(CHPEG_OP_IFAIL,          25), // S
  };

const char *chpeg_ext_bytecode_strings[31] = {"{", "}", "<-", "/", "(", ")", "$", "<", ">", "CSILTPR", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "^", "]", "[^", "-", "\\", "abfvnrt'\"[]\\", "0-3", "0-7", "\\x", "0-9a-fA-F", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"};

int chpeg_ext_bytecode_str_len[31] = {1, 1, 2, 1, 1, 1, 1, 1, 1, 7, 7, 10, 1, 1, 1, 1, 1, 2, 1, 1, 12, 3, 3, 2, 9, 2, 3, 1, 4, 1, 2};

const ChpegByteCode chpeg_ext_bytecode = {
  26, // num_defs
  (char **)chpeg_ext_bytecode_def_names,
  chpeg_ext_bytecode_def_flags,
  chpeg_ext_bytecode_def_addrs,
  426, // num_instructions
  chpeg_ext_bytecode_instructions,
  31, // num_strings
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
