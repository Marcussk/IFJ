#ifndef SYNTAX_ANALYZER_h
#define SYNTAX_ANALYZER_h

#include "expr.h"
#include "lexParser.h"
#include "hashTable.h"
#include "ifjTypes.h"
#include "stack.h"
#include "errorHandler.h"
#include "instruction.h"
#include "expr.h"
#include "tokenBuff.h"
#include "semanticAnalyzer.h"

typedef struct {
	LexParser * lp;
	TokenBuff tokBuff;
	InstrQueue instr;
	int stackIndexCntr;
} SyntaxAnalyzer;


void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp);
void SyntaxAnalyzer_parse(SyntaxAnalyzer * self);
void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parseCond(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_block(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self);
tIFJ SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self);



#endif
