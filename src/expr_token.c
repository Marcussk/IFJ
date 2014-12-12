#include "expr_token.h"


char * ExprToken_getName(ExprToken t) {
	switch (t.type) {
	case terminal:
		return "terminal";
	case nonterminal:
		return "nonterminal";
	}
	return "Unknown";
}

Token Token_getContent(Token token, iVar* var) {
	switch (token) {
	case t_num_int:
	case t_num_real:
	case t_str_val:
	case t_true:
	case t_false:
		return t_id;
	case t_id:
		if (var->type == iFn)
			return t_func;
		else
			return t_id;
	default:
		return token;
	}
}

void ExprToken_Init(ExprToken *token) {
	token->content = t_eof;
	token->shifted = false;
	token->type = terminal;
	token->datatype = iUnknown;
	token->id = NULL;
	token->value = NULL;
}

void tokenToExpr(ExprToken *Expr, Token token, LexParser * lp) {
	Expr->content = Token_getContent(token, lp->lastSymbol);
	Expr->type = terminal;

	if (token == t_id) {
		Expr->id = lp->lastSymbol;
		Expr->datatype = lp->lastSymbol->type;
	} else
		Expr->id = NULL;

	//free(Expr->value);
	if (Token_isValue(token)) {
		Expr->value = malloc(sizeof(iVal));
		*Expr->value = str2iVal(lp->str.buff, token, lp->input.line);
		Expr->datatype = Token_getType(token);
	} else
		Expr->value = NULL;
}
