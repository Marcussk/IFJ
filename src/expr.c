#include "expr.h"

IMPLEMENT_STACK(expr, ExprToken);

ExprToken *ExprEndToken;
ExprToken *ExprLastToken;
ExprToken *TopMostTerminal;

int tokenToChar(Token token) {
	switch ((int) token) {
		case t_num_int:
		case t_num_real:
		case t_str_val:
			return t_id;
		default:
			return (int) token;
	}
}

int getTokenType(Token token) {
	switch (token) {
	case t_true:
	case t_false:
		return boolean;
	case t_num_int:
	case t_num_real:
	case t_str_val:
		return (int) token;
	default:
		return none;
	};
	return none;
}

void tokenToExpr(ExprToken *Expr, Token token) {
	Expr->content = tokenToChar(token); //tokenToChar(token);
	Expr->type = terminal;
	Expr->datatype = getTokenType(token);
}

ExprToken *findTopMostTerminal(exprStack *s) {
	exprStackNodeT * itr = s->top;
	while (itr != NULL) {
		if (itr->data.type == terminal)
			return &(itr->data);
		itr = itr->next;
	}
	return NULL;
}
ExprToken *ExprTokenInit(ExprToken *token) {
	token = malloc(sizeof(ExprToken));
	if (!token) {
		memoryError("ExprTokenInit cann't alloc memory\n");
	}
	token->content = t_eof;
	token->shifted = false;
	token->type = terminal;
	token->datatype = none;
	return token;
}

void printStack(exprStack *self) {
	exprStackNodeT *itr = self->top;
	while (itr != NULL) {
		printf("----content - %d, type - %d, datatype - %d, shifted - %d\n",
				itr->data.content, itr->data.type, itr->data.datatype,
				itr->data.shifted);
		itr = itr->next;
	}
}

void ExprInit(exprStack *stack) {
	exprStack__init__(stack);
	ExprEndToken = ExprTokenInit(ExprEndToken);
	ExprLastToken = ExprTokenInit(ExprLastToken);
	exprStack_push(stack, *ExprEndToken);
}

void expression(TokenBuff * tokenBuff, InstrQueue * istructions) {
	printf("--Calling expression-- Line: %d\n", tokenBuff->lp->lineNum);

	exprStack *stack;
	stack = malloc(sizeof(exprStack));
	ExprInit(stack);

	Token lastToken = TokenBuff_next(tokenBuff);

	do {
		tokenToExpr(ExprLastToken, lastToken); // "copy" content of LastToken to ExprLastToken
		TopMostTerminal = findTopMostTerminal(stack);
		printf("prTable indexes - [%d][%d]\n", TopMostTerminal->content,
				ExprLastToken->content);
		switch (prTable[TopMostTerminal->content][ExprLastToken->content]) {
		case shift:
			printf("shift\n");
			TopMostTerminal->shifted = true;
			exprStack_push(stack, *ExprLastToken);
			break;
			// Vloz zacatek handle
		case equal:
			printf("equal\n");
			exprStack_push(stack, *ExprLastToken);
			// push ExprLastToken
			break;
		case reduce:
			// Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo
			// a zredukuj
			printf("reduce\n");
			break;
		case error:
			printf("error\n");

			// Zahlas syntaktickou chybu
		};
		printStack(stack);
		lastToken = TokenBuff_next(tokenBuff);
	} while (!Token_isKeyword(lastToken) && lastToken != t_scolon);

	// There is some terminal on stack - error
	if (findTopMostTerminal(stack) != ExprEndToken->content) {
		printf("Syntax error in expression\n");
	}

	printf("Last token - %d - %s\n--Returning from expression\n\n", lastToken,
			getTokenName(lastToken));

	TokenBuff_pushBack(tokenBuff, lastToken);
}
