#ifndef EXPR_h
#define EXPR_h

#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"
#include "lex_parser.h"
#include "parser_tables.h"
#include "stack.h"
#include "str_routines.h"
#include "token_buff.h"
#include "token_map.h"
#include "expr_token.h"
#include "semantic_analyzer.h"

//#define EXPR_DEBUG

DECLARE_STACK(expr, ExprToken);

typedef struct {
	TokenBuff * tokenBuff;
	InstrQueue * instructions;
	exprStack stack;
} ExprParser;

void ExprParser__init__(ExprParser * self, TokenBuff * tokenBuff,
		InstrQueue * instructions);
tIFJ ExprParser_parse(ExprParser * self);
void ExprParser__dell__(ExprParser * self);
void Syntax_err_throw(ExprParser * self, char * tokenName, char * msg);
void Syntax_err_throw_t(ExprParser * self, Token lastToken, char * msg);
void Syntax_err_throw_et(ExprParser * self, ExprToken lastToken, char * msg);

#endif
