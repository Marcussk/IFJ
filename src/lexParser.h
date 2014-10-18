#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "strRoutines.h"

#include "tokenMap.h"

typedef enum {
	lp_read, lp_string, lp_comment, lp_escape, lp_error
} LexParserStates;

typedef struct sLexParser {
	void (*newTokenEv)(struct sLexParser * p, Token t);
	String str;
	LexParserStates state;
	TokenParser tParser;
	int lineNum;
	Token lastToken;
} LexParser;

LexParser LexParser__init__(void (*newTokenEv)(LexParser * p, Token t));
void LexParser_push(LexParser * p, char ch);
void LexParser_flush(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_clear(LexParser * p);

#endif
