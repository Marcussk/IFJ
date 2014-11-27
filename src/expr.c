#include "expr.h"

IMPLEMENT_STACK(expr, ExprToken);

ExprToken *ExprEndToken;
ExprToken *ExprLastToken;
ExprToken *TopMostTerminal;

int getTokenContent(Token token) {
	switch ((int) token) {
	case t_num_int:
	case t_num_real:
	case t_str_val:
		return t_id;
	default:
		return (int) token;
	}
}

ExprToken *ExprTokenInit(ExprToken *token) {
	token = malloc(sizeof(ExprToken));
	if (!token) {
		memoryError("ExprTokenInit can't allocate memory for new token\n");
	}
	token->content = t_eof;
	token->shifted = false;
	token->type = terminal;
	token->datatype = iUnknown;
	token->id = NULL;
	token->value = NULL;
	return token;
}

void ExprInit(exprStack *stack) {
	exprStack__init__(stack);
	ExprEndToken = ExprTokenInit(ExprEndToken);
	ExprLastToken = ExprTokenInit(ExprLastToken);
	exprStack_push(stack, *ExprEndToken);
}

bool isValue(Token t) {
	return t == t_num_int || t == t_num_real || t == t_str_val || t == t_true
			|| t == t_false;
}

void tokenToExpr(ExprToken *Expr, Token token, LexParser * lp) {
	Expr->content = getTokenContent(token);
	Expr->type = terminal;

	if (token == t_id) {
		Expr->id = lp->lastSymbol;
		Expr->datatype = lp->lastSymbol->type;
	} else
		Expr->id = NULL;

	if (isValue(token))
		Expr->value = malloc(sizeof(iVal));

	switch (token) {
	case t_num_int:
		if (!sscanf(lp->str.buff, "%d", &(Expr->value->iInt)))
			lexError("Cannot parse int num", lp->str.buff, lp->lineNum);
		Expr->datatype = iInt;
		break;

	case t_num_real:
		if (!sscanf(lp->str.buff, "%f", &(Expr->value->iReal)))
			lexError("Cannot parse real num", lp->str.buff, lp->lineNum);
		Expr->datatype = iReal;
		break;

	case t_str_val:
		Expr->value->iString = strdup(lp->str.buff);
		if (!Expr->value->iString)
			lexError("Cannot parse string", lp->str.buff, lp->lineNum);
		Expr->datatype = iString;
		break;
	case t_true:
		Expr->value->iInt = 1;
		Expr->datatype = iBool;
		break;
	case t_false:
		Expr->value->iInt = 0;
		Expr->datatype = iBool;
		break;
	default:
		Expr->value = NULL;
	}
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

int findHandle(exprStack * stack) {
	int i = 0;
	exprStackNodeT * tmp = stack->top;
	while (tmp != NULL) {
		i++;
		if (tmp->data.shifted)
			return i;
		tmp = tmp->next;
	}
	return 0;

}

void reduceRule(exprStack *stack, ExprToken *TopMostTerminal, TokenBuff *tokenBuff) {
	ExprToken *operand1, *operator, *operand2;
	operand1 = ExprTokenInit(operand1);
	operator = ExprTokenInit(operator);
	operand2 = ExprTokenInit(operand2);
	switch (TopMostTerminal->content) {
	case t_id:
		printf("STACK POSITION = %d\n", findHandle(stack));
		TopMostTerminal->type = nonterminal;
		//TopMostTerminal->shifted = false;
		break;
	case t_plus:
	case t_minus:
	case t_asterisk:
	case t_slash:
	case t_less:
	case t_greater:
	case t_lessOrEqv:
	case t_greaterOrEqv:
	case t_eqv:
	case t_notEqv:
		printf("STACK POSITION = %d\n", findHandle(stack));
		if (findHandle(stack) < 4)
			syntaxError("Expression syntax error - not enough operands", tokenBuff->lp->lineNum, ",");

		if (TopMostTerminal != &(stack->top->next->data)) // Check if TopMostTerminal is operator - terminal
			syntaxError("Expression Error - Operator error", tokenBuff->lp->lineNum, ",");

		*operand2 = exprStack_pop(stack);
		*operator = exprStack_pop(stack);
		*operand1 = stack->top->data;

		if (operand2->type != nonterminal || operand1->type != nonterminal){
			syntaxError("Expression Error - Operands error", tokenBuff->lp->lineNum, ",");
		}

		printf("Time to reduce binary operation (+,-,*,/,<,>,..)\n");
		//unimplementedError("Operations with 2 operands not implemented\n");
		break;
	}

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

bool iVar_isFn(iVar * v) {
	return v->type == iFn;
}

void expression(TokenBuff * tokenBuff, InstrQueue * istructions) {
	printf("<Expr Line: %d>\n", tokenBuff->lp->lineNum);
	/*
	 * if iVar_isFn read '(' (and save in token '(' iVar of the fn )
	 *  for all params do expression (with this stack) there mus be somethink like test
	 *  on whitch param we are and if type fits (you will have pointer on param of function (iVar.fn.params ...)
	 *  and if it have nex you have to read next param or throw syntaxErr .. )
	 *
	 * Do it, do it now !!! :-D
	 * */
	exprStack *stack;
	stack = malloc(sizeof(exprStack));
	if (!stack)
		memoryError("expression can't allocate memory for new stack\n");

	ExprInit(stack);

	Token lastToken = TokenBuff_next(tokenBuff);
	tokenToExpr(ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken

	while (1) {
		if (Token_isKeyword(lastToken) || lastToken == t_scolon)
			break;
		TopMostTerminal = findTopMostTerminal(stack);

		printf("prTable indexes - [%d][%d]\n", TopMostTerminal->content,
				ExprLastToken->content);

		switch (prTable[TopMostTerminal->content][ExprLastToken->content]) {
		case shift:
			printf("shift\n");
			TopMostTerminal->shifted = true;
			exprStack_push(stack, *ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			break;
			// Vloz zacatek handle
		case equal:
			printf("equal\n");
			exprStack_push(stack, *ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			// push ExprLastToken
			break;
		case reduce:
			// Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo
			// a zredukuj
			printf("reduce\n");
			reduceRule(stack, TopMostTerminal, tokenBuff);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
			break;
		case error:
			syntaxError("Expression Error", tokenBuff->lp->lineNum,
					getTokenName(lastToken));
		};
		printStack(stack);
	}

	TopMostTerminal = findTopMostTerminal(stack);
	// !! check if all the items on stack can be reduced, else error
	// There is some terminal on stack - error
	if (TopMostTerminal->content != ExprEndToken->content) {
		syntaxError("Expression ended unexpectedly", tokenBuff->lp->lineNum,
				getTokenName(lastToken));
	}

	printf("</Expr lastToken:%d - %s >\n\n", lastToken,
			getTokenName(lastToken));
	TokenBuff_pushBack(tokenBuff, lastToken);
}
