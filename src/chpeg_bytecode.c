//
// experimental trim op bytecode
//
#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_bytecode.h"
#endif

CHPEG_DEF const ChpegByteCode chpeg_bytecode = {
  .num_defs = 21,
  .def_names = (char*[21]) {"Grammar", "Definition", "Choice", "Sequence", "Predicate", "Repeat", "Primary", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "TrimOp", "Dot", "S"},
  .def_flags = (int[21]) {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE},
  .def_addrs = (int[21]) {2, 15, 42, 55, 72, 83, 94, 139, 146, 155, 191, 207, 228, 244, 250, 275, 283, 287, 291, 295, 299}, // presubtracted by 1
  .num_instructions = 329,
  .instructions = (int[329]) {
  /*     0 */ CHPEG_INST(CHPEG_OP_IDENT       ,        0), /* Grammar */
  /*     1 */ CHPEG_INST(CHPEG_OP_FAIL        ,        0),
  /*     2 */ CHPEG_INST(CHPEG_OP_SUCC        ,        0),
  /*     3 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*     4 */ CHPEG_INST(CHPEG_OP_GOTO        ,       14),
  /*     5 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*     6 */ CHPEG_INST(CHPEG_OP_IDENT       ,        1), /* Definition */
  /*     7 */ CHPEG_INST(CHPEG_OP_GOTO        ,        8),
  /*     8 */ CHPEG_INST(CHPEG_OP_RPMAT       ,        5),
  /*     9 */ CHPEG_INST(CHPEG_OP_RPDONE      ,       14),
  /*    10 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*    11 */ CHPEG_INST(CHPEG_OP_DOT         ,       12),
  /*    12 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,       14),
  /*    13 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,       14),
  /*    14 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        0), /* Grammar */
  /*    15 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        0), /* Grammar */
  /*    16 */ CHPEG_INST(CHPEG_OP_IDENT       ,        8), /* Identifier */
  /*    17 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    18 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    19 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    20 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    21 */ CHPEG_INST(CHPEG_OP_LIT         ,        0), /* "{" */
  /*    22 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    23 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    24 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    25 */ CHPEG_INST(CHPEG_OP_IDENT       ,        7), /* Options */
  /*    26 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    27 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    28 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    29 */ CHPEG_INST(CHPEG_OP_LIT         ,        1), /* "}" */
  /*    30 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    31 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    32 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    33 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    34 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    35 */ CHPEG_INST(CHPEG_OP_LIT         ,        2), /* "<-" */
  /*    36 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    37 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    38 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    39 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*    40 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    41 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        1), /* Definition */
  /*    42 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        1), /* Definition */
  /*    43 */ CHPEG_INST(CHPEG_OP_IDENT       ,        3), /* Sequence */
  /*    44 */ CHPEG_INST(CHPEG_OP_GOTO        ,       54),
  /*    45 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*    46 */ CHPEG_INST(CHPEG_OP_LIT         ,        3), /* "/" */
  /*    47 */ CHPEG_INST(CHPEG_OP_GOTO        ,       52),
  /*    48 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    49 */ CHPEG_INST(CHPEG_OP_GOTO        ,       52),
  /*    50 */ CHPEG_INST(CHPEG_OP_IDENT       ,        3), /* Sequence */
  /*    51 */ CHPEG_INST(CHPEG_OP_GOTO        ,       52),
  /*    52 */ CHPEG_INST(CHPEG_OP_RSMAT       ,       45),
  /*    53 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*    54 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        2), /* Choice */
  /*    55 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        2), /* Choice */
  /*    56 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*    57 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*    58 */ CHPEG_INST(CHPEG_OP_IDENT       ,       18), /* TrimOp */
  /*    59 */ CHPEG_INST(CHPEG_OP_GOTO        ,       62),
  /*    60 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    61 */ CHPEG_INST(CHPEG_OP_GOTO        ,       62),
  /*    62 */ CHPEG_INST(CHPEG_OP_CISUCC      ,       68),
  /*    63 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*    64 */ CHPEG_INST(CHPEG_OP_IDENT       ,        4), /* Predicate */
  /*    65 */ CHPEG_INST(CHPEG_OP_GOTO        ,       66),
  /*    66 */ CHPEG_INST(CHPEG_OP_CISUCC      ,       68),
  /*    67 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*    68 */ CHPEG_INST(CHPEG_OP_CFAIL       ,       69),
  /*    69 */ CHPEG_INST(CHPEG_OP_RPMAT       ,       56),
  /*    70 */ CHPEG_INST(CHPEG_OP_RPDONE      ,       71),
  /*    71 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        3), /* Sequence */
  /*    72 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        3), /* Sequence */
  /*    73 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    74 */ CHPEG_INST(CHPEG_OP_IDENT       ,       16), /* PredOp */
  /*    75 */ CHPEG_INST(CHPEG_OP_GOTO        ,       78),
  /*    76 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    77 */ CHPEG_INST(CHPEG_OP_GOTO        ,       78),
  /*    78 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    79 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    80 */ CHPEG_INST(CHPEG_OP_IDENT       ,        5), /* Repeat */
  /*    81 */ CHPEG_INST(CHPEG_OP_GOTO        ,       82),
  /*    82 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        4), /* Predicate */
  /*    83 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        4), /* Predicate */
  /*    84 */ CHPEG_INST(CHPEG_OP_IDENT       ,        6), /* Primary */
  /*    85 */ CHPEG_INST(CHPEG_OP_GOTO        ,       93),
  /*    86 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    87 */ CHPEG_INST(CHPEG_OP_IDENT       ,       17), /* RepOp */
  /*    88 */ CHPEG_INST(CHPEG_OP_GOTO        ,       91),
  /*    89 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    90 */ CHPEG_INST(CHPEG_OP_GOTO        ,       91),
  /*    91 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    92 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    93 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        5), /* Repeat */
  /*    94 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        5), /* Repeat */
  /*    95 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*    96 */ CHPEG_INST(CHPEG_OP_IDENT       ,        8), /* Identifier */
  /*    97 */ CHPEG_INST(CHPEG_OP_GOTO        ,      105),
  /*    98 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    99 */ CHPEG_INST(CHPEG_OP_GOTO        ,      105),
  /*   100 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   101 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        4), /* "{<" */
  /*   102 */ CHPEG_INST(CHPEG_OP_GOTO        ,      103),
  /*   103 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      105),
  /*   104 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      105),
  /*   105 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   106 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   107 */ CHPEG_INST(CHPEG_OP_LIT         ,        5), /* "(" */
  /*   108 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   109 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   110 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   111 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*   112 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   113 */ CHPEG_INST(CHPEG_OP_LIT         ,        6), /* ")" */
  /*   114 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   115 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   116 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   117 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   118 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   119 */ CHPEG_INST(CHPEG_OP_IDENT       ,        9), /* Literal */
  /*   120 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   121 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   122 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   123 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   124 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   125 */ CHPEG_INST(CHPEG_OP_IDENT       ,       10), /* CharClass */
  /*   126 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   127 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   128 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   129 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   130 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   131 */ CHPEG_INST(CHPEG_OP_IDENT       ,       19), /* Dot */
  /*   132 */ CHPEG_INST(CHPEG_OP_GOTO        ,      135),
  /*   133 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   134 */ CHPEG_INST(CHPEG_OP_GOTO        ,      135),
  /*   135 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   136 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   137 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      138),
  /*   138 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        6), /* Primary */
  /*   139 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        6), /* Primary */
  /*   140 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   141 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        7), /* "ILS" */
  /*   142 */ CHPEG_INST(CHPEG_OP_GOTO        ,      143),
  /*   143 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      140),
  /*   144 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   145 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        7), /* Options */
  /*   146 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        7), /* Options */
  /*   147 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        8), /* "a-zA-Z_" */
  /*   148 */ CHPEG_INST(CHPEG_OP_GOTO        ,      154),
  /*   149 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   150 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        9), /* "a-zA-Z_0-9" */
  /*   151 */ CHPEG_INST(CHPEG_OP_GOTO        ,      152),
  /*   152 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      149),
  /*   153 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   154 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        8), /* Identifier */
  /*   155 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        8), /* Identifier */
  /*   156 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   157 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       10), /* "'" */
  /*   158 */ CHPEG_INST(CHPEG_OP_GOTO        ,      171),
  /*   159 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   160 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   161 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       10), /* "'" */
  /*   162 */ CHPEG_INST(CHPEG_OP_GOTO        ,      163),
  /*   163 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      167),
  /*   164 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      167),
  /*   165 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* Char */
  /*   166 */ CHPEG_INST(CHPEG_OP_GOTO        ,      167),
  /*   167 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      159),
  /*   168 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   169 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       10), /* "'" */
  /*   170 */ CHPEG_INST(CHPEG_OP_GOTO        ,      171),
  /*   171 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      189),
  /*   172 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   173 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "\"" */
  /*   174 */ CHPEG_INST(CHPEG_OP_GOTO        ,      187),
  /*   175 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   176 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   177 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "\"" */
  /*   178 */ CHPEG_INST(CHPEG_OP_GOTO        ,      179),
  /*   179 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      183),
  /*   180 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      183),
  /*   181 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* Char */
  /*   182 */ CHPEG_INST(CHPEG_OP_GOTO        ,      183),
  /*   183 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      175),
  /*   184 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   185 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "\"" */
  /*   186 */ CHPEG_INST(CHPEG_OP_GOTO        ,      187),
  /*   187 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      189),
  /*   188 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   189 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      190),
  /*   190 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        9), /* Literal */
  /*   191 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        9), /* Literal */
  /*   192 */ CHPEG_INST(CHPEG_OP_LIT         ,       12), /* "[" */
  /*   193 */ CHPEG_INST(CHPEG_OP_GOTO        ,      206),
  /*   194 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   195 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   196 */ CHPEG_INST(CHPEG_OP_LIT         ,       13), /* "]" */
  /*   197 */ CHPEG_INST(CHPEG_OP_GOTO        ,      198),
  /*   198 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      202),
  /*   199 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      202),
  /*   200 */ CHPEG_INST(CHPEG_OP_IDENT       ,       11), /* CharRange */
  /*   201 */ CHPEG_INST(CHPEG_OP_GOTO        ,      202),
  /*   202 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      194),
  /*   203 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      206),
  /*   204 */ CHPEG_INST(CHPEG_OP_LIT         ,       13), /* "]" */
  /*   205 */ CHPEG_INST(CHPEG_OP_GOTO        ,      206),
  /*   206 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       10), /* CharClass */
  /*   207 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       10), /* CharClass */
  /*   208 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   209 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* Char */
  /*   210 */ CHPEG_INST(CHPEG_OP_GOTO        ,      220),
  /*   211 */ CHPEG_INST(CHPEG_OP_LIT         ,       14), /* "-" */
  /*   212 */ CHPEG_INST(CHPEG_OP_GOTO        ,      220),
  /*   213 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   214 */ CHPEG_INST(CHPEG_OP_LIT         ,       13), /* "]" */
  /*   215 */ CHPEG_INST(CHPEG_OP_GOTO        ,      216),
  /*   216 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      220),
  /*   217 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      220),
  /*   218 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* Char */
  /*   219 */ CHPEG_INST(CHPEG_OP_GOTO        ,      220),
  /*   220 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      226),
  /*   221 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   222 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* Char */
  /*   223 */ CHPEG_INST(CHPEG_OP_GOTO        ,      224),
  /*   224 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      226),
  /*   225 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   226 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      227),
  /*   227 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       11), /* CharRange */
  /*   228 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       11), /* CharRange */
  /*   229 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   230 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* EscChar */
  /*   231 */ CHPEG_INST(CHPEG_OP_GOTO        ,      232),
  /*   232 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      242),
  /*   233 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   234 */ CHPEG_INST(CHPEG_OP_IDENT       ,       14), /* OctChar */
  /*   235 */ CHPEG_INST(CHPEG_OP_GOTO        ,      236),
  /*   236 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      242),
  /*   237 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   238 */ CHPEG_INST(CHPEG_OP_IDENT       ,       15), /* PlainChar */
  /*   239 */ CHPEG_INST(CHPEG_OP_GOTO        ,      240),
  /*   240 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      242),
  /*   241 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   242 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      243),
  /*   243 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       12), /* Char */
  /*   244 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       12), /* Char */
  /*   245 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "\\" */
  /*   246 */ CHPEG_INST(CHPEG_OP_GOTO        ,      249),
  /*   247 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       16), /* "nrt'\"[]\\" */
  /*   248 */ CHPEG_INST(CHPEG_OP_GOTO        ,      249),
  /*   249 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       13), /* EscChar */
  /*   250 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       13), /* EscChar */
  /*   251 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   252 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "\\" */
  /*   253 */ CHPEG_INST(CHPEG_OP_GOTO        ,      260),
  /*   254 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       17), /* "0-3" */
  /*   255 */ CHPEG_INST(CHPEG_OP_GOTO        ,      260),
  /*   256 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "0-7" */
  /*   257 */ CHPEG_INST(CHPEG_OP_GOTO        ,      260),
  /*   258 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "0-7" */
  /*   259 */ CHPEG_INST(CHPEG_OP_GOTO        ,      260),
  /*   260 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      273),
  /*   261 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   262 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "\\" */
  /*   263 */ CHPEG_INST(CHPEG_OP_GOTO        ,      271),
  /*   264 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "0-7" */
  /*   265 */ CHPEG_INST(CHPEG_OP_GOTO        ,      271),
  /*   266 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*   267 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "0-7" */
  /*   268 */ CHPEG_INST(CHPEG_OP_GOTO        ,      269),
  /*   269 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*   270 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*   271 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      273),
  /*   272 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   273 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      274),
  /*   274 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       14), /* OctChar */
  /*   275 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       14), /* OctChar */
  /*   276 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   277 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "\\" */
  /*   278 */ CHPEG_INST(CHPEG_OP_GOTO        ,      279),
  /*   279 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      282),
  /*   280 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      282),
  /*   281 */ CHPEG_INST(CHPEG_OP_DOT         ,      282),
  /*   282 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       15), /* PlainChar */
  /*   283 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       15), /* PlainChar */
  /*   284 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "&!" */
  /*   285 */ CHPEG_INST(CHPEG_OP_GOTO        ,      286),
  /*   286 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       16), /* PredOp */
  /*   287 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       16), /* PredOp */
  /*   288 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "*+?" */
  /*   289 */ CHPEG_INST(CHPEG_OP_GOTO        ,      290),
  /*   290 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       17), /* RepOp */
  /*   291 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       17), /* RepOp */
  /*   292 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       21), /* "<>" */
  /*   293 */ CHPEG_INST(CHPEG_OP_GOTO        ,      294),
  /*   294 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       18), /* TrimOp */
  /*   295 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       18), /* TrimOp */
  /*   296 */ CHPEG_INST(CHPEG_OP_LIT         ,       22), /* "." */
  /*   297 */ CHPEG_INST(CHPEG_OP_GOTO        ,      298),
  /*   298 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       19), /* Dot */
  /*   299 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       19), /* Dot */
  /*   300 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   301 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   302 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   303 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       23), /* " \t\r\n" */
  /*   304 */ CHPEG_INST(CHPEG_OP_GOTO        ,      305),
  /*   305 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      302),
  /*   306 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      307),
  /*   307 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      324),
  /*   308 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   309 */ CHPEG_INST(CHPEG_OP_LIT         ,       24), /* "#" */
  /*   310 */ CHPEG_INST(CHPEG_OP_GOTO        ,      322),
  /*   311 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   312 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   313 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       25), /* "\r\n" */
  /*   314 */ CHPEG_INST(CHPEG_OP_GOTO        ,      315),
  /*   315 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      318),
  /*   316 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      318),
  /*   317 */ CHPEG_INST(CHPEG_OP_DOT         ,      318),
  /*   318 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      311),
  /*   319 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   320 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       25), /* "\r\n" */
  /*   321 */ CHPEG_INST(CHPEG_OP_GOTO        ,      322),
  /*   322 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      324),
  /*   323 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   324 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      325),
  /*   325 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      300),
  /*   326 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   327 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       20), /* S */
  /*   328 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       20), /* S */
  },
  .num_strings = 26,
  .strings = (unsigned char**)(char*[26]) {"{", "}", "<-", "/", "{<", "(", ")", "ILS", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", "<>", ".", " \t\r\n", "#", "\r\n"},
  .str_len = (int[26]) {1, 1, 2, 1, 2, 1, 1, 3, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 2, 1, 4, 1, 2}
};
