#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "peg_cbyte_parser.h"


int peg_num_defs = 20;
char *peg_def_names[] = {"Grammar", "Definition", "Choice", "Sequence", "Predicate", "Repeat", "Primary", "Options", "Identifier", "Literal", "CharClass", "CharRange", "Char", "EscChar", "OctChar", "PlainChar", "PredOp", "RepOp", "Dot", "S"};
int peg_def_flags[] = {STOP, 0, 0, 0, 0, 0, 0, 0, LEAF, STOP, STOP, 0, 0, LEAF, LEAF, LEAF, LEAF, LEAF, LEAF, IGNORE};
int peg_def_addrs[] = {2, 15, 42, 55, 62, 73, 84, 129, 136, 145, 181, 197, 218, 234, 240, 265, 273, 277, 281, 285}; // presubtracted by 1

int peg_num_strings = 25;
char *peg_strings[] = {"{", "}", "<-", "/", "{<", "(", ")", "ILS", "a-zA-Z_", "a-zA-Z_0-9", "'", "\"", "[", "]", "-", "\\", "nrt'\"[]\\", "0-3", "0-7", "&!", "*+?", ".", " \t\r\n", "#", "\r\n"};
int peg_str_len[] = {1, 1, 2, 1, 2, 1, 1, 3, 7, 10, 1, 1, 1, 1, 1, 1, 8, 3, 3, 2, 3, 1, 4, 1, 2};

