#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "strRoutines.h"

#include "tokenMap.h"

typedef enum {
	lp_read, lp_comment, lp_string
} LexParserStates;

typedef struct {
	void (*newTokenEv)(int);
	String str;
	LexParserStates state;
	TokenParser tParser;
	int lineNum;
} LexParser;

LexParser LexParser__init__( void (*newTokenEv)(int));
void LexParser_push(LexParser * p, char ch);
void LexParser__dell__(LexParser * p);
void LexParser_clear(LexParser * p);

#endif
