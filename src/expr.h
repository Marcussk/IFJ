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

//#define EXPR_DEBUG

typedef enum {
	terminal, nonterminal
} ExprType;

typedef struct exprtoken {
	Token content; // contains terminal converted to a single char - terminal or E
	iVar * id; // contains variable to use or function to call
	iVal * value;
	bool shifted;
	ExprType type;
	tIFJ datatype;
} ExprToken;

DECLARE_STACK(expr, ExprToken);

char * getExprTokenName(ExprToken t);
Token getTokenContent(Token token, iVar* var);
void printStack(exprStack *self);
void ExprTokenInit(ExprToken *token);
void ExprInit(exprStack *stack);
void tokenToExpr(ExprToken *Expr, Token token, LexParser * lp);
ExprToken *findTopMostTerminal(exprStack *s);
void reduceRule(exprStack *stack, ExprToken *TopMostTerminal, TokenBuff *tokenBuff, InstrQueue * instructions);
tIFJ expression(TokenBuff * tokenBuff, InstrQueue * istructions);

#endif
