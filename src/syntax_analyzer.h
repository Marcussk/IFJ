#ifndef SYNTAX_ANALYZER_h
#define SYNTAX_ANALYZER_h

#include <stdbool.h>
#include "error_handler.h"
#include "expr_parser.h"
#include "expr_parser.h"
#include "stack.h"
#include "instruction.h"
#include "hash_table.h"
#include "ifj_types.h"
#include "lex_parser.h"
#include "semantic_analyzer.h"
#include "token_buff.h"
#include "defs.h"


typedef struct {
	LexParser * lp;
	TokenBuff tokBuff;
	InstrQueue instr;
	int stackIndexCntr;
	bool isInGlobals;
	bool mainBodyParsed;
} SyntaxAnalyzer;


void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp);
void SyntaxAnalyzer_parse(SyntaxAnalyzer * self);
void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parseCond(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_block(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_repeat(SyntaxAnalyzer * self);
tIFJ SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self);



#endif
