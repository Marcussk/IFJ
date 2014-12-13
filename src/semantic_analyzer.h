#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>

#include "ifj_types.h"
#include "instruction.h"
#include "i_function.h"
#include "token_map.h"
#include "lex_parser.h"

//the entire header file file
void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2,LexParser * lp);
void SemAnalyzer_checkcond(tIFJ param, LexParser * lp);
void SemAnalyzer_typeconvert(InstrQueue * self, tIFJ param1, tIFJ param2, Token operator);
#endif
