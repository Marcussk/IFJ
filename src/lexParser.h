#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "strRoutines.h"
#include "yield.h"
#include "hashTable.h"
#include "tokenMap.h"
#include "buffFile.h"


extern HashTable * symbolTable;

typedef struct {
	BuffFile input;
	TokenParser tParser;
	String str;// alias buffer
	int lineNum;
	Token lastToken;
	iVar * lastSymbol;
} LexParser;

void LexParser__init__(LexParser * p, FILE * inFile);
Token LexParser_gen(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_clear(LexParser * p);

#endif