int peg_num_instructions = 315;
int peg_instructions[] = {
    /*     0 <:Grammar 0 (Grammar) []>                */ INST(IDENT, 0),
    /*     1 <:Grammar 0 (Grammar) []>                */ INST(FAIL, 0),
    /*     2 <:Grammar 0 (Grammar) []>                */ INST(SUCC, 0),
    /*     3 <---:Identifier 4 (Grammar[0]) "S">      */ INST(IDENT, 19),
    /*     4 <---:Identifier 4 (Grammar[0]) "S">      */ INST(GOTO, 14),
    /*     5 <---:Repeat 5 (Grammar[1]) []>           */ INST(RPBEG, 0),
    /*     6 <----:Identifier 6 (Grammar[1][0]) "Definition"> */ INST(IDENT, 1),
    /*     7 <----:Identifier 6 (Grammar[1][0]) "Definition"> */ INST(GOTO, 8),
    /*     8 <---:Repeat 5 (Grammar[1]) []>           */ INST(RPMAT, 5),
    /*     9 <---:Repeat 5 (Grammar[1]) []>           */ INST(RPDONE, 14),
    /*    10 <---:Predicate 7 (Grammar[2]) []>        */ INST(PRED, 0),
    /*    11 <----:Dot 8 (Grammar[2][0]) ".">         */ INST(DOT, 12),
    /*    12 <---:Predicate 7 (Grammar[2]) []>        */ INST(PMATCHF, 14),
    /*    13 <---:Predicate 7 (Grammar[2]) []>        */ INST(PNOMATS, 14),
    /*    14 <-:Definition 1 (def:Grammar) []> {S}    */ INST(ISUCC, 0),
    /*    15 <-:Definition 1 (def:Grammar) []> {S}    */ INST(IFAIL, 0),
    /*    16 <---:Identifier 12 (Definition[0]) "Identifier"> */ INST(IDENT, 8),
    /*    17 <---:Identifier 12 (Definition[0]) "Identifier"> */ INST(GOTO, 41),
    /*    18 <---:Identifier 13 (Definition[1]) "S">  */ INST(IDENT, 19),
    /*    19 <---:Identifier 13 (Definition[1]) "S">  */ INST(GOTO, 41),
    /*    20 <---:Repeat 14 (Definition[2]) []>       */ INST(RQBEG, 0),
    /*    21 <-----:Literal  (Definition[2][0][0]) "{"> */ INST(LIT, 0),
    /*    22 <-----:Literal  (Definition[2][0][0]) "{"> */ INST(GOTO, 33),
    /*    23 <-----:Identifier 16 (Definition[2][0][1]) "S"> */ INST(IDENT, 19),
    /*    24 <-----:Identifier 16 (Definition[2][0][1]) "S"> */ INST(GOTO, 33),
    /*    25 <-----:Identifier 17 (Definition[2][0][2]) "Options"> */ INST(IDENT, 7),
    /*    26 <-----:Identifier 17 (Definition[2][0][2]) "Options"> */ INST(GOTO, 33),
    /*    27 <-----:Identifier 18 (Definition[2][0][3]) "S"> */ INST(IDENT, 19),
    /*    28 <-----:Identifier 18 (Definition[2][0][3]) "S"> */ INST(GOTO, 33),
    /*    29 <-----:Literal  (Definition[2][0][4]) "}"> */ INST(LIT, 1),
    /*    30 <-----:Literal  (Definition[2][0][4]) "}"> */ INST(GOTO, 33),
    /*    31 <-----:Identifier 19 (Definition[2][0][5]) "S"> */ INST(IDENT, 19),
    /*    32 <-----:Identifier 19 (Definition[2][0][5]) "S"> */ INST(GOTO, 33),
    /*    33 <---:Repeat 14 (Definition[2]) []>       */ INST(RQMAT, 0),
    /*    34 <---:Repeat 14 (Definition[2]) []>       */ INST(RQDONE, 0),
    /*    35 <---:Literal  (Definition[3]) "<-">      */ INST(LIT, 2),
    /*    36 <---:Literal  (Definition[3]) "<-">      */ INST(GOTO, 41),
    /*    37 <---:Identifier 20 (Definition[4]) "S">  */ INST(IDENT, 19),
    /*    38 <---:Identifier 20 (Definition[4]) "S">  */ INST(GOTO, 41),
    /*    39 <---:Identifier 21 (Definition[5]) "Choice"> */ INST(IDENT, 2),
    /*    40 <---:Identifier 21 (Definition[5]) "Choice"> */ INST(GOTO, 41),
    /*    41 <-:Definition 9 (def:Definition) []> {}  */ INST(ISUCC, 1),
    /*    42 <-:Definition 9 (def:Definition) []> {}  */ INST(IFAIL, 0),
    /*    43 <---:Identifier 25 (Choice[0]) "Sequence"> */ INST(IDENT, 3),
    /*    44 <---:Identifier 25 (Choice[0]) "Sequence"> */ INST(GOTO, 54),
    /*    45 <---:Repeat 26 (Choice[1]) []>           */ INST(RSBEG, 0),
    /*    46 <-----:Literal  (Choice[1][0][0]) "/">   */ INST(LIT, 3),
    /*    47 <-----:Literal  (Choice[1][0][0]) "/">   */ INST(GOTO, 52),
    /*    48 <-----:Identifier 28 (Choice[1][0][1]) "S"> */ INST(IDENT, 19),
    /*    49 <-----:Identifier 28 (Choice[1][0][1]) "S"> */ INST(GOTO, 52),
    /*    50 <-----:Identifier 29 (Choice[1][0][2]) "Sequence"> */ INST(IDENT, 3),
    /*    51 <-----:Identifier 29 (Choice[1][0][2]) "Sequence"> */ INST(GOTO, 52),
    /*    52 <---:Repeat 26 (Choice[1]) []>           */ INST(RSMAT, 45),
    /*    53 <---:Repeat 26 (Choice[1]) []>           */ INST(RSDONE, 0),
    /*    54 <-:Definition 22 (def:Choice) []> {}     */ INST(ISUCC, 2),
    /*    55 <-:Definition 22 (def:Choice) []> {}     */ INST(IFAIL, 0),
    /*    56 <--:Repeat 32 (Sequence) []>             */ INST(RPBEG, 0),
    /*    57 <---:Identifier 33 (Sequence[0]) "Predicate"> */ INST(IDENT, 4),
    /*    58 <---:Identifier 33 (Sequence[0]) "Predicate"> */ INST(GOTO, 59),
    /*    59 <--:Repeat 32 (Sequence) []>             */ INST(RPMAT, 56),
    /*    60 <--:Repeat 32 (Sequence) []>             */ INST(RPDONE, 61),
    /*    61 <-:Definition 30 (def:Sequence) []> {}   */ INST(ISUCC, 3),
    /*    62 <-:Definition 30 (def:Sequence) []> {}   */ INST(IFAIL, 0),
    /*    63 <---:Repeat 37 (Predicate[0]) []>        */ INST(RQBEG, 0),
    /*    64 <-----:Identifier 39 (Predicate[0][0][0]) "PredOp"> */ INST(IDENT, 16),
    /*    65 <-----:Identifier 39 (Predicate[0][0][0]) "PredOp"> */ INST(GOTO, 68),
    /*    66 <-----:Identifier 40 (Predicate[0][0][1]) "S"> */ INST(IDENT, 19),
    /*    67 <-----:Identifier 40 (Predicate[0][0][1]) "S"> */ INST(GOTO, 68),
    /*    68 <---:Repeat 37 (Predicate[0]) []>        */ INST(RQMAT, 0),
    /*    69 <---:Repeat 37 (Predicate[0]) []>        */ INST(RQDONE, 0),
    /*    70 <---:Identifier 41 (Predicate[1]) "Repeat"> */ INST(IDENT, 5),
    /*    71 <---:Identifier 41 (Predicate[1]) "Repeat"> */ INST(GOTO, 72),
    /*    72 <-:Definition 34 (def:Predicate) []> {}  */ INST(ISUCC, 4),
    /*    73 <-:Definition 34 (def:Predicate) []> {}  */ INST(IFAIL, 0),
    /*    74 <---:Identifier 45 (Repeat[0]) "Primary"> */ INST(IDENT, 6),
    /*    75 <---:Identifier 45 (Repeat[0]) "Primary"> */ INST(GOTO, 83),
    /*    76 <---:Repeat 46 (Repeat[1]) []>           */ INST(RQBEG, 0),
    /*    77 <-----:Identifier 48 (Repeat[1][0][0]) "RepOp"> */ INST(IDENT, 17),
    /*    78 <-----:Identifier 48 (Repeat[1][0][0]) "RepOp"> */ INST(GOTO, 81),
    /*    79 <-----:Identifier 49 (Repeat[1][0][1]) "S"> */ INST(IDENT, 19),
    /*    80 <-----:Identifier 49 (Repeat[1][0][1]) "S"> */ INST(GOTO, 81),
    /*    81 <---:Repeat 46 (Repeat[1]) []>           */ INST(RQMAT, 0),
    /*    82 <---:Repeat 46 (Repeat[1]) []>           */ INST(RQDONE, 0),
    /*    83 <-:Definition 42 (def:Repeat) []> {}     */ INST(ISUCC, 5),
    /*    84 <-:Definition 42 (def:Repeat) []> {}     */ INST(IFAIL, 0),
    /*    85 <--:Choice 52 (Primary) []>              */ INST(CHOICE, 0),
    /*    86 <----:Identifier 54 (Primary[0][0]) "Identifier"> */ INST(IDENT, 8),
    /*    87 <----:Identifier 54 (Primary[0][0]) "Identifier"> */ INST(GOTO, 95),
    /*    88 <----:Identifier 55 (Primary[0][1]) "S"> */ INST(IDENT, 19),
    /*    89 <----:Identifier 55 (Primary[0][1]) "S"> */ INST(GOTO, 95),
    /*    90 <----:Predicate 56 (Primary[0][2]) []>   */ INST(PRED, 0),
    /*    91 <-----:CharClass  (Primary[0][2][0]) []> */ INST(CHRCLS, 4),
    /*    92 <-----:CharClass  (Primary[0][2][0]) []> */ INST(GOTO, 93),
    /*    93 <----:Predicate 56 (Primary[0][2]) []>   */ INST(PMATCHF, 95),
    /*    94 <----:Predicate 56 (Primary[0][2]) []>   */ INST(PNOMATS, 95),
    /*    95 <--:Choice 52 (Primary) []>              */ INST(CISUCC, 127),
    /*    96 <--:Choice 52 (Primary) []>              */ INST(CIFAIL, 0),
    /*    97 <----:Literal  (Primary[1][0]) "(">      */ INST(LIT, 5),
    /*    98 <----:Literal  (Primary[1][0]) "(">      */ INST(GOTO, 107),
    /*    99 <----:Identifier 58 (Primary[1][1]) "S"> */ INST(IDENT, 19),
    /*   100 <----:Identifier 58 (Primary[1][1]) "S"> */ INST(GOTO, 107),
    /*   101 <----:Identifier 59 (Primary[1][2]) "Choice"> */ INST(IDENT, 2),
    /*   102 <----:Identifier 59 (Primary[1][2]) "Choice"> */ INST(GOTO, 107),
    /*   103 <----:Literal  (Primary[1][3]) ")">      */ INST(LIT, 6),
    /*   104 <----:Literal  (Primary[1][3]) ")">      */ INST(GOTO, 107),
    /*   105 <----:Identifier 60 (Primary[1][4]) "S"> */ INST(IDENT, 19),
    /*   106 <----:Identifier 60 (Primary[1][4]) "S"> */ INST(GOTO, 107),
    /*   107 <--:Choice 52 (Primary) []>              */ INST(CISUCC, 127),
    /*   108 <--:Choice 52 (Primary) []>              */ INST(CIFAIL, 0),
    /*   109 <----:Identifier 62 (Primary[2][0]) "Literal"> */ INST(IDENT, 9),
    /*   110 <----:Identifier 62 (Primary[2][0]) "Literal"> */ INST(GOTO, 113),
    /*   111 <----:Identifier 63 (Primary[2][1]) "S"> */ INST(IDENT, 19),
    /*   112 <----:Identifier 63 (Primary[2][1]) "S"> */ INST(GOTO, 113),
    /*   113 <--:Choice 52 (Primary) []>              */ INST(CISUCC, 127),
    /*   114 <--:Choice 52 (Primary) []>              */ INST(CIFAIL, 0),
    /*   115 <----:Identifier 65 (Primary[3][0]) "CharClass"> */ INST(IDENT, 10),
    /*   116 <----:Identifier 65 (Primary[3][0]) "CharClass"> */ INST(GOTO, 119),
    /*   117 <----:Identifier 66 (Primary[3][1]) "S"> */ INST(IDENT, 19),
    /*   118 <----:Identifier 66 (Primary[3][1]) "S"> */ INST(GOTO, 119),
    /*   119 <--:Choice 52 (Primary) []>              */ INST(CISUCC, 127),
    /*   120 <--:Choice 52 (Primary) []>              */ INST(CIFAIL, 0),
    /*   121 <----:Identifier 68 (Primary[4][0]) "Dot"> */ INST(IDENT, 18),
    /*   122 <----:Identifier 68 (Primary[4][0]) "Dot"> */ INST(GOTO, 125),
    /*   123 <----:Identifier 69 (Primary[4][1]) "S"> */ INST(IDENT, 19),
    /*   124 <----:Identifier 69 (Primary[4][1]) "S"> */ INST(GOTO, 125),
    /*   125 <--:Choice 52 (Primary) []>              */ INST(CISUCC, 127),
    /*   126 <--:Choice 52 (Primary) []>              */ INST(CIFAIL, 0),
    /*   127 <--:Choice 52 (Primary) []>              */ INST(CFAIL, 128),
    /*   128 <-:Definition 50 (def:Primary) []> {}    */ INST(ISUCC, 6),
    /*   129 <-:Definition 50 (def:Primary) []> {}    */ INST(IFAIL, 0),
    /*   130 <--:Repeat 72 (Options) []>              */ INST(RSBEG, 0),
    /*   131 <---:CharClass  (Options[0]) []>         */ INST(CHRCLS, 7),
    /*   132 <---:CharClass  (Options[0]) []>         */ INST(GOTO, 133),
    /*   133 <--:Repeat 72 (Options) []>              */ INST(RSMAT, 130),
    /*   134 <--:Repeat 72 (Options) []>              */ INST(RSDONE, 0),
    /*   135 <-:Definition 70 (def:Options) []> {}    */ INST(ISUCC, 7),
    /*   136 <-:Definition 70 (def:Options) []> {}    */ INST(IFAIL, 0),
    /*   137 <---:CharClass  (Identifier[0]) []>      */ INST(CHRCLS, 8),
    /*   138 <---:CharClass  (Identifier[0]) []>      */ INST(GOTO, 144),
    /*   139 <---:Repeat 76 (Identifier[1]) []>       */ INST(RSBEG, 0),
    /*   140 <----:CharClass  (Identifier[1][0]) []>  */ INST(CHRCLS, 9),
    /*   141 <----:CharClass  (Identifier[1][0]) []>  */ INST(GOTO, 142),
    /*   142 <---:Repeat 76 (Identifier[1]) []>       */ INST(RSMAT, 139),
    /*   143 <---:Repeat 76 (Identifier[1]) []>       */ INST(RSDONE, 0),
    /*   144 <-:Definition 73 (def:Identifier) []> {L} */ INST(ISUCC, 8),
    /*   145 <-:Definition 73 (def:Identifier) []> {L} */ INST(IFAIL, 0),
    /*   146 <--:Choice 79 (Literal) []>              */ INST(CHOICE, 0),
    /*   147 <----:CharClass  (Literal[0][0]) []>     */ INST(CHRCLS, 10),
    /*   148 <----:CharClass  (Literal[0][0]) []>     */ INST(GOTO, 161),
    /*   149 <----:Repeat 81 (Literal[0][1]) []>      */ INST(RSBEG, 0),
    /*   150 <------:Predicate 83 (Literal[0][1][0][0]) []> */ INST(PRED, 0),
    /*   151 <-------:CharClass  (Literal[0][1][0][0][0]) []> */ INST(CHRCLS, 10),
    /*   152 <-------:CharClass  (Literal[0][1][0][0][0]) []> */ INST(GOTO, 153),
    /*   153 <------:Predicate 83 (Literal[0][1][0][0]) []> */ INST(PMATCHF, 157),
    /*   154 <------:Predicate 83 (Literal[0][1][0][0]) []> */ INST(PNOMATS, 157),
    /*   155 <------:Identifier 84 (Literal[0][1][0][1]) "Char"> */ INST(IDENT, 12),
    /*   156 <------:Identifier 84 (Literal[0][1][0][1]) "Char"> */ INST(GOTO, 157),
    /*   157 <----:Repeat 81 (Literal[0][1]) []>      */ INST(RSMAT, 149),
    /*   158 <----:Repeat 81 (Literal[0][1]) []>      */ INST(RSDONE, 0),
    /*   159 <----:CharClass  (Literal[0][2]) []>     */ INST(CHRCLS, 10),
    /*   160 <----:CharClass  (Literal[0][2]) []>     */ INST(GOTO, 161),
    /*   161 <--:Choice 79 (Literal) []>              */ INST(CISUCC, 179),
    /*   162 <--:Choice 79 (Literal) []>              */ INST(CIFAIL, 0),
    /*   163 <----:CharClass  (Literal[1][0]) []>     */ INST(CHRCLS, 11),
    /*   164 <----:CharClass  (Literal[1][0]) []>     */ INST(GOTO, 177),
    /*   165 <----:Repeat 86 (Literal[1][1]) []>      */ INST(RSBEG, 0),
    /*   166 <------:Predicate 88 (Literal[1][1][0][0]) []> */ INST(PRED, 0),
    /*   167 <-------:CharClass  (Literal[1][1][0][0][0]) []> */ INST(CHRCLS, 11),
    /*   168 <-------:CharClass  (Literal[1][1][0][0][0]) []> */ INST(GOTO, 169),
    /*   169 <------:Predicate 88 (Literal[1][1][0][0]) []> */ INST(PMATCHF, 173),
    /*   170 <------:Predicate 88 (Literal[1][1][0][0]) []> */ INST(PNOMATS, 173),
    /*   171 <------:Identifier 89 (Literal[1][1][0][1]) "Char"> */ INST(IDENT, 12),
    /*   172 <------:Identifier 89 (Literal[1][1][0][1]) "Char"> */ INST(GOTO, 173),
    /*   173 <----:Repeat 86 (Literal[1][1]) []>      */ INST(RSMAT, 165),
    /*   174 <----:Repeat 86 (Literal[1][1]) []>      */ INST(RSDONE, 0),
    /*   175 <----:CharClass  (Literal[1][2]) []>     */ INST(CHRCLS, 11),
    /*   176 <----:CharClass  (Literal[1][2]) []>     */ INST(GOTO, 177),
    /*   177 <--:Choice 79 (Literal) []>              */ INST(CISUCC, 179),
    /*   178 <--:Choice 79 (Literal) []>              */ INST(CIFAIL, 0),
    /*   179 <--:Choice 79 (Literal) []>              */ INST(CFAIL, 180),
    /*   180 <-:Definition 77 (def:Literal) []> {S}   */ INST(ISUCC, 9),
    /*   181 <-:Definition 77 (def:Literal) []> {S}   */ INST(IFAIL, 0),
    /*   182 <---:Literal  (CharClass[0]) "[">        */ INST(LIT, 12),
    /*   183 <---:Literal  (CharClass[0]) "[">        */ INST(GOTO, 196),
    /*   184 <---:Repeat 93 (CharClass[1]) []>        */ INST(RPBEG, 0),
    /*   185 <-----:Predicate 95 (CharClass[1][0][0]) []> */ INST(PRED, 0),
    /*   186 <------:Literal  (CharClass[1][0][0][0]) "]"> */ INST(LIT, 13),
    /*   187 <------:Literal  (CharClass[1][0][0][0]) "]"> */ INST(GOTO, 188),
    /*   188 <-----:Predicate 95 (CharClass[1][0][0]) []> */ INST(PMATCHF, 192),
    /*   189 <-----:Predicate 95 (CharClass[1][0][0]) []> */ INST(PNOMATS, 192),
    /*   190 <-----:Identifier 96 (CharClass[1][0][1]) "CharRange"> */ INST(IDENT, 11),
    /*   191 <-----:Identifier 96 (CharClass[1][0][1]) "CharRange"> */ INST(GOTO, 192),
    /*   192 <---:Repeat 93 (CharClass[1]) []>        */ INST(RPMAT, 184),
    /*   193 <---:Repeat 93 (CharClass[1]) []>        */ INST(RPDONE, 196),
    /*   194 <---:Literal  (CharClass[2]) "]">        */ INST(LIT, 13),
    /*   195 <---:Literal  (CharClass[2]) "]">        */ INST(GOTO, 196),
    /*   196 <-:Definition 90 (def:CharClass) []> {S} */ INST(ISUCC, 10),
    /*   197 <-:Definition 90 (def:CharClass) []> {S} */ INST(IFAIL, 0),
    /*   198 <--:Choice 99 (CharRange) []>            */ INST(CHOICE, 0),
    /*   199 <----:Identifier 101 (CharRange[0][0]) "Char"> */ INST(IDENT, 12),
    /*   200 <----:Identifier 101 (CharRange[0][0]) "Char"> */ INST(GOTO, 210),
    /*   201 <----:Literal  (CharRange[0][1]) "-">    */ INST(LIT, 14),
    /*   202 <----:Literal  (CharRange[0][1]) "-">    */ INST(GOTO, 210),
    /*   203 <----:Predicate 102 (CharRange[0][2]) []> */ INST(PRED, 0),
    /*   204 <-----:Literal  (CharRange[0][2][0]) "]"> */ INST(LIT, 13),
    /*   205 <-----:Literal  (CharRange[0][2][0]) "]"> */ INST(GOTO, 206),
    /*   206 <----:Predicate 102 (CharRange[0][2]) []> */ INST(PMATCHF, 210),
    /*   207 <----:Predicate 102 (CharRange[0][2]) []> */ INST(PNOMATS, 210),
    /*   208 <----:Identifier 103 (CharRange[0][3]) "Char"> */ INST(IDENT, 12),
    /*   209 <----:Identifier 103 (CharRange[0][3]) "Char"> */ INST(GOTO, 210),
    /*   210 <--:Choice 99 (CharRange) []>            */ INST(CISUCC, 216),
    /*   211 <--:Choice 99 (CharRange) []>            */ INST(CIFAIL, 0),
    /*   212 <---:Identifier 104 (CharRange[1]) "Char"> {E} */ INST(IDENT, 12),
    /*   213 <---:Identifier 104 (CharRange[1]) "Char"> {E} */ INST(GOTO, 214),
    /*   214 <--:Choice 99 (CharRange) []>            */ INST(CISUCC, 216),
    /*   215 <--:Choice 99 (CharRange) []>            */ INST(CIFAIL, 0),
    /*   216 <--:Choice 99 (CharRange) []>            */ INST(CFAIL, 217),
    /*   217 <-:Definition 97 (def:CharRange) []> {}  */ INST(ISUCC, 11),
    /*   218 <-:Definition 97 (def:CharRange) []> {}  */ INST(IFAIL, 0),
    /*   219 <--:Choice 107 (Char) []>                */ INST(CHOICE, 0),
    /*   220 <---:Identifier 108 (Char[0]) "EscChar"> {E} */ INST(IDENT, 13),
    /*   221 <---:Identifier 108 (Char[0]) "EscChar"> {E} */ INST(GOTO, 222),
    /*   222 <--:Choice 107 (Char) []>                */ INST(CISUCC, 232),
    /*   223 <--:Choice 107 (Char) []>                */ INST(CIFAIL, 0),
    /*   224 <---:Identifier 109 (Char[1]) "OctChar"> {E} */ INST(IDENT, 14),
    /*   225 <---:Identifier 109 (Char[1]) "OctChar"> {E} */ INST(GOTO, 226),
    /*   226 <--:Choice 107 (Char) []>                */ INST(CISUCC, 232),
    /*   227 <--:Choice 107 (Char) []>                */ INST(CIFAIL, 0),
    /*   228 <---:Identifier 110 (Char[2]) "PlainChar"> {E} */ INST(IDENT, 15),
    /*   229 <---:Identifier 110 (Char[2]) "PlainChar"> {E} */ INST(GOTO, 230),
    /*   230 <--:Choice 107 (Char) []>                */ INST(CISUCC, 232),
    /*   231 <--:Choice 107 (Char) []>                */ INST(CIFAIL, 0),
    /*   232 <--:Choice 107 (Char) []>                */ INST(CFAIL, 233),
    /*   233 <-:Definition 105 (def:Char) []> {}      */ INST(ISUCC, 12),
    /*   234 <-:Definition 105 (def:Char) []> {}      */ INST(IFAIL, 0),
    /*   235 <---:Literal  (EscChar[0]) "\\">         */ INST(LIT, 15),
    /*   236 <---:Literal  (EscChar[0]) "\\">         */ INST(GOTO, 239),
    /*   237 <---:CharClass  (EscChar[1]) []>         */ INST(CHRCLS, 16),
    /*   238 <---:CharClass  (EscChar[1]) []>         */ INST(GOTO, 239),
    /*   239 <-:Definition 111 (def:EscChar) []> {L}  */ INST(ISUCC, 13),
    /*   240 <-:Definition 111 (def:EscChar) []> {L}  */ INST(IFAIL, 0),
    /*   241 <--:Choice 116 (OctChar) []>             */ INST(CHOICE, 0),
    /*   242 <----:Literal  (OctChar[0][0]) "\\">     */ INST(LIT, 15),
    /*   243 <----:Literal  (OctChar[0][0]) "\\">     */ INST(GOTO, 250),
    /*   244 <----:CharClass  (OctChar[0][1]) []>     */ INST(CHRCLS, 17),
    /*   245 <----:CharClass  (OctChar[0][1]) []>     */ INST(GOTO, 250),
    /*   246 <----:CharClass  (OctChar[0][2]) []>     */ INST(CHRCLS, 18),
    /*   247 <----:CharClass  (OctChar[0][2]) []>     */ INST(GOTO, 250),
    /*   248 <----:CharClass  (OctChar[0][3]) []>     */ INST(CHRCLS, 18),
    /*   249 <----:CharClass  (OctChar[0][3]) []>     */ INST(GOTO, 250),
    /*   250 <--:Choice 116 (OctChar) []>             */ INST(CISUCC, 263),
    /*   251 <--:Choice 116 (OctChar) []>             */ INST(CIFAIL, 0),
    /*   252 <----:Literal  (OctChar[1][0]) "\\">     */ INST(LIT, 15),
    /*   253 <----:Literal  (OctChar[1][0]) "\\">     */ INST(GOTO, 261),
    /*   254 <----:CharClass  (OctChar[1][1]) []>     */ INST(CHRCLS, 18),
    /*   255 <----:CharClass  (OctChar[1][1]) []>     */ INST(GOTO, 261),
    /*   256 <----:Repeat 119 (OctChar[1][2]) []>     */ INST(RQBEG, 0),
    /*   257 <-----:CharClass  (OctChar[1][2][0]) []> */ INST(CHRCLS, 18),
    /*   258 <-----:CharClass  (OctChar[1][2][0]) []> */ INST(GOTO, 259),
    /*   259 <----:Repeat 119 (OctChar[1][2]) []>     */ INST(RQMAT, 0),
    /*   260 <----:Repeat 119 (OctChar[1][2]) []>     */ INST(RQDONE, 0),
    /*   261 <--:Choice 116 (OctChar) []>             */ INST(CISUCC, 263),
    /*   262 <--:Choice 116 (OctChar) []>             */ INST(CIFAIL, 0),
    /*   263 <--:Choice 116 (OctChar) []>             */ INST(CFAIL, 264),
    /*   264 <-:Definition 114 (def:OctChar) []> {L}  */ INST(ISUCC, 14),
    /*   265 <-:Definition 114 (def:OctChar) []> {L}  */ INST(IFAIL, 0),
    /*   266 <---:Predicate 123 (PlainChar[0]) []>    */ INST(PRED, 0),
    /*   267 <----:Literal  (PlainChar[0][0]) "\\">   */ INST(LIT, 15),
    /*   268 <----:Literal  (PlainChar[0][0]) "\\">   */ INST(GOTO, 269),
    /*   269 <---:Predicate 123 (PlainChar[0]) []>    */ INST(PMATCHF, 272),
    /*   270 <---:Predicate 123 (PlainChar[0]) []>    */ INST(PNOMATS, 272),
    /*   271 <---:Dot 124 (PlainChar[1]) ".">         */ INST(DOT, 272),
    /*   272 <-:Definition 120 (def:PlainChar) []> {L} */ INST(ISUCC, 15),
    /*   273 <-:Definition 120 (def:PlainChar) []> {L} */ INST(IFAIL, 0),
    /*   274 <--:CharClass  (PredOp) []>              */ INST(CHRCLS, 19),
    /*   275 <--:CharClass  (PredOp) []>              */ INST(GOTO, 276),
    /*   276 <-:Definition 125 (def:PredOp) []> {L}   */ INST(ISUCC, 16),
    /*   277 <-:Definition 125 (def:PredOp) []> {L}   */ INST(IFAIL, 0),
    /*   278 <--:CharClass  (RepOp) []>               */ INST(CHRCLS, 20),
    /*   279 <--:CharClass  (RepOp) []>               */ INST(GOTO, 280),
    /*   280 <-:Definition 127 (def:RepOp) []> {L}    */ INST(ISUCC, 17),
    /*   281 <-:Definition 127 (def:RepOp) []> {L}    */ INST(IFAIL, 0),
    /*   282 <--:Literal  (Dot) ".">                  */ INST(LIT, 21),
    /*   283 <--:Literal  (Dot) ".">                  */ INST(GOTO, 284),
    /*   284 <-:Definition 129 (def:Dot) []> {L}      */ INST(ISUCC, 18),
    /*   285 <-:Definition 129 (def:Dot) []> {L}      */ INST(IFAIL, 0),
    /*   286 <--:Repeat 133 (S) []>                   */ INST(RSBEG, 0),
    /*   287 <---:Choice 134 (S[0]) []>               */ INST(CHOICE, 0),
    /*   288 <----:Repeat 135 (S[0][0]) []> {E}       */ INST(RPBEG, 0),
    /*   289 <-----:CharClass  (S[0][0][0]) []>       */ INST(CHRCLS, 22),
    /*   290 <-----:CharClass  (S[0][0][0]) []>       */ INST(GOTO, 291),
    /*   291 <----:Repeat 135 (S[0][0]) []> {E}       */ INST(RPMAT, 288),
    /*   292 <----:Repeat 135 (S[0][0]) []> {E}       */ INST(RPDONE, 293),
    /*   293 <---:Choice 134 (S[0]) []>               */ INST(CISUCC, 310),
    /*   294 <---:Choice 134 (S[0]) []>               */ INST(CIFAIL, 0),
    /*   295 <-----:Literal  (S[0][1][0]) "#">        */ INST(LIT, 23),
    /*   296 <-----:Literal  (S[0][1][0]) "#">        */ INST(GOTO, 308),
    /*   297 <-----:Repeat 137 (S[0][1][1]) []>       */ INST(RSBEG, 0),
    /*   298 <-------:Predicate 139 (S[0][1][1][0][0]) []> */ INST(PRED, 0),
    /*   299 <--------:CharClass  (S[0][1][1][0][0][0]) []> */ INST(CHRCLS, 24),
    /*   300 <--------:CharClass  (S[0][1][1][0][0][0]) []> */ INST(GOTO, 301),
    /*   301 <-------:Predicate 139 (S[0][1][1][0][0]) []> */ INST(PMATCHF, 304),
    /*   302 <-------:Predicate 139 (S[0][1][1][0][0]) []> */ INST(PNOMATS, 304),
    /*   303 <-------:Dot 140 (S[0][1][1][0][1]) "."> */ INST(DOT, 304),
    /*   304 <-----:Repeat 137 (S[0][1][1]) []>       */ INST(RSMAT, 297),
    /*   305 <-----:Repeat 137 (S[0][1][1]) []>       */ INST(RSDONE, 0),
    /*   306 <-----:CharClass  (S[0][1][2]) []>       */ INST(CHRCLS, 24),
    /*   307 <-----:CharClass  (S[0][1][2]) []>       */ INST(GOTO, 308),
    /*   308 <---:Choice 134 (S[0]) []>               */ INST(CISUCC, 310),
    /*   309 <---:Choice 134 (S[0]) []>               */ INST(CIFAIL, 0),
    /*   310 <---:Choice 134 (S[0]) []>               */ INST(CFAIL, 311),
    /*   311 <--:Repeat 133 (S) []>                   */ INST(RSMAT, 286),
    /*   312 <--:Repeat 133 (S) []>                   */ INST(RSDONE, 0),
    /*   313 <-:Definition 131 (def:S) []> {I}        */ INST(ISUCC, 19),
    /*   314 <-:Definition 131 (def:S) []> {I}        */ INST(IFAIL, 0)
};
ByteCode peg_byte_code = {
    20,
    peg_def_names,
    peg_def_flags,
    peg_def_addrs,
    315,
    peg_instructions,
    25,
    (unsigned char **)peg_strings,
    peg_str_len,
};
#define SANITY_CHECKS 1
#define DEBUG_ERRORS 0
#define ERRORS 1
#define ERRORS_PRED 1
#define ERRORS_IDENT 1
#define ERRORS_TERMINALS 1
#define ERROR_REPEAT_INHIBIT 0 // probably flawed idea, don't enable

