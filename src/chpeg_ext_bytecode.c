#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_ext_bytecode.h"
#endif

CHPEG_DEF const ChpegByteCode chpeg_bytecode = {
  .num_defs = 21,
  .def_names = (char*[21]) {"Grammar", "Definition", "Choice", "Sequence", "Predicate", "Repeat", "Primary", "Trim", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"},
  .def_flags = (int[21]) {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE},
  .def_addrs = (int[21]) {3, 16, 43, 56, 63, 74, 85, 139, 151, 158, 167, 203, 219, 240, 256, 262, 287, 295, 299, 303, 307},
  .num_instructions = 339,
  .instructions = (int[339]) {
  /*     0 */ CHPEG_INST(CHPEG_OP_IDENT       ,        0), /* Grammar */
  /*     1 */ CHPEG_INST(CHPEG_OP_FAIL        ,        0),
  /*     2 */ CHPEG_INST(CHPEG_OP_SUCC        ,        0),
  /*     3 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*     4 */ CHPEG_INST(CHPEG_OP_GOTO        ,       15),
  /*     5 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*     6 */ CHPEG_INST(CHPEG_OP_IDENT       ,        1), /* Definition */
  /*     7 */ CHPEG_INST(CHPEG_OP_GOTO        ,        9),
  /*     8 */ CHPEG_INST(CHPEG_OP_RPMAT       ,        6),
  /*     9 */ CHPEG_INST(CHPEG_OP_RPDONE      ,       15),
  /*    10 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*    11 */ CHPEG_INST(CHPEG_OP_DOT         ,       13),
  /*    12 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,       15),
  /*    13 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,       15),
  /*    14 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        0), /* Grammar */
  /*    15 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        0), /* Grammar */
  /*    16 */ CHPEG_INST(CHPEG_OP_IDENT       ,        9), /* Identifier */
  /*    17 */ CHPEG_INST(CHPEG_OP_GOTO        ,       42),
  /*    18 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    19 */ CHPEG_INST(CHPEG_OP_GOTO        ,       42),
  /*    20 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    21 */ CHPEG_INST(CHPEG_OP_LIT         ,        0), /* "{" */
  /*    22 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    23 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    24 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    25 */ CHPEG_INST(CHPEG_OP_IDENT       ,        8), /* Options */
  /*    26 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    27 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    28 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    29 */ CHPEG_INST(CHPEG_OP_LIT         ,        1), /* "}" */
  /*    30 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    31 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    32 */ CHPEG_INST(CHPEG_OP_GOTO        ,       34),
  /*    33 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    34 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    35 */ CHPEG_INST(CHPEG_OP_LIT         ,        2), /* "<-" */
  /*    36 */ CHPEG_INST(CHPEG_OP_GOTO        ,       42),
  /*    37 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    38 */ CHPEG_INST(CHPEG_OP_GOTO        ,       42),
  /*    39 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*    40 */ CHPEG_INST(CHPEG_OP_GOTO        ,       42),
  /*    41 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        1), /* Definition */
  /*    42 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        1), /* Definition */
  /*    43 */ CHPEG_INST(CHPEG_OP_IDENT       ,        3), /* Sequence */
  /*    44 */ CHPEG_INST(CHPEG_OP_GOTO        ,       55),
  /*    45 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*    46 */ CHPEG_INST(CHPEG_OP_LIT         ,        3), /* "/" */
  /*    47 */ CHPEG_INST(CHPEG_OP_GOTO        ,       53),
  /*    48 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    49 */ CHPEG_INST(CHPEG_OP_GOTO        ,       53),
  /*    50 */ CHPEG_INST(CHPEG_OP_IDENT       ,        3), /* Sequence */
  /*    51 */ CHPEG_INST(CHPEG_OP_GOTO        ,       53),
  /*    52 */ CHPEG_INST(CHPEG_OP_RSMAT       ,       46),
  /*    53 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*    54 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        2), /* Choice */
  /*    55 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        2), /* Choice */
  /*    56 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*    57 */ CHPEG_INST(CHPEG_OP_IDENT       ,        4), /* Predicate */
  /*    58 */ CHPEG_INST(CHPEG_OP_GOTO        ,       60),
  /*    59 */ CHPEG_INST(CHPEG_OP_RPMAT       ,       57),
  /*    60 */ CHPEG_INST(CHPEG_OP_RPDONE      ,       62),
  /*    61 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        3), /* Sequence */
  /*    62 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        3), /* Sequence */
  /*    63 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    64 */ CHPEG_INST(CHPEG_OP_IDENT       ,       17), /* PredOp */
  /*    65 */ CHPEG_INST(CHPEG_OP_GOTO        ,       69),
  /*    66 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    67 */ CHPEG_INST(CHPEG_OP_GOTO        ,       69),
  /*    68 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    69 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    70 */ CHPEG_INST(CHPEG_OP_IDENT       ,        5), /* Repeat */
  /*    71 */ CHPEG_INST(CHPEG_OP_GOTO        ,       73),
  /*    72 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        4), /* Predicate */
  /*    73 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        4), /* Predicate */
  /*    74 */ CHPEG_INST(CHPEG_OP_IDENT       ,        6), /* Primary */
  /*    75 */ CHPEG_INST(CHPEG_OP_GOTO        ,       84),
  /*    76 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    77 */ CHPEG_INST(CHPEG_OP_IDENT       ,       18), /* RepOp */
  /*    78 */ CHPEG_INST(CHPEG_OP_GOTO        ,       82),
  /*    79 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    80 */ CHPEG_INST(CHPEG_OP_GOTO        ,       82),
  /*    81 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    82 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    83 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        5), /* Repeat */
  /*    84 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        5), /* Repeat */
  /*    85 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*    86 */ CHPEG_INST(CHPEG_OP_IDENT       ,        9), /* Identifier */
  /*    87 */ CHPEG_INST(CHPEG_OP_GOTO        ,      101),
  /*    88 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    89 */ CHPEG_INST(CHPEG_OP_GOTO        ,      101),
  /*    90 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*    91 */ CHPEG_INST(CHPEG_OP_LIT         ,        0), /* "{" */
  /*    92 */ CHPEG_INST(CHPEG_OP_GOTO        ,       94),
  /*    93 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      101),
  /*    94 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      101),
  /*    95 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*    96 */ CHPEG_INST(CHPEG_OP_LIT         ,        2), /* "<-" */
  /*    97 */ CHPEG_INST(CHPEG_OP_GOTO        ,       99),
  /*    98 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      101),
  /*    99 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      101),
  /*   100 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   101 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   102 */ CHPEG_INST(CHPEG_OP_LIT         ,        4), /* "(" */
  /*   103 */ CHPEG_INST(CHPEG_OP_GOTO        ,      113),
  /*   104 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   105 */ CHPEG_INST(CHPEG_OP_GOTO        ,      113),
  /*   106 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*   107 */ CHPEG_INST(CHPEG_OP_GOTO        ,      113),
  /*   108 */ CHPEG_INST(CHPEG_OP_LIT         ,        5), /* ")" */
  /*   109 */ CHPEG_INST(CHPEG_OP_GOTO        ,      113),
  /*   110 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   111 */ CHPEG_INST(CHPEG_OP_GOTO        ,      113),
  /*   112 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   113 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   114 */ CHPEG_INST(CHPEG_OP_IDENT       ,        7), /* Trim */
  /*   115 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   116 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   117 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   118 */ CHPEG_INST(CHPEG_OP_IDENT       ,       10), /* Literal */
  /*   119 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   120 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   121 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   122 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   123 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   124 */ CHPEG_INST(CHPEG_OP_IDENT       ,       11), /* CharClass */
  /*   125 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   126 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   127 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   128 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   129 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   130 */ CHPEG_INST(CHPEG_OP_IDENT       ,       19), /* Dot */
  /*   131 */ CHPEG_INST(CHPEG_OP_GOTO        ,      135),
  /*   132 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   133 */ CHPEG_INST(CHPEG_OP_GOTO        ,      135),
  /*   134 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      137),
  /*   135 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   136 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      138),
  /*   137 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        6), /* Primary */
  /*   138 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        6), /* Primary */
  /*   139 */ CHPEG_INST(CHPEG_OP_LIT         ,        6), /* "<" */
  /*   140 */ CHPEG_INST(CHPEG_OP_GOTO        ,      150),
  /*   141 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   142 */ CHPEG_INST(CHPEG_OP_GOTO        ,      150),
  /*   143 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*   144 */ CHPEG_INST(CHPEG_OP_GOTO        ,      150),
  /*   145 */ CHPEG_INST(CHPEG_OP_LIT         ,        7), /* ">" */
  /*   146 */ CHPEG_INST(CHPEG_OP_GOTO        ,      150),
  /*   147 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   148 */ CHPEG_INST(CHPEG_OP_GOTO        ,      150),
  /*   149 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        7), /* Trim */
  /*   150 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        7), /* Trim */
  /*   151 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   152 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        8), /* "ILS" */
  /*   153 */ CHPEG_INST(CHPEG_OP_GOTO        ,      155),
  /*   154 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      152),
  /*   155 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   156 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        8), /* Options */
  /*   157 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        8), /* Options */
  /*   158 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        9), /* "a-zA-Z_" */
  /*   159 */ CHPEG_INST(CHPEG_OP_GOTO        ,      166),
  /*   160 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   161 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       10), /* "a-zA-Z_0-9" */
  /*   162 */ CHPEG_INST(CHPEG_OP_GOTO        ,      164),
  /*   163 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      161),
  /*   164 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   165 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        9), /* Identifier */
  /*   166 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        9), /* Identifier */
  /*   167 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   168 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "'" */
  /*   169 */ CHPEG_INST(CHPEG_OP_GOTO        ,      183),
  /*   170 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   171 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   172 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "'" */
  /*   173 */ CHPEG_INST(CHPEG_OP_GOTO        ,      175),
  /*   174 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      179),
  /*   175 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      179),
  /*   176 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   177 */ CHPEG_INST(CHPEG_OP_GOTO        ,      179),
  /*   178 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      171),
  /*   179 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   180 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "'" */
  /*   181 */ CHPEG_INST(CHPEG_OP_GOTO        ,      183),
  /*   182 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      201),
  /*   183 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   184 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "\"" */
  /*   185 */ CHPEG_INST(CHPEG_OP_GOTO        ,      199),
  /*   186 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   187 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   188 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "\"" */
  /*   189 */ CHPEG_INST(CHPEG_OP_GOTO        ,      191),
  /*   190 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      195),
  /*   191 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      195),
  /*   192 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   193 */ CHPEG_INST(CHPEG_OP_GOTO        ,      195),
  /*   194 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      187),
  /*   195 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   196 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "\"" */
  /*   197 */ CHPEG_INST(CHPEG_OP_GOTO        ,      199),
  /*   198 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      201),
  /*   199 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   200 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      202),
  /*   201 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       10), /* Literal */
  /*   202 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       10), /* Literal */
  /*   203 */ CHPEG_INST(CHPEG_OP_LIT         ,       13), /* "[" */
  /*   204 */ CHPEG_INST(CHPEG_OP_GOTO        ,      218),
  /*   205 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   206 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   207 */ CHPEG_INST(CHPEG_OP_LIT         ,       14), /* "]" */
  /*   208 */ CHPEG_INST(CHPEG_OP_GOTO        ,      210),
  /*   209 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      214),
  /*   210 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      214),
  /*   211 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* CharRange */
  /*   212 */ CHPEG_INST(CHPEG_OP_GOTO        ,      214),
  /*   213 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      206),
  /*   214 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      218),
  /*   215 */ CHPEG_INST(CHPEG_OP_LIT         ,       14), /* "]" */
  /*   216 */ CHPEG_INST(CHPEG_OP_GOTO        ,      218),
  /*   217 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       11), /* CharClass */
  /*   218 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       11), /* CharClass */
  /*   219 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   220 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   221 */ CHPEG_INST(CHPEG_OP_GOTO        ,      232),
  /*   222 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "-" */
  /*   223 */ CHPEG_INST(CHPEG_OP_GOTO        ,      232),
  /*   224 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   225 */ CHPEG_INST(CHPEG_OP_LIT         ,       14), /* "]" */
  /*   226 */ CHPEG_INST(CHPEG_OP_GOTO        ,      228),
  /*   227 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      232),
  /*   228 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      232),
  /*   229 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   230 */ CHPEG_INST(CHPEG_OP_GOTO        ,      232),
  /*   231 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      238),
  /*   232 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   233 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   234 */ CHPEG_INST(CHPEG_OP_GOTO        ,      236),
  /*   235 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      238),
  /*   236 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   237 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      239),
  /*   238 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       12), /* CharRange */
  /*   239 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       12), /* CharRange */
  /*   240 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   241 */ CHPEG_INST(CHPEG_OP_IDENT       ,       14), /* EscChar */
  /*   242 */ CHPEG_INST(CHPEG_OP_GOTO        ,      244),
  /*   243 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      254),
  /*   244 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   245 */ CHPEG_INST(CHPEG_OP_IDENT       ,       15), /* OctChar */
  /*   246 */ CHPEG_INST(CHPEG_OP_GOTO        ,      248),
  /*   247 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      254),
  /*   248 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   249 */ CHPEG_INST(CHPEG_OP_IDENT       ,       16), /* PlainChar */
  /*   250 */ CHPEG_INST(CHPEG_OP_GOTO        ,      252),
  /*   251 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      254),
  /*   252 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   253 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      255),
  /*   254 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       13), /* Char */
  /*   255 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       13), /* Char */
  /*   256 */ CHPEG_INST(CHPEG_OP_LIT         ,       16), /* "\\" */
  /*   257 */ CHPEG_INST(CHPEG_OP_GOTO        ,      261),
  /*   258 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       17), /* "nrt'\"[]\\" */
  /*   259 */ CHPEG_INST(CHPEG_OP_GOTO        ,      261),
  /*   260 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       14), /* EscChar */
  /*   261 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       14), /* EscChar */
  /*   262 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   263 */ CHPEG_INST(CHPEG_OP_LIT         ,       16), /* "\\" */
  /*   264 */ CHPEG_INST(CHPEG_OP_GOTO        ,      272),
  /*   265 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "0-3" */
  /*   266 */ CHPEG_INST(CHPEG_OP_GOTO        ,      272),
  /*   267 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "0-7" */
  /*   268 */ CHPEG_INST(CHPEG_OP_GOTO        ,      272),
  /*   269 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "0-7" */
  /*   270 */ CHPEG_INST(CHPEG_OP_GOTO        ,      272),
  /*   271 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      285),
  /*   272 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   273 */ CHPEG_INST(CHPEG_OP_LIT         ,       16), /* "\\" */
  /*   274 */ CHPEG_INST(CHPEG_OP_GOTO        ,      283),
  /*   275 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "0-7" */
  /*   276 */ CHPEG_INST(CHPEG_OP_GOTO        ,      283),
  /*   277 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*   278 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "0-7" */
  /*   279 */ CHPEG_INST(CHPEG_OP_GOTO        ,      281),
  /*   280 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*   281 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*   282 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      285),
  /*   283 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   284 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      286),
  /*   285 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       15), /* OctChar */
  /*   286 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       15), /* OctChar */
  /*   287 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   288 */ CHPEG_INST(CHPEG_OP_LIT         ,       16), /* "\\" */
  /*   289 */ CHPEG_INST(CHPEG_OP_GOTO        ,      291),
  /*   290 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      294),
  /*   291 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      294),
  /*   292 */ CHPEG_INST(CHPEG_OP_DOT         ,      294),
  /*   293 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       16), /* PlainChar */
  /*   294 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       16), /* PlainChar */
  /*   295 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "&!" */
  /*   296 */ CHPEG_INST(CHPEG_OP_GOTO        ,      298),
  /*   297 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       17), /* PredOp */
  /*   298 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       17), /* PredOp */
  /*   299 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       21), /* "*+?" */
  /*   300 */ CHPEG_INST(CHPEG_OP_GOTO        ,      302),
  /*   301 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       18), /* RepOp */
  /*   302 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       18), /* RepOp */
  /*   303 */ CHPEG_INST(CHPEG_OP_LIT         ,       22), /* "." */
  /*   304 */ CHPEG_INST(CHPEG_OP_GOTO        ,      306),
  /*   305 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       19), /* Dot */
  /*   306 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       19), /* Dot */
  /*   307 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   308 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   309 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   310 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       23), /* " \t\r\n" */
  /*   311 */ CHPEG_INST(CHPEG_OP_GOTO        ,      313),
  /*   312 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      310),
  /*   313 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      315),
  /*   314 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      335),
  /*   315 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   316 */ CHPEG_INST(CHPEG_OP_LIT         ,       24), /* "#" */
  /*   317 */ CHPEG_INST(CHPEG_OP_GOTO        ,      333),
  /*   318 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   319 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   320 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       25), /* "\r\n" */
  /*   321 */ CHPEG_INST(CHPEG_OP_GOTO        ,      323),
  /*   322 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      326),
  /*   323 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      326),
  /*   324 */ CHPEG_INST(CHPEG_OP_DOT         ,      326),
  /*   325 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      319),
  /*   326 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   327 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*   328 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       25), /* "\r\n" */
  /*   329 */ CHPEG_INST(CHPEG_OP_GOTO        ,      331),
  /*   330 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*   331 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*   332 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      335),
  /*   333 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   334 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      336),
  /*   335 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      308),
  /*   336 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   337 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       20), /* S */
  /*   338 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       20), /* S */
  },
  .num_strings = 26,
  .strings = (unsigned char**)(char*[26]) {"{", "}", "<-", "/", "(", ")", "<", ">", "ILS", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"},
  .str_len = (int[26]) {1, 1, 2, 1, 1, 1, 1, 1, 3, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2}
};
