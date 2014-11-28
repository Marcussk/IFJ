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
#ifdef EXPR_DEGUG
		printf("<%d:content - %s, type - %d, datatype - %d, shifted - %d/>\n",
				poss, getTokenName(itr->data.content), itr->data.type,
				itr->data.datatype, itr->data.shifted);
#endif
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

	//free(Expr->value);
	if (Token_isValue(token)){
		Expr->value = malloc(sizeof(iVal));
	}else
		Expr->value = NULL;

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

InstrCode tokenToInstruction(Token token){
	switch(token){
	case t_plus:
		return i_add;
	case t_minus:
		return i_sub;
	case t_asterisk:
		return i_mul;
	case t_slash:
		return i_div;
	case t_less:
		return i_less;
	case t_greater:
		return i_more;
	case t_lessOrEqv:
		return i_loreq;
	case t_greaterOrEqv:
		return i_moreq;
	case t_eqv:
		return i_equal;
	case t_notEqv:
		return i_nequal;
	default:
		return i_noop;
	}
}
void reduceRule(exprStack *stack, ExprToken *TopMostTerminal,
		TokenBuff *tokenBuff, InstrQueue * instructions) {
	ExprToken operand1, operator, operand2, lastItem, result, parameter;
	InstrParam * param;
	Token cont = TopMostTerminal->content;
#ifdef EXPR_DEGUG
	printf("-----%d\n", cont);
#endif
	switch (cont) {
	case t_id:
		TopMostTerminal->type = nonterminal;
		param = malloc(sizeof(InstrParam));
		if (TopMostTerminal->id) {
			param->stackAddr = TopMostTerminal->id->stackIndex;
			InstrQueue_insert(instructions, (Instruction ) { i_push, iStackRef,
							param, NULL, NULL });

		} else if (TopMostTerminal->value) {
			*param = iVal2InstrP(*TopMostTerminal->value, TopMostTerminal->datatype);
			InstrQueue_insert(instructions, (Instruction ) { i_push,
							TopMostTerminal->datatype, param,
							NULL, NULL });
		} else {
			sem_Error("Use of id which have no iVar and no iVal");
		}
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
#ifdef EXPR_DEGUG
		printf("Time to reduce binary operation (+,-,*,/,<,>,..)\n");
		printf("STACK POSITION = %d\n", findHandle(stack));
#endif
		if (findHandle(stack) < 4)
			syntaxError("Expression syntax error - missing operands",
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
		//InstrQueue_insert(&instr, (Instruction ) { operator.content, result.datatype, operand1, operand2, result });

		result.type = nonterminal;
		exprStack_push(stack, result);
		// [TODO] instr add, eq, etc...
		break;
	case t_rParenthessis:
		if (findHandle(stack) < 4)
			syntaxError("Expression syntax error - not enough operands",
					tokenBuff->lp->lineNum, ",");

		if (TopMostTerminal->content != stack->top->data.content) // ')' Must be on top of stack
			syntaxError("Expression syntax error - expected )",
					tokenBuff->lp->lineNum, "");
		exprStack_pop(stack); // Pop ')'

		ExprTokenInit(&lastItem);
		lastItem = stack->top->data; // Should be '(' or E

		ExprTokenInit(&result);

		if (lastItem.content == t_lParenthessis && lastItem.type == terminal) { // '()' Function with no parameters or an empty expession
#ifdef EXPR_DEGUG
				printf("Generate call instruction\n"); // empty expression not implemented yet
#endif
			exprStack_pop(stack); // Pop ')'
			result.type = nonterminal;
		}

		else if (lastItem.type == nonterminal) { // We have found E) found
			result = parameter = exprStack_pop(stack); // might be parameter, needs to be saved later
			if (result.datatype == iString){
				printf("String in parameter: %s\n", result.value->iString);
			}
			lastItem = stack->top->data;
			if (lastItem.content == t_lParenthessis
					&& lastItem.type == terminal) { // (E) - not sure if function with 1 parameter or just an expression
				exprStack_pop(stack);
				lastItem = stack->top->data;
				result.type = nonterminal;
				if (lastItem.content == t_func && lastItem.type == terminal) { // got id(E)
				// Push parameter to instruction queue here
				//exprStack_push(stack, result); // Keep
					result = exprStack_pop(stack);
					result.type = nonterminal;
					exprStack_push(stack, result); // Keep
					printf("Function type: %d\n", result.id->val.fn->builtin);
					/* Do tohoto ifu vloz push
					 * instrukci muzes dat natvrdo i_write a typ iString
					 * samotny string na vypsani je v result.value->iString (u printf o par radku vys to vytiskne spravne :))
					 */

					if (result.id->val.fn->builtin == b_write){
						param = malloc(sizeof(InstrParam));
						param->iString = result.value->iString;
						InstrQueue_insert(instructions, (Instruction ) { i_push, iString ,  param,  NULL, NULL });
						InstrQueue_insert(instructions, (Instruction ) { i_write, iString ,  NULL,  NULL, NULL });
					}
				} else { // It's just (E)
#ifdef EXPR_DEGUG
				printf("It's just normal E\n");
#endif
					exprStack_push(stack, result);
				}
			} else if (lastItem.content == t_comma && lastItem.type == terminal) // Found ,E) -> function with more parameters, we do not consider that yet
				unimplementedError(
						"Functions with more than 1 parameters not implemented");
			else
				// Syntax error
				syntaxError("Syntax Error - expected ) or function parameters",
						-1, "");
		}

		else
			syntaxError("Syntax Error - expected ) or function parameters", -1,
					"");
		break;
	default:
		syntaxError("unknown content of ExprToken", -1, "");
	}

}

void expression(TokenBuff * tokenBuff, InstrQueue * instructions) {
	ExprToken *TopMostTerminal;
	exprStack *stack = malloc(sizeof(exprStack));
	if (!stack)
		memoryError("expression can't allocate memory for new stack\n");
	ExprInit(stack);
Token lastToken = TokenBuff_next(tokenBuff);
#ifdef EXPR_DEGUG
	printf("<Expr Line: %d>\n", tokenBuff->lp->lineNum);
#endif
	tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken

	while (!(Token_isKeyword(lastToken) || lastToken == t_scolon)) { // cann't  require anything else
		printStack(stack);

		TopMostTerminal = findTopMostTerminal(stack);
#ifdef EXPR_DEGUG
		printf("prtable indexes [%d][%d]\n", TopMostTerminal->content,
				ExprLastToken.content);
#endif
		switch (prTable[TopMostTerminal->content][ExprLastToken.content]) {
		case shift:		// Vloz zacatek handle
#ifdef EXPR_DEGUG
		printf("shift\n");
#endif
			TopMostTerminal->shifted = true;
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
#ifdef EXPR_DEGUG
			printf("next symbol read\n");
#endif
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			break;

		case equal:	// push ExprLastToken
#ifdef EXPR_DEGUG
		printf("equal\n");
#endif
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
#ifdef EXPR_DEGUG
			printf("next symbol read\n");
#endif
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken
			break;

		case reduce: // Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo a zredukuj
#ifdef EXPR_DEGUG
		printf("reduce\n");
#endif
			reduceRule(stack, TopMostTerminal, tokenBuff, instructions);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
			break;

		case error:
			syntaxError("Expression Error, error state from prTable",
					tokenBuff->lp->lineNum, getTokenName(lastToken));
		};
        }
	while (true) {
		TopMostTerminal = findTopMostTerminal(stack);
		if (stack->size == 2 && stack->top->data.type == nonterminal) { // only $ and S
			break;
		}
		if (prTable[TopMostTerminal->content][t_eof] == reduce) {
#ifdef EXPR_DEGUG
			printf("reduce\n");
#endif
			reduceRule(stack, TopMostTerminal, NULL, instructions);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
		} else {
			printStack(stack);
			syntaxError("Expression Error Everything read, can't reduce",
					tokenBuff->lp->lineNum, getExprTokenName(ExprLastToken));
		}
	}
#ifdef EXPR_DEGUG
	printf("Last stack status\n");
	printStack(stack);

	printf("</Expr lastToken:%d - %s >\n\n", lastToken,
			getTokenName(lastToken));
#endif
	TokenBuff_pushBack(tokenBuff, lastToken);
}