static char *OpNames[] =
{
    "GOTO",
    "IDENT",
    "ISUCC",
    "IFAIL",
    "RSBEG",
    "RQBEG",
    "CHOICE",
    "CISUCC",
    "CFAIL",
    "CIFAIL",
    "RPBEG",
    "RPMAT",
    "RPDONE",
    "RSMAT",
    "RSDONE",
    "RQDONE",
    "RQMAT",
    "PRED",
    "PMATCHF",
    "PNOMATF",
    "PMATCHS",
    "PNOMATS",
    "CHRCLS",
    "LIT",
    "DOT",
    "SUCC",
    "FAIL"
};

// debugging print helper, caller must free() returned value
unsigned char *esc_string(const unsigned char *str, int str_length, int limit)
{
    static const char *hex_digit = "0123456789ABCDEF";
    static const char *esc_chars = "\\\"\t\r\n";
    if (!str || str_length < 0) { return NULL; }
    if (limit > 0 && limit < 3) { return NULL; }

    int clen = 0, len = 0, i = 0, trunc = 0;
    unsigned char c = 0;
    unsigned char *res = NULL;

    for (i = 0; i < str_length; ++i) {
        clen = 1;
        c = str[i];
        if (strchr(esc_chars, c)) {
            clen = 2;
        }
        else if (c < 32 || c > 126) {
            clen = 4;
        }

        if (limit && len + clen > limit - 3) {
            trunc = 1;
            break;
        }
        len += clen;
    }

    clen = i;
    if (trunc) { len += 3; }

    res = (unsigned char *)malloc(len + 1);
    unsigned char *p = res;
    for (i = 0; i < clen; ++i) {
        c = str[i];
        if (strchr(esc_chars, c)) {
            *p++ = '\\';
            switch (c) {
                case '\r': *p++ = 'r'; break;
                case '\t': *p++ = 't'; break;
                case '\n': *p++ = 'n'; break;
                default: *p++ = c; break;
            }
        }
        else if (c < 32 || c > 126) {
            *p++ = '\\';
            *p++ = 'x';
            *p++ = hex_digit[(c >> 4) & 0xf];
            *p++ = hex_digit[c & 0xf];
        }
        else {
            *p++ = c;
        }
    }
    if (trunc) { *p++ = '.'; *p++ = '.'; *p++ = '.'; }
    *p = '\0';
    return res;
}

