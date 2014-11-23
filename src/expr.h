#ifndef EXPR_h
#define EXPR_h

#include <stdio.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "lexParser.h"
#include "strRoutines.h"
#include "syntaxAnalyzer.h"
#include "tokenMap.h"
#include "stack.h"
#include "parserTables.h"

// TODO: copy function bodies for stack

typedef enum {
	terminal, nonterminal
} ExprType;

typedef enum {
	none, boolean = 16, integer, real, string // none is for '$' and initialized ExprTokens
} ExprDataType;

typedef struct exprtoken{
	unsigned int content; // contains terminal converted to a single char - terminal or E
	bool shifted;
	ExprType type;
	ExprDataType datatype;
} ExprToken;

DECLARE_STACK(expr, ExprToken);
#endif
