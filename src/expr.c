#include "expr.h"

//#define EXPR_DEGUG

#ifdef EXPR_DEBUG
#define EXPR_DEBUGING(body) body
#else
#define EXPR_DEBUGING(body) ;
#endif

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
EXPR_DEBUGING(
		void printStack(exprStack *self) { exprStackNodeT *itr = self->top; int poss = self->size - 1; while (itr != NULL) { printf("<%d:content - %s, type - %d, datatype - %d, shifted - %d/ >\n", poss, getTokenName(itr->data.content), itr->data.type, itr->data.datatype, itr->data.shifted); itr = itr->next; poss--; } })

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
	if (Token_isValue(token)) {
		Expr->value = malloc(sizeof(iVal));
		*Expr->value = str2iVal(lp->str.buff, token, lp->lineNum);
		Expr->datatype = Token_getType(token);
	} else
		Expr->value = NULL;
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

tIFJ getResultType(tIFJ operand1, tIFJ operand2, Token operator) {
	switch (operator) {
	case t_plus:
		if (operand1 == operand2 && (operand1 == iInt || operand1 == iString))
			return operand1;
		else if (operand1 >= iInt && operand1 <= iReal && // int or real
				operand2 >= iInt && operand2 <= iReal)
			return iReal;
		break;
	case t_minus:
	case t_asterisk:
		if (operand1 == iInt && operand2 == iInt)
			return iInt;
		else if (operand1 >= iInt && operand1 <= iReal && // int or real
				operand2 >= iInt && operand1 <= iReal)
			return iReal;
		break;
	case t_slash:
		if (operand1 >= iInt && operand1 <= iReal && // int or real
				operand2 >= iInt && operand2 <= iReal)
			return iReal;
		break;
	case t_less:
	case t_greater:
	case t_lessOrEqv:
	case t_greaterOrEqv:
	case t_eqv:
	case t_notEqv:
		if (operand1 == operand2 && operand1 >= iBool && operand1 <= iString) // any type
			return iBool;
		break;
	default:
		break;
	}
	sem_Error("Unknown operator");
	return iUnknown;
}
int isOperator(Token t) {
	return (t >= t_plus && t <= t_notEqv);
}

void reduceParams(exprStack *stack, TokenBuff *tokenBuff, int paramCount,
		int gotFunc, InstrQueue * instructions) { // ')' already found and popped
	ExprToken *TopMost;
	ExprToken result;
	int i;
	TopMost = malloc(sizeof(ExprToken));
	*TopMost = stack->top->data;

	if (TopMost->type == nonterminal) { // Got parameter
		result = exprStack_pop(stack);
		*TopMost = stack->top->data;
		if (TopMost->content == t_comma) { // this must be a function
			exprStack_pop(stack); // pop comma
			return reduceParams(stack, tokenBuff, ++paramCount, 1, instructions);
		} else if (TopMost->content == t_lParenthessis) {
			exprStack_pop(stack); // Pop '('
			*TopMost = stack->top->data;
			// There must be function id, an operator or nothing on the left from '('
			if (TopMost->content == t_func) {
				InstrQueue_insert(instructions,
						(Instruction ) { i_call,
										TopMost->id->val.fn->retVal.type,
										NULL, NULL, NULL });
				for (i = 0; i < paramCount; i++) {
					InstrQueue_insert(instructions, (Instruction ) { i_call,
									iVoid,
									NULL, NULL, NULL });
				}
				//result.datatype = navratovy typ funkce
				exprStack_pop(stack); // pop t_func on stack
			} else if (gotFunc) //!(isOperator(TopMost->content)) && TopMost->Content != t_eof)
				syntaxError("Expected function id", -1,
						getTokenName(stack->top->data.content));
			exprStack_push(stack, result); //

			stack->top->data.type = nonterminal; // From now, function result must be considered as nonterminal
			return;
		} else if (isOperator(TopMost->content)) // just a regular expression in brackets (maybe useless)
			return;
		else
			syntaxError("Expected function parameter", -1, "");
	} else {
		printf("LastToken = %s\n", getTokenName(TopMost->content));
		syntaxError("Expected something else", -1, "");
	}
}

