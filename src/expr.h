#include <stdio.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "lexParser.h"
#include "strRoutines.h"
#include "syntaxAnalyzer.h"
#include "tokenMap.h"

// TODO: copy function bodies for stack

typedef enum {
	terminal, nonterminal
} ExprType;

typedef enum {
	none, boolean = 15, integer, real, string // none is for '$' and initialized ExprTokens
} ExprDataType;

typedef struct exprtoken{
	char content; // contains terminal converted to a single char - terminal or E
	ExprType type;
	ExprDataType datatype;
} ExprToken;


typedef ExprToken stackElementT;

typedef struct stackTag {
	stackElementT data;
	struct stackTag *next;
} stackNodeT;

typedef struct {
	stackNodeT *top;
} Stack;

Stack * Stack__init__();
void Stack_push(Stack * s, stackElementT data) ;
stackElementT Stack_pop(Stack * s);