void Node_print(Node *self, Parser *parser, const unsigned char *input, int depth)
{
    int flags = self->flags;
    unsigned char *data = esc_string(&input[self->offset], self->length, 40);
    const char *def_name = Parser_def_name(parser, self->def);

    if (depth == 0) {
        printf("---------------------------------------------------------------------------------\n");
        printf(" Begin    Len  DefID  Flags  Def. Name / Data\n");
        printf("---------------------------------------------------------------------------------\n");
    }
    printf("%6d %6d %6d | %s%s%s | %*s%s \"%s\"\n",
        self->offset,
        self->length,
        self->def,
        flags & STOP ? "S" : " ",
        flags & IGNORE ? "I" : " ",
        flags & LEAF ? "L" : " ",
        depth * 2, "",
        def_name ? def_name : "<N/A>",
        data ? data : (unsigned char *)""
        );
    if (data) { free(data); data = NULL; }
    for (Node *p = self->head; p; p = p->next) {
        Node_print(p, parser, input, depth + 1);
    }
}

Node *Node_new(int def, int offset, int length, int flags)
{
    Node *self = (Node *)malloc(sizeof(Node));
    self->def = def;
    self->offset = offset;
    self->length = length;
    self->flags = flags;
    self->num_children = 0;
    self->head = NULL;
    self->next = NULL;
    return self;
}