void reduceRule(exprStack *stack, ExprToken *TopMostTerminal,
		TokenBuff *tokenBuff, InstrQueue * instructions) {
	ExprToken operand1, operator, operand2, result;
	ExprToken *parameter;
	Instruction instr;
	InstrParam * p = NULL;

	// [TODO] useless
	parameter = malloc(sizeof(ExprToken));
	ExprTokenInit(parameter);

	Token cont = TopMostTerminal->content;
	EXPR_DEBUGING(printf("-----%d\n", cont);)
	switch (cont) {
	case t_id: // kdyz var, tak push 1. parametr bude stackaddr
		if (TopMostTerminal->type == terminal) {
			instr.code = i_push;
			instr.a1 = NULL;
			instr.a2 = NULL;
			instr.dest = NULL;
			if (TopMostTerminal->id) {
				if (!(TopMostTerminal->id->isInitialized))
					sem_Error("Uninitialized variable");
				p = malloc(sizeof(InstrParam));
				p->stackAddr = TopMostTerminal->id->stackIndex;
				instr.type = iStackRef;
			} else if (TopMostTerminal->value) {
				p = malloc(sizeof(InstrParam));
				*p = iVal2InstrP(*TopMostTerminal->value,
						TopMostTerminal->datatype);
				instr.type = TopMostTerminal->datatype;
			} else {
				break;
				//is on stack
			}
			TopMostTerminal->type = nonterminal;
			instr.a1 = p;
			InstrQueue_insert(instructions, instr);
			TopMostTerminal->type = nonterminal;
		} else {
			syntaxError("Reduction of token which was already reducted", -1,
					getExprTokenName(*TopMostTerminal));
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
		EXPR_DEBUGING(
				printf("Time to reduce binary operation (+,-,*,/,<,>,..)\n"); printf("STACK POSITION = %d\n", findHandle(stack));)
		if (findHandle(stack) < 4)
			syntaxError("Expression syntax error - missing operands",
					tokenBuff->lp->lineNum, ",");

		if (TopMostTerminal != &(stack->top->next->data)) // Check if TopMostTerminal is operator - terminal
			syntaxError("Expression Error - Operator error",
					tokenBuff->lp->lineNum, ",");

		operand2 = exprStack_pop(stack);
		operator = exprStack_pop(stack);
		operand1 = exprStack_pop(stack);

		if (operand2.type != nonterminal || operand1.type != nonterminal) {
			syntaxError("Expression Error - Operands error",
					tokenBuff->lp->lineNum, "nonterminal probably ','");
		}
		ExprTokenInit(&result);

		result.datatype = getResultType(operand1.datatype, operand2.datatype,
				operator.content);
		InstrParam * paramCnt = malloc(sizeof(InstrParam));
		paramCnt->iInt = 0;
		InstrQueue_insert(instructions,
				(Instruction ) { Token2Instruction(operator.content),
								operand1.datatype, paramCnt,
								NULL, NULL });

		result.type = nonterminal;
		result.content = t_id;
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
		ExprToken last = exprStack_pop(stack);
		if (last.content == t_lParenthessis) {
			last = exprStack_pop(stack);
			if (last.content == t_func) {
				// function with no parameter
				InstrQueue_insert(instructions,
						(Instruction ) { i_call,
										last.id->val.fn->retVal.type, NULL,
										NULL, NULL });
			} else
				syntaxError("Empty brackets are not allowed",
						tokenBuff->lp->lineNum, "");
		}

		exprStack_push(stack, last);

		reduceParams(stack, tokenBuff, 0, 0, instructions);
		result.type = nonterminal;
		result.content = t_id;

		/*
		 ExprTokenInit(&lastItem);
		 lastItem = stack->top->data; // Should be '(' or E

		 ExprTokenInit(&result);

		 if (lastItem.content == t_lParenthessis && lastItem.type == terminal) { // '()' Function with no parameters or an empty expession
		 printf("Generate call instruction\n"); // empty expression not implemented yet
		 exprStack_pop(stack); // Pop ')'
		 result.type = nonterminal;
		 } else if (lastItem.type == nonterminal) { // We have found E) found
		 result = exprStack_pop(stack); // might be parameter, needs to be saved later
		 *parameter = result;

		 lastItem = stack->top->data;
		 if (lastItem.content == t_lParenthessis
		 && lastItem.type == terminal) { // (E) - not sure if function with 1 parameter or just an expression
		 exprStack_pop(stack);
		 lastItem = stack->top->data;
		 result.type = nonterminal;
		 if (lastItem.content == t_func && lastItem.type == terminal) { // got id(E)
		 // Push parameter to instruction queue here
		 result = exprStack_pop(stack);
		 result.type = nonterminal;
		 exprStack_push(stack, result); // Keep
		 if (result.id->val.fn->builtin) {
		 unimplementedError(
		 "others builtins are not implemented yet");
		 }

		 unimplementedError("Call is not implemented now");
		 } else { // It's just (E)
		 EXPR_DEBUGING(printf("It's just normal E\n");)
		 exprStack_push(stack, result);
		 }
		 } else if (lastItem.content == t_comma && lastItem.type == terminal) // Found ,E) -> function with more parameters, we do not consider that yet
		 unimplementedError(
		 "Functions with more than 1 parameters not implemented");
		 else
		 syntaxError("Syntax Error - expected ) or function parameters",
		 -1, "");
		 } else
		 syntaxError("Syntax Error - expected ) or function parameters", -1,
		 "");

		 //unimplementedError("Right parenthesis not implemented yet");
		 */
		break;
	default:
		syntaxError("unknown content of ExprToken", -1, "");
	}

}

void parseWrite(TokenBuff * tokenBuff, InstrQueue * instructions) {
	Token lastToken = TokenBuff_next(tokenBuff);
	if (lastToken != t_lParenthessis)
		syntaxError("writeCall expects '(' after write", tokenBuff->lp->lineNum,
				getTokenName(lastToken));
	iVar * lastSymbol;
	Instruction instr;
	instr.code = i_push;
	instr.a2 = NULL;
	instr.dest = NULL;
	InstrParam * param;

	while (true) {
		lastToken = TokenBuff_next(tokenBuff);
		if (lastToken == t_id) {
			lastSymbol = tokenBuff->lp->lastSymbol;
			if (lastSymbol->type != iFn) {
				if (!lastSymbol->isInitialized)
					sem_Error("Use of uninitialized variable");
				param = malloc(sizeof(InstrParam));
				param->stackAddr = lastSymbol->stackIndex;
				instr.type = iStackRef;
				instr.a1 = param;
			} else {
				syntaxError("Function call cannot be in write call",
						tokenBuff->lp->lineNum, getTokenName(lastToken));
			}
		} else if (Token_isValue(lastToken)) {
			param = malloc(sizeof(InstrParam));
			*param = iVal2InstrP(
					str2iVal(tokenBuff->lp->str.buff, lastToken,
							tokenBuff->lp->lineNum), Token_getType(lastToken));
			instr.type = Token_getType(lastToken);
			instr.a1 = param;
		} else {
			syntaxError("write call unexpected argument",
					tokenBuff->lp->lineNum, getTokenName(lastToken));

		}
		InstrQueue_insert(instructions, instr);
		if (instr.type == iStackRef) {
			instr.type = lastSymbol->type;
		}
		InstrQueue_insert(instructions, (Instruction ) { i_write, instr.type,
				NULL, NULL, NULL });
		lastToken = TokenBuff_next(tokenBuff);
		if (lastToken == t_comma)
			continue;
		else if (lastToken == t_rParenthessis)
			return;
		else
			syntaxError("write call unexpected argument",
					tokenBuff->lp->lineNum, getTokenName(lastToken));

	}
}

void parseReadLn(TokenBuff * tokenBuff, InstrQueue * instructions) {
	Token lastToken = TokenBuff_next(tokenBuff);
	if (lastToken != t_lParenthessis)
		syntaxError("Expected (", tokenBuff->lp->lineNum,
				getTokenName(lastToken));

	iVar * lastSymbol;
	InstrParam * param;

	lastToken = TokenBuff_next(tokenBuff);
	if (lastToken == t_id) {
		lastSymbol = tokenBuff->lp->lastSymbol;
		tokenBuff->lp->lastSymbol->isInitialized = true;
		param = malloc(sizeof(InstrParam));
		param->stackAddr = lastSymbol->stackIndex;
	} else
		syntaxError("Expected identificator", tokenBuff->lp->lineNum,
				getTokenName(lastToken));

	InstrQueue_insert(instructions, (Instruction ) { i_readln, lastSymbol->type,
			NULL, NULL, param });

	lastToken = TokenBuff_next(tokenBuff);
	if (lastToken == t_rParenthessis)
		return;
	else
		syntaxError("write call unexpected argument", tokenBuff->lp->lineNum,
				getTokenName(lastToken));
}

tIFJ expression(TokenBuff * tokenBuff, InstrQueue * instructions) {
	ExprToken *TopMostTerminal;
	exprStack *stack = malloc(sizeof(exprStack));
	if (!stack)
		memoryError("expression can't allocate memory for new stack\n");
	ExprInit(stack);

	Token lastToken = TokenBuff_next(tokenBuff);
	if (lastToken == t_id && tokenBuff->lp->lastSymbol->type == iFn
			&& tokenBuff->lp->lastSymbol->val.fn->builtin) {
		Builtins b = tokenBuff->lp->lastSymbol->val.fn->builtin;
		switch (b) {
		case b_readLn:
			parseReadLn(tokenBuff, instructions);
			return iVoid;
		case b_write:
			parseWrite(tokenBuff, instructions);
			return iVoid;
		default:
			break;
		}
	}EXPR_DEBUGING(printf("<Expr Line: %d>\n", tokenBuff->lp->lineNum);)
	tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp); // "copy" content of LastToken to ExprLastToken

	do {
		TopMostTerminal = findTopMostTerminal(stack);
		EXPR_DEBUGING(
				printStack(stack); printf("prtable indexes [%d][%d]\n", TopMostTerminal->content, ExprLastToken.content);)

		switch (prTable[TopMostTerminal->content][ExprLastToken.content]) {
		case shift:		// Vloz zacatek handle
			EXPR_DEBUGING(printf("shift\n");)
			TopMostTerminal->shifted = true;
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp);
			break;

		case equal:	// push ExprLastToken
			EXPR_DEBUGING(printf("equal\n");)
			exprStack_push(stack, ExprLastToken);
			lastToken = TokenBuff_next(tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, tokenBuff->lp);
			break;

		case reduce: // Search for handle on stack and reduce expression
			EXPR_DEBUGING(printf("reduce\n");)
			reduceRule(stack, TopMostTerminal, tokenBuff, instructions);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
			break;

		case error:
			syntaxError("Expression Error, error state from prTable",
					tokenBuff->lp->lineNum, getTokenName(lastToken));
		};
	} while (!(Token_isKeyword(lastToken) || lastToken == t_scolon)); // cann't  require anything else

	while (true) {
		TopMostTerminal = findTopMostTerminal(stack);
		if (stack->size == 2 && stack->top->data.type == nonterminal) { // only $ and S
			break;
		}
		if (prTable[TopMostTerminal->content][t_eof] == reduce) {
			EXPR_DEBUGING(printf("reduce\n");)
			reduceRule(stack, TopMostTerminal, tokenBuff, instructions);
			TopMostTerminal = findTopMostTerminal(stack);
			TopMostTerminal->shifted = false;
		} else {
			EXPR_DEBUGING(printStack(stack);)
			syntaxError("Expression Error Everything read, can't reduce",
					tokenBuff->lp->lineNum, getExprTokenName(ExprLastToken));
		}
	}EXPR_DEBUGING(
			printf("Last stack status\n"); printStack(stack); printf("</Expr lastToken:%d - %s >\n\n", lastToken, getTokenName(lastToken));)

	TokenBuff_pushBack(tokenBuff, lastToken);
	return (stack->top->data.datatype);
}
