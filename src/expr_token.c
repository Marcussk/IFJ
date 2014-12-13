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
		*Expr->value = str2iVal(lp->str.buff, token, lp);
		Expr->datatype = Token_getType(token);
	} else
		Expr->value = NULL;
}


iVal str2iVal(char * str, Token token, LexParser * lp) {
	iVal val;
	switch (token) {
	case t_num_int:
		if (!sscanf(str, "%d", &(val.iInt)))
			Lex_throwError(lp, "Cannot parse int num");
		break;

	case t_num_real:
		if (!sscanf(str, "%f", &(val.iReal)))
			Lex_throwError(lp,"Cannot parse real num");
		break;

	case t_str_val:
		val.iString = strdup(str);
		if (!val.iString)
			Lex_throwError(lp,"Cannot parse string");
		break;
	case t_true:
		val.iInt = 1;
		break;
	case t_false:
		val.iInt = 0;
		break;
	default:
		Lex_throwError(lp,"cannot convert value");
	}
	return val;
}
