#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "strRoutines.h"
#include "yield.h"
#include "hashTable.h"
#include "tokenMap.h"

extern HashTable * symbolTable;

typedef enum {
	lp_read, lp_string, lp_comment, lp_escape, lp_error
} LexParserStates;

typedef struct {
	FILE * input;
	String str;
	LexParserStates state;
	TokenParser tParser;
	int lineNum;
	Token lastToken;
	iVar * lastSymbol;
} LexParser;

void LexParser__init__(LexParser * p, FILE * inFile);
Token LexParser_iterator(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_clear(LexParser * p);

#endif
