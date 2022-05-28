#ifndef CHPEG_AMALGAMATION
#include "chpeg/chpeg_bytecode.h"
#endif

CHPEG_DEF const ChpegByteCode chpeg_bytecode = {
  .num_defs = 21,
  .def_names = (char*[21]) {"Grammar", "Definition", "Choice", "Sequence", "Predicate", "Repeat", "Primary", "Trim", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"},
  .def_flags = (int[21]) {CHPEG_FLAG_STOP, 0, 0, 0, 0, 0, 0, CHPEG_FLAG_STOP, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_STOP, CHPEG_FLAG_STOP, 0, 0, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_LEAF, CHPEG_FLAG_IGNORE},
  .def_addrs = (int[21]) {2, 15, 42, 55, 62, 73, 84, 133, 145, 152, 161, 197, 213, 234, 250, 256, 281, 289, 293, 297, 301}, // presubtracted by 1
  .num_instructions = 331,
  .instructions = (int[331]) {
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
  /*    16 */ CHPEG_INST(CHPEG_OP_IDENT       ,        9), /* Identifier */
  /*    17 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    18 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    19 */ CHPEG_INST(CHPEG_OP_GOTO        ,       41),
  /*    20 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    21 */ CHPEG_INST(CHPEG_OP_LIT         ,        0), /* "{" */
  /*    22 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    23 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    24 */ CHPEG_INST(CHPEG_OP_GOTO        ,       33),
  /*    25 */ CHPEG_INST(CHPEG_OP_IDENT       ,        8), /* Options */
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
  /*    57 */ CHPEG_INST(CHPEG_OP_IDENT       ,        4), /* Predicate */
  /*    58 */ CHPEG_INST(CHPEG_OP_GOTO        ,       59),
  /*    59 */ CHPEG_INST(CHPEG_OP_RPMAT       ,       56),
  /*    60 */ CHPEG_INST(CHPEG_OP_RPDONE      ,       61),
  /*    61 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        3), /* Sequence */
  /*    62 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        3), /* Sequence */
  /*    63 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    64 */ CHPEG_INST(CHPEG_OP_IDENT       ,       17), /* PredOp */
  /*    65 */ CHPEG_INST(CHPEG_OP_GOTO        ,       68),
  /*    66 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    67 */ CHPEG_INST(CHPEG_OP_GOTO        ,       68),
  /*    68 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    69 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    70 */ CHPEG_INST(CHPEG_OP_IDENT       ,        5), /* Repeat */
  /*    71 */ CHPEG_INST(CHPEG_OP_GOTO        ,       72),
  /*    72 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        4), /* Predicate */
  /*    73 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        4), /* Predicate */
  /*    74 */ CHPEG_INST(CHPEG_OP_IDENT       ,        6), /* Primary */
  /*    75 */ CHPEG_INST(CHPEG_OP_GOTO        ,       83),
  /*    76 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*    77 */ CHPEG_INST(CHPEG_OP_IDENT       ,       18), /* RepOp */
  /*    78 */ CHPEG_INST(CHPEG_OP_GOTO        ,       81),
  /*    79 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    80 */ CHPEG_INST(CHPEG_OP_GOTO        ,       81),
  /*    81 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*    82 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*    83 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        5), /* Repeat */
  /*    84 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        5), /* Repeat */
  /*    85 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*    86 */ CHPEG_INST(CHPEG_OP_IDENT       ,        9), /* Identifier */
  /*    87 */ CHPEG_INST(CHPEG_OP_GOTO        ,       95),
  /*    88 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*    89 */ CHPEG_INST(CHPEG_OP_GOTO        ,       95),
  /*    90 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*    91 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        4), /* "{<" */
  /*    92 */ CHPEG_INST(CHPEG_OP_GOTO        ,       93),
  /*    93 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,       95),
  /*    94 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,       95),
  /*    95 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*    96 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*    97 */ CHPEG_INST(CHPEG_OP_LIT         ,        5), /* "(" */
  /*    98 */ CHPEG_INST(CHPEG_OP_GOTO        ,      107),
  /*    99 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   100 */ CHPEG_INST(CHPEG_OP_GOTO        ,      107),
  /*   101 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*   102 */ CHPEG_INST(CHPEG_OP_GOTO        ,      107),
  /*   103 */ CHPEG_INST(CHPEG_OP_LIT         ,        6), /* ")" */
  /*   104 */ CHPEG_INST(CHPEG_OP_GOTO        ,      107),
  /*   105 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   106 */ CHPEG_INST(CHPEG_OP_GOTO        ,      107),
  /*   107 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*   108 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   109 */ CHPEG_INST(CHPEG_OP_IDENT       ,        7), /* Trim */
  /*   110 */ CHPEG_INST(CHPEG_OP_GOTO        ,      111),
  /*   111 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*   112 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   113 */ CHPEG_INST(CHPEG_OP_IDENT       ,       10), /* Literal */
  /*   114 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   115 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   116 */ CHPEG_INST(CHPEG_OP_GOTO        ,      117),
  /*   117 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*   118 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   119 */ CHPEG_INST(CHPEG_OP_IDENT       ,       11), /* CharClass */
  /*   120 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   121 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   122 */ CHPEG_INST(CHPEG_OP_GOTO        ,      123),
  /*   123 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*   124 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   125 */ CHPEG_INST(CHPEG_OP_IDENT       ,       19), /* Dot */
  /*   126 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   127 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   128 */ CHPEG_INST(CHPEG_OP_GOTO        ,      129),
  /*   129 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      131),
  /*   130 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   131 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      132),
  /*   132 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        6), /* Primary */
  /*   133 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        6), /* Primary */
  /*   134 */ CHPEG_INST(CHPEG_OP_LIT         ,        7), /* "<" */
  /*   135 */ CHPEG_INST(CHPEG_OP_GOTO        ,      144),
  /*   136 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   137 */ CHPEG_INST(CHPEG_OP_GOTO        ,      144),
  /*   138 */ CHPEG_INST(CHPEG_OP_IDENT       ,        2), /* Choice */
  /*   139 */ CHPEG_INST(CHPEG_OP_GOTO        ,      144),
  /*   140 */ CHPEG_INST(CHPEG_OP_LIT         ,        8), /* ">" */
  /*   141 */ CHPEG_INST(CHPEG_OP_GOTO        ,      144),
  /*   142 */ CHPEG_INST(CHPEG_OP_IDENT       ,       20), /* S */
  /*   143 */ CHPEG_INST(CHPEG_OP_GOTO        ,      144),
  /*   144 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        7), /* Trim */
  /*   145 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        7), /* Trim */
  /*   146 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   147 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,        9), /* "ILS" */
  /*   148 */ CHPEG_INST(CHPEG_OP_GOTO        ,      149),
  /*   149 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      146),
  /*   150 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   151 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        8), /* Options */
  /*   152 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        8), /* Options */
  /*   153 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       10), /* "a-zA-Z_" */
  /*   154 */ CHPEG_INST(CHPEG_OP_GOTO        ,      160),
  /*   155 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   156 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       11), /* "a-zA-Z_0-9" */
  /*   157 */ CHPEG_INST(CHPEG_OP_GOTO        ,      158),
  /*   158 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      155),
  /*   159 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   160 */ CHPEG_INST(CHPEG_OP_ISUCC       ,        9), /* Identifier */
  /*   161 */ CHPEG_INST(CHPEG_OP_IFAIL       ,        9), /* Identifier */
  /*   162 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   163 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "'" */
  /*   164 */ CHPEG_INST(CHPEG_OP_GOTO        ,      177),
  /*   165 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   166 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   167 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "'" */
  /*   168 */ CHPEG_INST(CHPEG_OP_GOTO        ,      169),
  /*   169 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      173),
  /*   170 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      173),
  /*   171 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   172 */ CHPEG_INST(CHPEG_OP_GOTO        ,      173),
  /*   173 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      165),
  /*   174 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   175 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       12), /* "'" */
  /*   176 */ CHPEG_INST(CHPEG_OP_GOTO        ,      177),
  /*   177 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      195),
  /*   178 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   179 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       13), /* "\"" */
  /*   180 */ CHPEG_INST(CHPEG_OP_GOTO        ,      193),
  /*   181 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   182 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   183 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       13), /* "\"" */
  /*   184 */ CHPEG_INST(CHPEG_OP_GOTO        ,      185),
  /*   185 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      189),
  /*   186 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      189),
  /*   187 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   188 */ CHPEG_INST(CHPEG_OP_GOTO        ,      189),
  /*   189 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      181),
  /*   190 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   191 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       13), /* "\"" */
  /*   192 */ CHPEG_INST(CHPEG_OP_GOTO        ,      193),
  /*   193 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      195),
  /*   194 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   195 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      196),
  /*   196 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       10), /* Literal */
  /*   197 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       10), /* Literal */
  /*   198 */ CHPEG_INST(CHPEG_OP_LIT         ,       14), /* "[" */
  /*   199 */ CHPEG_INST(CHPEG_OP_GOTO        ,      212),
  /*   200 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   201 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   202 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "]" */
  /*   203 */ CHPEG_INST(CHPEG_OP_GOTO        ,      204),
  /*   204 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      208),
  /*   205 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      208),
  /*   206 */ CHPEG_INST(CHPEG_OP_IDENT       ,       12), /* CharRange */
  /*   207 */ CHPEG_INST(CHPEG_OP_GOTO        ,      208),
  /*   208 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      200),
  /*   209 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      212),
  /*   210 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "]" */
  /*   211 */ CHPEG_INST(CHPEG_OP_GOTO        ,      212),
  /*   212 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       11), /* CharClass */
  /*   213 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       11), /* CharClass */
  /*   214 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   215 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   216 */ CHPEG_INST(CHPEG_OP_GOTO        ,      226),
  /*   217 */ CHPEG_INST(CHPEG_OP_LIT         ,       16), /* "-" */
  /*   218 */ CHPEG_INST(CHPEG_OP_GOTO        ,      226),
  /*   219 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   220 */ CHPEG_INST(CHPEG_OP_LIT         ,       15), /* "]" */
  /*   221 */ CHPEG_INST(CHPEG_OP_GOTO        ,      222),
  /*   222 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      226),
  /*   223 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      226),
  /*   224 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   225 */ CHPEG_INST(CHPEG_OP_GOTO        ,      226),
  /*   226 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      232),
  /*   227 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   228 */ CHPEG_INST(CHPEG_OP_IDENT       ,       13), /* Char */
  /*   229 */ CHPEG_INST(CHPEG_OP_GOTO        ,      230),
  /*   230 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      232),
  /*   231 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   232 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      233),
  /*   233 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       12), /* CharRange */
  /*   234 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       12), /* CharRange */
  /*   235 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   236 */ CHPEG_INST(CHPEG_OP_IDENT       ,       14), /* EscChar */
  /*   237 */ CHPEG_INST(CHPEG_OP_GOTO        ,      238),
  /*   238 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      248),
  /*   239 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   240 */ CHPEG_INST(CHPEG_OP_IDENT       ,       15), /* OctChar */
  /*   241 */ CHPEG_INST(CHPEG_OP_GOTO        ,      242),
  /*   242 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      248),
  /*   243 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   244 */ CHPEG_INST(CHPEG_OP_IDENT       ,       16), /* PlainChar */
  /*   245 */ CHPEG_INST(CHPEG_OP_GOTO        ,      246),
  /*   246 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      248),
  /*   247 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   248 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      249),
  /*   249 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       13), /* Char */
  /*   250 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       13), /* Char */
  /*   251 */ CHPEG_INST(CHPEG_OP_LIT         ,       17), /* "\\" */
  /*   252 */ CHPEG_INST(CHPEG_OP_GOTO        ,      255),
  /*   253 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       18), /* "nrt'\"[]\\" */
  /*   254 */ CHPEG_INST(CHPEG_OP_GOTO        ,      255),
  /*   255 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       14), /* EscChar */
  /*   256 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       14), /* EscChar */
  /*   257 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   258 */ CHPEG_INST(CHPEG_OP_LIT         ,       17), /* "\\" */
  /*   259 */ CHPEG_INST(CHPEG_OP_GOTO        ,      266),
  /*   260 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       19), /* "0-3" */
  /*   261 */ CHPEG_INST(CHPEG_OP_GOTO        ,      266),
  /*   262 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "0-7" */
  /*   263 */ CHPEG_INST(CHPEG_OP_GOTO        ,      266),
  /*   264 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "0-7" */
  /*   265 */ CHPEG_INST(CHPEG_OP_GOTO        ,      266),
  /*   266 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      279),
  /*   267 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   268 */ CHPEG_INST(CHPEG_OP_LIT         ,       17), /* "\\" */
  /*   269 */ CHPEG_INST(CHPEG_OP_GOTO        ,      277),
  /*   270 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "0-7" */
  /*   271 */ CHPEG_INST(CHPEG_OP_GOTO        ,      277),
  /*   272 */ CHPEG_INST(CHPEG_OP_RQBEG       ,        0),
  /*   273 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       20), /* "0-7" */
  /*   274 */ CHPEG_INST(CHPEG_OP_GOTO        ,      275),
  /*   275 */ CHPEG_INST(CHPEG_OP_RQMAT       ,        0),
  /*   276 */ CHPEG_INST(CHPEG_OP_RQDONE      ,        0),
  /*   277 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      279),
  /*   278 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   279 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      280),
  /*   280 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       15), /* OctChar */
  /*   281 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       15), /* OctChar */
  /*   282 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   283 */ CHPEG_INST(CHPEG_OP_LIT         ,       17), /* "\\" */
  /*   284 */ CHPEG_INST(CHPEG_OP_GOTO        ,      285),
  /*   285 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      288),
  /*   286 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      288),
  /*   287 */ CHPEG_INST(CHPEG_OP_DOT         ,      288),
  /*   288 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       16), /* PlainChar */
  /*   289 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       16), /* PlainChar */
  /*   290 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       21), /* "&!" */
  /*   291 */ CHPEG_INST(CHPEG_OP_GOTO        ,      292),
  /*   292 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       17), /* PredOp */
  /*   293 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       17), /* PredOp */
  /*   294 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       22), /* "*+?" */
  /*   295 */ CHPEG_INST(CHPEG_OP_GOTO        ,      296),
  /*   296 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       18), /* RepOp */
  /*   297 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       18), /* RepOp */
  /*   298 */ CHPEG_INST(CHPEG_OP_LIT         ,       23), /* "." */
  /*   299 */ CHPEG_INST(CHPEG_OP_GOTO        ,      300),
  /*   300 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       19), /* Dot */
  /*   301 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       19), /* Dot */
  /*   302 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   303 */ CHPEG_INST(CHPEG_OP_CHOICE      ,        0),
  /*   304 */ CHPEG_INST(CHPEG_OP_RPBEG       ,        0),
  /*   305 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       24), /* " \t\r\n" */
  /*   306 */ CHPEG_INST(CHPEG_OP_GOTO        ,      307),
  /*   307 */ CHPEG_INST(CHPEG_OP_RPMAT       ,      304),
  /*   308 */ CHPEG_INST(CHPEG_OP_RPDONE      ,      309),
  /*   309 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      326),
  /*   310 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   311 */ CHPEG_INST(CHPEG_OP_LIT         ,       25), /* "#" */
  /*   312 */ CHPEG_INST(CHPEG_OP_GOTO        ,      324),
  /*   313 */ CHPEG_INST(CHPEG_OP_RSBEG       ,        0),
  /*   314 */ CHPEG_INST(CHPEG_OP_PREDN       ,        0),
  /*   315 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       26), /* "\r\n" */
  /*   316 */ CHPEG_INST(CHPEG_OP_GOTO        ,      317),
  /*   317 */ CHPEG_INST(CHPEG_OP_PMATCHF     ,      320),
  /*   318 */ CHPEG_INST(CHPEG_OP_PNOMATS     ,      320),
  /*   319 */ CHPEG_INST(CHPEG_OP_DOT         ,      320),
  /*   320 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      313),
  /*   321 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   322 */ CHPEG_INST(CHPEG_OP_CHRCLS      ,       26), /* "\r\n" */
  /*   323 */ CHPEG_INST(CHPEG_OP_GOTO        ,      324),
  /*   324 */ CHPEG_INST(CHPEG_OP_CISUCC      ,      326),
  /*   325 */ CHPEG_INST(CHPEG_OP_CIFAIL      ,        0),
  /*   326 */ CHPEG_INST(CHPEG_OP_CFAIL       ,      327),
  /*   327 */ CHPEG_INST(CHPEG_OP_RSMAT       ,      302),
  /*   328 */ CHPEG_INST(CHPEG_OP_RSDONE      ,        0),
  /*   329 */ CHPEG_INST(CHPEG_OP_ISUCC       ,       20), /* S */
  /*   330 */ CHPEG_INST(CHPEG_OP_IFAIL       ,       20), /* S */
  },
  .num_strings = 27,
  .strings = (unsigned char**)(char*[27]) {"{", "}", "<-", "/", "{<", "(", ")", "<", ">", "ILS", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"},
  .str_len = (int[27]) {1, 1, 2, 1, 2, 1, 1, 1, 1, 3, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2}
};
