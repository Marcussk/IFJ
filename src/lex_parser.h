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
#include "errors_structs.h"

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

void LexParser__init__(LexParser * p, BuffFile input);
Token LexParser_next(LexParser * p);
void LexParser__dell__(LexParser * p);
void LexParser_fnParamsEnter(LexParser * self);
void LexParser_fnBodyLeave(LexParser * self);
void LexParser_fnBodyEnter(LexParser * self, iVar * fn);
void Lex_throwError(LexParser * self, char *  msg);
Err_syntax Syntax_err_create(LexParser * lp);
void Definition_err_throw(LexParser * lp);
void Type_err_throw(LexParser * lp, char * msg);
void Semantic_err_throw(LexParser * lp, char * msg);

#endif
