#ifndef EXPR_h
#define EXPR_h

#include <stdio.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "lexParser.h"
#include "strRoutines.h"
#include "tokenMap.h"
#include "stack.h"
#include "parserTables.h"
#include "tokenBuff.h"

/*
 *
 * findhandle not used initialization, but use of unitialized (badly written initializer)
 *
 * */

typedef enum {
	terminal, nonterminal
} ExprType;

/*
typedef enum {
	none, boolean = 16, integer, real, string // none is for '$' and initialized ExprTokens
} ExprDataType;*/

typedef struct exprtoken{
	Token content; // contains terminal converted to a single char - terminal or E
	iVar * id; // contains variable to use or function to call
	iVal * value;
	bool shifted;
	ExprType type;
	tIFJ datatype;
} ExprToken;

DECLARE_STACK(expr, ExprToken);

#endif
