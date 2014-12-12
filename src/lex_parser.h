#ifndef LEX_PARSER_h
#define LEX_PARSER_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "buff_file.h"
#include "builtins.h"
#include "defs.h"
#include "hash_table.h"
#include "str_routines.h"
#include "token_map.h"
#include "i_function.h"

typedef enum {
	lp_searchOnly, lp_insertOnly, lp_fnSearch, lp_parseParams, lp_ignore
} LexParser_mode;

typedef struct {
	BuffFile input;
	String str; // alias buffer
	iVar * lastSymbol;
	iFunction * lastFunction;
	HashTable * symbolTable;
	LexParser_mode idMode;
} LexParser;

void LexParser__init__(LexParser * p, FILE * inFile);
Token LexParser_next(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_fnParamsEnter(LexParser * self);
void LexParser_fnBodyLeave(LexParser * self);
void LexParser_fnBodyEnter(LexParser * self, iVar * fn);

#endif
