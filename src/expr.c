#include "expr.h"

IMPLEMENT_STACK(expr, ExprToken);

ExprToken ExprLastToken;

char * getExprTokenName(ExprToken t) {
	switch (t.type) {
	case terminal:
		return "terminal";
	case nonterminal:
		return "nonterminal";
	}
	return "Unknown";
}

Token getTokenContent(Token token, iVar* var) {
	switch (token) {
	case t_num_int:
	case t_num_real:
	case t_str_val:
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

void printStack(exprStack *self) {
	exprStackNodeT *itr = self->top;
	int poss = self->size - 1;
	while (itr != NULL) {
		printf("<%d:content - %s, type - %d, datatype - %d, shifted - %d/>\n",
				poss, getTokenName(itr->data.content), itr->data.type,
				itr->data.datatype, itr->data.shifted);
		itr = itr->next;
		poss--;
	}
}

void ExprTokenInit(ExprToken *token) {
	token->content = t_eof;
	token->shifted = false;
	token->type = terminal;
	token->datatype = iUnknown;
	token->id = NULL;
	token->value = NULL;
}

void ExprInit(exprStack *stack) {
	ExprToken start;
	ExprTokenInit(&start);
	exprStack__init__(stack);
	exprStack_push(stack, start);
	ExprTokenInit(&ExprLastToken);
}

void tokenToExpr(ExprToken *Expr, Token token, LexParser * lp) {
	Expr->content = getTokenContent(token, lp->lastSymbol);
	Expr->type = terminal;

	if (token == t_id) {
		Expr->id = lp->lastSymbol;
		Expr->datatype = lp->lastSymbol->type;
	} else
		Expr->id = NULL;

	if (Token_isValue(token))
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

void reduceRule(exprStack *stack, ExprToken *TopMostTerminal,
		TokenBuff *tokenBuff) {
	ExprToken operand1, operator, operand2, result;
	Token cont = TopMostTerminal->content;
	printf("-----%d\n", cont);
	switch (cont) {
	case t_id:
		TopMostTerminal->type = nonterminal;
		// [TODO] instr pop
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
		printf("Time to reduce binary operation (+,-,*,/,<,>,..)\n");
		printf("STACK POSITION = %d\n", findHandle(stack));
		if (findHandle(stack) < 4)
			syntaxError("Expression syntax error - not enough operands",
					tokenBuff->lp->lineNum, ",");

		if (TopMostTerminal != &(stack->top->next->data)) // Check if TopMostTerminal is operator - terminal
			syntaxError("Expression Error - Operator error",
					tokenBuff->lp->lineNum, ",");

		operand2 = exprStack_pop(stack);
		operator = exprStack_pop(stack);
		operand1 = exprStack_pop(stack);

		if (operand2.datatype != operand1.datatype)
			sem_Error("Incompatible operands");

		if (operand2.type != nonterminal || operand1.type != nonterminal) {
			syntaxError("Expression Error - Operands error",
					tokenBuff->lp->lineNum, "nonterminal probably ','");
		}
		ExprTokenInit(&result);
		if (cont == t_less || cont == t_greater || cont == t_lessOrEqv
				|| cont == t_greaterOrEqv || cont == t_eqv
				|| cont == t_notEqv) {
			result.datatype = iBool;
		} else {
			result.datatype = operand1.datatype;
		}

		result.type = nonterminal;
		exprStack_push(stack, result);
		// [TODO] instr add, eq, etc...
		break;
	case t_rParenthessis:
		unimplementedError("Right parenthesis not implented yet");
		break;
	default:
		syntaxError("unknown content of ExprToken", -1, "");
	}

}

void expression(TokenBuff * tokenBuff, InstrQueue * istructions) {
	ExprToken *TopMostTerminal;
	exprStack *stack = malloc(sizeof(exprStack));
	Token lastToken = TokenBuff_next(tokenBuff);
	if (!stack)
		memoryError("expression can't allocate memory for new stack\n");
	ExprInit(stack);

	printf("<Expr Line: %d>\n", tokenBuff->lp->lineNum);

	tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken

	while (!(Token_isKeyword(lastToken) || lastToken == t_scolon)) { // cann't  require anything else
		TopMostTerminal = findTopMostTerminal(stack);
		printf("prtable indexes [%d][%d]\n", TopMostTerminal->content, ExprLastToken.content);
		switch (prTable[TopMostTerminal->content][ExprLastToken.content]) {
		case shift:		// Vloz zacatek handle
			printf("shift\n");
			TopMostTerminal->shifted = true;
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			break;

		case equal:	// push ExprLastToken
			printf("equal\n");
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			break;

		case reduce: // Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo a zredukuj
			printf("reduce\n");
			reduceRule(stack, TopMostTerminal, tokenBuff);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
			break;

		case error:
			syntaxError("Expression Error, error state from prTable",
					tokenBuff->lp->lineNum, getTokenName(lastToken));
		};
		printStack(stack);
	}
	while (true) {
		TopMostTerminal = findTopMostTerminal(stack);
		if (stack->size == 2 && stack->top->data.type == nonterminal) // only $ and S
			break;
		if (prTable[TopMostTerminal->content][t_eof] == reduce) {
			printf("reduce\n");
			reduceRule(stack, TopMostTerminal, tokenBuff);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
		} else {
			printStack(stack);
			syntaxError("Expression Error all readen and canot reduce",
					tokenBuff->lp->lineNum, getExprTokenName(ExprLastToken));
		}
	}

	printf("</Expr lastToken:%d - %s >\n\n", lastToken,
			getTokenName(lastToken));
	TokenBuff_pushBack(tokenBuff, lastToken);
}