void Node_free(Node *self)
{
    Node *tmp;
    for (Node *p = self->head; p; p = tmp) {
        tmp = p->next;
        Node_free(p);
    }
    self->head = NULL;
    free(self);
}

Node *Node_push_child(Node *self, int def, int offset, int length, int flags)
{
    Node *node = Node_new(def, offset, length, flags);
    node->next = self->head;
    self->head = node;
    ++(self->num_children);
    return node;
}

void Node_pop_child(Node *self)
{
    if (self->head) {
        Node *tmp = self->head;
        self->head = self->head->next;
        Node_free(tmp);
        --(self->num_children);
    }
}

Node *Node_unwrap(Node *self)
{
    if (!(self->flags & (STOP|LEAF)) && self->num_children == 1) {
        Node *tmp = Node_unwrap(self->head);
        self->head = NULL;
        Node_free(self);
        return tmp;
    }
    Node *p = self->head; self->head = NULL;
    Node *tmp;
    for (; p; p=tmp) {
        tmp = p->next;
        p = Node_unwrap(p);
        p->next = self->head;
        self->head = p;
    }
    return self;
}

//
// Parser
//

Parser *Parser_new(const ByteCode *byte_code)
{
    Parser *self = (Parser *)malloc(sizeof(Parser));

    self->num_defs = byte_code->num_defs;
    self->def_names = byte_code->def_names;
    self->def_flags = byte_code->def_flags;
    self->def_addrs = byte_code->def_addrs;
    self->num_instructions = byte_code->num_instructions;
    self->instructions = byte_code->instructions;
    self->num_strings = byte_code->num_strings;
    self->strings = byte_code->strings;
    self->str_len = byte_code->str_len;

    self->tree_root = NULL;
    self->max_tree_depth = 256;
    self->max_stack_size = 256 * 8;
    self->error_offset = -1;
    self->error_parent_def = -1;
    self->error_def = -1;
    self->error_inst = -1;
    self->error_expected = -1;

    return self;
}

