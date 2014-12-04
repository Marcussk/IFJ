#ifndef EXPR_TOKEN_h
#define EXPR_TOKEN_h

#include <stdbool.h>
#include "token_map.h"
#include "ifj_types.h"
#include "lex_parser.h"

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

char * ExprToken_getName(ExprToken t);
Token Token_getContent(Token token, iVar* var);
void ExprToken_Init(ExprToken *token);
void tokenToExpr(ExprToken *Expr, Token token, LexParser * lp);

#endif
