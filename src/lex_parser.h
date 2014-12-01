#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "buffFile.h"
#include "builtins.h"
#include "defs.h"
#include "hash_table.h"
#include "str_routines.h"
#include "token_map.h"


typedef enum { j_continue =0, j_readStr, j_reset, j_readEscape } LexParser_jobPlan;
typedef enum { lp_searchOnly, lp_insertOnly, lp_debug } LexParser_mode;


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
	HashTable * symbolTable;
	LexParser_mode idMode;
} LexParser;

void LexParser__init__(LexParser * p, FILE * inFile);
Token LexParser_next(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_clear(LexParser * p);
void LexParser_fnParamsEnter(LexParser * self);
void LexParser_fnBodyEnter(LexParser * self, tIFJ fnReturnType);
void LexParser_fnBodyLeave(LexParser * self);


#endif