void Parser_free(Parser *self)
{
    if (self->tree_root) {
        Node_free(self->tree_root);
        self->tree_root = NULL;
    }
    free(self);
}

void Parser_print_tree(Parser *self, const unsigned char *input)
{
    Node_print(self->tree_root, self, input, 0);
}

void Parser_expected(Parser *self, int parent_def, int def, int inst, int offset, int expected)
{
    if (offset >= self->error_offset && !(def == 0 && inst == -1)) {
        self->error_offset = offset;
        self->error_parent_def = parent_def;
        self->error_def = def;
        self->error_inst = inst;
        self->error_expected = expected;
    }
}

void Parser_print_error(Parser *self, const unsigned char *input)
{
#if ERRORS
    const char *parent_def_name = Parser_def_name(self, self->error_parent_def);
    const char *def_name = Parser_def_name(self, self->error_def);

    if (self->error_offset >= 0) {
        if (self->error_inst >= 0) {
            int op = self->error_inst & 0xff;
            int arg = self->error_inst >> 8;
            unsigned char *esc = NULL;
            const char *str = NULL;
            switch (op) {
                case DOT:
                    str = "character"; break;
                case IDENT:
                    str = Parser_def_name(self, arg); break;
                case LIT:
                    esc = esc_string((unsigned char *)self->strings[arg], self->str_len[arg], 20);
                    break;
                default: // unhandled op, show instruction in <> for debugging
                    esc = esc_string((unsigned char *)&self->error_inst, sizeof(int), 20);
                    break;
            }
            printf("%s \"%s\" in %s at offset %d\n",
                    self->error_expected ? "Expected" : "Unexpected",
                    str ? (unsigned char *)str : esc,
                    def_name ? def_name : "<N/A>",
                    self->error_offset);
            if (esc) {
                free(esc);
                esc = NULL;
            }
        }
        else {
            if (parent_def_name) {
                printf("%s %s in %s at offset %d\n",
                        self->error_expected ? "Expected" : "Unexpected",
                        def_name ? def_name : "<N/A>",
                        parent_def_name ? parent_def_name : "<N/A>",
                        self->error_offset);
            }
            else {
                printf("%s %s at offset %d\n",
                        self->error_expected ? "Expected" : "Unexpected",
                        def_name ? def_name : "<N/A>",
                        self->error_offset);
            }
        }
    }
    else {
        printf("No errors detected / tracked.\n");
    }
#else
    printf("Error tracking disabled at compile time.\n");
#endif
}

const char *Parser_def_name(Parser *self, int index)
{
    if (index >= 0 && index < self->num_defs) {
        return self->def_names[index];
    }
    return 0;
}

