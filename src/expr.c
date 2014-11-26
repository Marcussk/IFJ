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
}

ExprToken *ExprTokenInit(ExprToken *token) {
	token = malloc(sizeof(ExprToken));
	if (!token) {
		memoryError("ExprTokenInit can't allocate memory for new token\n");
	}
	token->content = t_eof;
	token->shifted = false;
	token->type = terminal;
	token->datatype = none;
	return token;
}

void ExprInit(exprStack *stack) {
	exprStack__init__(stack);
	ExprEndToken = ExprTokenInit(ExprEndToken);
	ExprLastToken = ExprTokenInit(ExprLastToken);
	exprStack_push(stack, *ExprEndToken);
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
/*
int getStackPos(exprStack *stack, ExprToken *token)
{
	if(!stack->top)
		return 0;
	int i = 1;
	exprStackNodeT *tmp = stack->top;
	while(!tmp)
		if (tmp->next == NULL)
			return 0;
		if (&(tmp->data) == token)
			return i;
		i++;
		tmp = tmp->next;
}
*/

int findHandle(exprStack * stack) {\
	int i = 0;
	exprStackNodeT * tmp = stack->top;
	while (tmp != NULL)
	{
		i++;
		if (tmp->data.shifted)
			return i;
		tmp = tmp->next;
	}
	return 0;

}

void reduceRule(exprStack *stack, ExprToken *TopMostTerminal)
{
	switch (TopMostTerminal->content)
	{
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
			/*if (getStackPos(stack, TopMostTerminal) < 3)
				printf("syntax error - not enough operands\n");
			*/
			printf("Time to reduce binary operation (+,-,*,/,<,>,..)\n");
			unimplementedError(
					"Operations with 2 operands not implemented\n");
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
void expression(TokenBuff * tokenBuff, InstrQueue * istructions) {
	printf("--Calling expression-- Line: %d\n", tokenBuff->lp->lineNum);

	exprStack *stack;
	stack = malloc(sizeof(exprStack));
	if (!stack)
		memoryError("expression can't allocate memory for new stack\n");

	ExprInit(stack);

	Token lastToken = TokenBuff_next(tokenBuff);
	tokenToExpr(ExprLastToken, lastToken); // "copy" content of LastToken to ExprLastToken
	unsigned int endOfInput = 0;  // Flag for end of input -> when we get keyword or semicolon

	while(1)
	{
		endOfInput = (Token_isKeyword(lastToken) || lastToken == t_scolon);
		TopMostTerminal = findTopMostTerminal(stack);

		if (endOfInput){ // Input has ended, from now, current token will always be '$'
			if (TopMostTerminal->content == ExprEndToken->content){ // There is no terminal on stack
				printf("Break\n");
				break;
			}
			ExprLastToken->content = ExprEndToken->content;
		}

		printf("prTable indexes - [%d][%d]\n", TopMostTerminal->content,
				ExprLastToken->content);

		switch (prTable[TopMostTerminal->content][ExprLastToken->content]) {
			case shift:
				printf("shift\n");
				TopMostTerminal->shifted = true;
				exprStack_push(stack, *ExprLastToken);
				lastToken = TokenBuff_next(tokenBuff);
				tokenToExpr(ExprLastToken, lastToken); // "copy" content of LastToken to ExprLastToken
				break;
				// Vloz zacatek handle
			case equal:
				printf("equal\n");
				exprStack_push(stack, *ExprLastToken);
				lastToken = TokenBuff_next(tokenBuff);
				tokenToExpr(ExprLastToken, lastToken); // "copy" content of LastToken to ExprLastToken
				// push ExprLastToken
				break;
			case reduce:
				// Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo
				// a zredukuj
				printf("reduce\n");
				reduceRule(stack, TopMostTerminal);
				TopMostTerminal = findTopMostTerminal(stack);
				TopMostTerminal->shifted = false;
				break;
			case error:
				printf("error\n");
				/*if (TopMostTerminal->content == t_id && (ExprLastToken->content == t_lParenthessis ||
					ExprLastToken->content == t_id)) // maybe useless condition, will be removed later
					break;*/
				syntaxError("Expression Error", tokenBuff->lp->lineNum, "s");

			// Zahlas syntaktickou chybu
		};
		printStack(stack);
	}

	TopMostTerminal = findTopMostTerminal(stack);
	// There is some terminal on stack - error
	if (TopMostTerminal->content != ExprEndToken->content) {
		printf("Syntax error in expression\n");
	}

	printf("Last token - %d - %s\n--Returning from expression\n\n", lastToken,
			getTokenName(lastToken));
	TokenBuff_pushBack(tokenBuff, lastToken);
}
