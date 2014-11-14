#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "strRoutines.h"
#include "hashTable.h"
#include "tokenMap.h"
#include "buffFile.h"


extern HashTable * symbolTable;

typedef enum { j_continue =0, j_readStr, j_reset, j_readEscape } LexParser_jobPlan;
/*
typedef struct {
	String str;
	Token lastToken;
	iVar * lastSymbol;
} Lp_PushBackBuff;
*/
typedef struct {
	BuffFile input;
	TokenParser tParser;
	String str;// alias buffer
	int lineNum;
	Token preLastToken;
	Token lastToken;
	iVar * lastSymbol;
	LexParser_jobPlan planedJob;
	Token pushBackTok;
} LexParser;

void LexParser__init__(LexParser * p, FILE * inFile);
Token LexParser_gen(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_pushBack(LexParser * p, Token t);
void LexParser_clear(LexParser * p);

#endif