// TODO: check sanity checks and overflow checks, make macros to make it easier
int Parser_parse(Parser *self, const unsigned char *input, int size)
{
    int offset = 0, locked = 0;

#if ERRORS
    int err_locked = 0;
#endif

    int stack[self->max_stack_size]; int top = -1;
    Node *tree_stack[self->max_tree_depth]; int tree_top = -1;

    const int *instructions = self->instructions;

    int op = 0, arg = 0, pc = 0;

    if (size < 0) { return INVALID_SIZE; }

    self->tree_root = Node_new(0, 0, -1, 0);
    if (tree_top >= self->max_tree_depth - 2) return TREE_STACK_OVERFLOW;
    tree_stack[++tree_top] = self->tree_root;

    unsigned long long cnt = 0, cnt_max = 0;
    cnt_max = (size <= 2642245) ? (size < 128 ? 2097152 : size * size * size) : (unsigned long long)-1LL;
#if SANITY_CHECKS
    int num_inst = self->num_instructions;
    for(cnt = 0; cnt < cnt_max && pc < num_inst; ++cnt, ++pc) {
#else
    for(cnt = 0; cnt < cnt_max; ++cnt, ++pc) {
#endif
        op = instructions[pc] & 0xff;
        arg = instructions[pc] >> 8;

        /*
        printf("pc=%d op=%d arg=%d top=%d tt=%d stack: ", pc, op, arg, top, tree_top);
        for (int i = 0; i <= top; i++) {
            printf("%d ", stack[i]);
        }
        printf("\n");
        */

        switch (op) {
            case GOTO: // arg = addr; GOTO addr; pc is set to addr
                pc = arg; break;

// Identifier
            case IDENT: // arg = def; Identifier "call"; on success, next instruction skipped (See ISUCC, IFAIL)
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW;
                if (!locked) {
                    if (tree_top >= self->max_tree_depth - 2) return TREE_STACK_OVERFLOW;
                    if (self->def_flags[arg] & (LEAF | IGNORE)) {
                        stack[++top] = 1; locked = 1;
                    }
                    else {
                        stack[++top] = 0;
                    }
                    tree_stack[tree_top+1] = Node_push_child(tree_stack[tree_top], arg, offset, -1, self->def_flags[arg]);
                    ++tree_top;
                }
                else {
                    stack[++top] = 0;
                }
                stack[++top] = offset;
                stack[++top] = pc;
                pc = self->def_addrs[arg];
                break;

            case ISUCC: // arg = def; Identifier "call" match success, "return", pc restored to pc+1, skipping next instruction
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
#endif
                pc = stack[top--] + 1;
                --top;
                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
                    tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                    --tree_top;
                    if (self->def_flags[arg] & IGNORE) {
#if SANITY_CHECKS
                        if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                        Node_pop_child(tree_stack[tree_top]);
                    }
                }
                break;

            case IFAIL: // Identifier "call" match failure, "return", pc restored (next instruction not skipped)
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
#endif
                pc = stack[top--];
                offset = stack[top--];

#if ERRORS && ERRORS_IDENT
                if (!err_locked) { // Tracking errors here is probably bare minimum of usefulness
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, -1, offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif

                if (stack[top--] == 1) { locked = 0; }
                if (!locked) {
#if SANITY_CHECKS
                    if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                    Node_pop_child(tree_stack[--tree_top]);
                }
                break;

// Choice
            case CHOICE:
                if (top >= self->max_stack_size - 3) return STACK_OVERFLOW;
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case CISUCC: // arg = success/fail pc addr
            case CFAIL:
#if SANITY_CHECKS
                if (top < 1) return STACK_UNDERFLOW;
#endif
                top -= 2;
                pc = arg;
                break;

            case CIFAIL:
                // backtrack
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                break;


// Repeat +
            case RPBEG:
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW;
#if ERRORS && ERROR_REPEAT_INHIBIT
                stack[++top] = 0; // used to inhibit error tracking after 1st rep
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // offset - backtrack point
                stack[++top] = 0; // cnt (match count)
                break;

            case RPMAT: // arg = loop pc addr
                ++stack[top]; // incr match count
                if (offset != stack[top-1]) { // only loop if consuming
                    stack[top-2] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top-1] = offset; // update backtrack point
                    pc = arg; // continue looping
#if ERRORS && ERROR_REPEAT_INHIBIT
                    if (!err_locked) { // inhibit error tracking after 1st rep
                        stack[top-3] = 1; err_locked = 1;
                    }
#endif
                }
                break;

            case RPDONE: // arg = match fail pc addr
#if SANITY_CHECKS
                if (top < 3) return STACK_UNDERFLOW;
#endif
                offset = stack[top-1];
                for (int i = tree_stack[tree_top]->num_children - stack[top-2]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                if (stack[top] > 0) { // op+ SUCCESS
                    top -= 3;
                }
                else { // op+ FAIL
                    top -= 3;
                    pc = arg;
                }
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (stack[top--]) { err_locked = 0; } // reenable error tracking (if we disabled it)
#endif
                break;

// Repeat *|?
            case RSBEG:
            case RQBEG:
                if (top >= self->max_stack_size - 4) return STACK_OVERFLOW;
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (!err_locked) { // inhibit error tracking
                    stack[++top] = 1; err_locked = 1;
                }
                else {
                    stack[++top] = 0;
                }
#endif
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
                break;

            case RSMAT: // arg = loop pc addr
                if (offset != stack[top]) { // only loop if consuming
                    stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                    stack[top] = offset; // update backtrack point
                    pc = arg; // continue looping
                } // else next instruction, which is a R*DONE
                break;

            case RSDONE: // * always succeeds, proceeds to next instr.
            case RQDONE: // ? always succeeds, proceeds to next instr.
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
#endif
                offset = stack[top];
                for (int i = tree_stack[tree_top]->num_children - stack[top-1]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                top -= 2;
#if ERRORS && ERROR_REPEAT_INHIBIT
                if (stack[top--]) { err_locked = 0; } // reenable error tracking (if we disabled it)
#endif
                break;

// Repeat ?
            case RQMAT: // no looping for ?
                stack[top-1] = tree_stack[tree_top]->num_children; // update backtrack point
                stack[top] = offset; // update backtrack point
                break;

// Predicate
            case PRED:
                // Predicate begin, should be followed with child instructions,
                // then PMATCH{S,F}, then PNOMAT{S,F}, depending on op (&,!)
                if (top >= self->max_stack_size - 3) return STACK_OVERFLOW;
                stack[++top] = tree_stack[tree_top]->num_children; // num_children - backtrack point
                stack[++top] = offset; // save offset for backtrack
#if ERRORS
#if ERRORS_PRED
                stack[++top] = pc + 1; // 1st child inst address for error
#endif
                if (!err_locked) {
                    stack[++top] = 1; err_locked = 1;
                }
                else {
                    stack[++top] = 0;
                }
#endif
                break;

            case PMATCHF: // Predicate matched, match is considered failure, arg = failure address
            case PNOMATF: // Predicate not matched, not match is considered failure, arg = failure address
#if ERRORS && ERRORS_PRED
                if (stack[top]) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[stack[top-1]],
                            offset, (PNOMATF == op));
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                goto pred_cleanup;

            case PMATCHS: // Predicate matched, match is considered success; next instruction skipped
                ++pc;
                // passthrough to pred_cleanup

            case PNOMATS: // Predicate not matched, not match is considered success
pred_cleanup:
#if SANITY_CHECKS
                if (top < 2) return STACK_UNDERFLOW;
#endif
#if ERRORS
                if (stack[top--]) { err_locked = 0; }
#if ERRORS_PRED
                --top;
#endif
#endif
                offset = stack[top--]; // restore saved offset (don't consume)
                for (int i = tree_stack[tree_top]->num_children - stack[top--]; i > 0; --i)
                    Node_pop_child(tree_stack[tree_top]);
                break;

// CharClass
            case CHRCLS: // arg = str_id; match CharClass; skip next instruction on match
                {
                    if (offset < size) {
                        int mlen = self->str_len[arg], i;
                        const unsigned char *mstr = self->strings[arg];
                        for (i = 0; i < mlen; ++i) {
                            if (mstr[i] == input[offset]) {
                                ++offset;
                                ++pc;
                                break;
                            }
                            if ((i < mlen - 2) && (mstr[i+1] == '-')) {
                                if ((input[offset] >= mstr[i]) && (input[offset] <= mstr[i+2])) {
                                    ++offset;
                                    ++pc;
                                    break;
                                }
                                i+=2;
                            }
                        }
                        if (i < mlen) break;
                    }
#if ERRORS && ERRORS_TERMINALS
                    if (!err_locked) {
                        Parser_expected(self,
                                tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                                tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                        Parser_print_error(self, input);
#endif
                    }
#endif
                }
                break;

// Literal
            case LIT: // arg = str_id; match literal string; skip next instruction on match
                {
                    int len = self->str_len[arg];
                    if ((offset < (size - (len - 1))) && !memcmp(&input[offset], self->strings[arg], len)) {
                        offset += len;
                        ++pc;
                        break;
                    }
                }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                break;

// Dot
            case DOT: // arg = fail addr; match any char; goto addr on failure
                if (offset < size) { offset++; break; }
#if ERRORS && ERRORS_TERMINALS
                if (!err_locked) {
                    Parser_expected(self,
                            tree_top > 0 ? tree_stack[tree_top-1]->def : -1,
                            tree_stack[tree_top]->def, instructions[pc], offset, 1);
#if DEBUG_ERRORS
                    Parser_print_error(self, input);
#endif
                }
#endif
                pc = arg;
                break;

// End
            case SUCC: // overall success
#if SANITY_CHECKS
                if (tree_top < 0) return TREE_STACK_UNDERFLOW;
#endif
                tree_stack[tree_top]->length = offset - tree_stack[tree_top]->offset;
                --tree_top;
                self->tree_root = Node_unwrap(self->tree_root);
                if (top != -1) return UNEXPECTED_STACK_DATA;
                if (tree_top != -1) return UNEXPECTED_TREE_STACK_DATA;
                return offset;

            case FAIL: // overall failure
                return arg < 0 ? arg : PARSE_FAILED;

            default:
                return UNKNOWN_INSTRUCTION;
        }
    }
    return RUNAWAY;
}

//
// ByteCode
//

ByteCode *ByteCode_new()
{
    ByteCode *self = (ByteCode *)malloc(sizeof(ByteCode));
    if (NULL == self) { return self; }
    self->num_defs = 0;
    self->def_names = NULL;
    self->def_flags = NULL;
    self->def_addrs = NULL;
    self->num_instructions = 0;
    self->instructions = NULL;
    self->num_strings = 0;
    self->strings = NULL;
    self->str_len = NULL;
    return self;
}

void ByteCode_free(ByteCode *self)
{
    int i;

    if (self->num_defs > 0) {
        for (i = 0; i < self->num_defs; ++i) {
            free(self->def_names[i]);
        }
        if (self->def_names) {
            free(self->def_names);
            self->def_names = NULL;
        }
        if (self->def_flags) {
            free(self->def_flags);
            self->def_flags = NULL;
        }
        if (self->def_addrs) {
            free(self->def_addrs);
            self->def_addrs = NULL;
        }
        self->num_defs = 0;
    }

    if (self->num_instructions > 0) {
        if (self->instructions) {
            free(self->instructions);
            self->instructions = NULL;
        }
        self->num_instructions = 0;
    }

    if (self->num_strings > 0) {
        for (i = 0; i < self->num_strings; ++i) {
            free(self->strings[i]);
        }
        free(self->strings);
        free(self->str_len);
        self->num_strings = 0;
    }
    free(self);
}


static inline const char *op_name(int op)
{
    return (op >= 0 && op < NUM_OPS) ? OpNames[op] : "N/A";
}

void ByteCode_print_instructions(ByteCode *self)
{
    for (int i = 0; i < self->num_instructions; ++i) {
        printf("INST %8d %12s %8d\n", i, op_name(self->instructions[i] & 0xff), self->instructions[i] >> 8);
    }
}

void ByteCode_print_defs(ByteCode *self)
{
    for (int i = 0; i < self->num_defs; ++i) {
        printf("DEF  %8d %12s %8d %6d\n", i, self->def_names[i], self->def_addrs[i], self->def_flags[i]);
    }
}

void ByteCode_print_strings(ByteCode *self)
{
    unsigned char *tmp;
    for (int i = 0; i < self->num_strings; ++i) {
        tmp = esc_string(self->strings[i], self->str_len[i], 256);
        printf("STR  %8d %8d \"%s\"\n", i, self->str_len[i], tmp);
        free(tmp);
    }
}

void ByteCode_print(ByteCode *self)
{
    ByteCode_print_defs(self);
    ByteCode_print_instructions(self);
    ByteCode_print_strings(self);
}

int ByteCode_compare(ByteCode *a, ByteCode *b)
{
    int i;

    /* Defs */

    if (a->num_defs != b->num_defs) {
        printf("a->num_defs != b->num_defs\n");
        return 1;
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (strcmp(a->def_names[i], b->def_names[i])) {
            printf("a->def_names[%d] = '%s'\n", i, a->def_names[i]);
            printf("b->def_names[%d] = '%s'\n", i, b->def_names[i]);
            return 2;
        }
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (a->def_flags[i] != b->def_flags[i]) {
            printf("a->def_flags[%d] = %d\n", i, a->def_flags[i]);
            printf("b->def_flags[%d] = %d\n", i, b->def_flags[i]);
            return 3;
        }
    }
    for (i = 0; i < a->num_defs; ++i) {
        if (a->def_addrs[i] != b->def_addrs[i]) {
            printf("a->def_addrs[%d] = %d\n", i, a->def_addrs[i]);
            printf("b->def_addrs[%d] = %d\n", i, b->def_addrs[i]);
            return 4;
        }
    }

    /* Instructions */

    if (a->num_instructions != b->num_instructions) {
        printf("a->num_instructions != b->num_instructions\n");
        return 5;
    }
    for (i = 0; i < a->num_instructions; ++i) {
        if (a->instructions[i] != b->instructions[i]) {
            printf("a->instructions[%d] = %s %d\n", i,
                    op_name(a->instructions[i] & 0xff), a->instructions[i] >> 8);
            printf("b->instructions[%d] = %s %d\n", i,
                    op_name(b->instructions[i] & 0xff), b->instructions[i] >> 8);
            return 6;
        }
    }

    /* Strings */

    if (a->num_strings != b->num_strings) {
        printf("a->num_strings != b->num_strings\n");
        return 7;
    }

    for (i = 0; i < a->num_strings; ++i) {
        if (a->str_len[i] != b->str_len[i] || memcmp(a->strings[i], b->strings[i], a->str_len[i])) {
            unsigned char *tmp;
            tmp = esc_string(a->strings[i], a->str_len[i], 256);
            printf("a->strings[%d] = \"%s\" (len=%d)\n", i, tmp, a->str_len[i]);
            free(tmp);
            tmp = esc_string(b->strings[i], b->str_len[i], 256);
            printf("b->strings[%d] = \"%s\" (len=%d)\n", i, tmp, b->str_len[i]);
            free(tmp);
            return 8;
        }
    }

    return 0;
}
