#include "expr_parser.h"

//#define EXPR_DEGUG

#ifdef EXPR_DEBUG
#define EXPR_DEBUGING(body) body
#else
#define EXPR_DEBUGING(body) ;
#endif

IMPLEMENT_STACK(expr, ExprToken);
/*
 EXPR_DEBUGING(
 void printStack(exprStack *self) { exprStackNodeT *itr = self->top; int poss = self->size - 1; while (itr != NULL) { printf("<%d:content - %s, type - %d, datatype - %d, shifted - %d/ >\n", poss, getTokenName(itr->data.content), itr->data.type, itr->data.datatype, itr->data.shifted); itr = itr->next; poss--; } })
 */

void ExprParser__init__(ExprParser * self, TokenBuff * tokenBuff,
		InstrQueue * instructions) {
	self->tokenBuff = tokenBuff;
	self->instructions = instructions;
	ExprToken start;
	ExprToken_Init(&start);
	exprStack__init__(&self->stack);
	exprStack_push(&self->stack, start);
}

ExprToken * findTopMostTerminal(exprStack *s) {
	int i;
	for (i = s->top; i >= 0; i--) {
		if (s->StackArray[i].type == terminal)
			return &(s->StackArray[i]);
	}
	return NULL;
}

int findHandle(exprStack * stack) {
	int i = 0;
	for (i = stack->top; i >= 0; i--) {
		if (stack->StackArray[i].shifted)
			return stack->top - i + 1;
	}
	return 0;
}

iFunction * findFunction(exprStack * stack) {
	int i = 0;
	for (i = stack->top; i > 1; i--) {
		if (stack->StackArray[i].content == t_lParenthessis) {
			if (stack->StackArray[i - 1].content == t_func)
				return stack->StackArray[i - 1].id->val.fn;
			else
				return NULL;
		}
	}
	return NULL;
}

tIFJ getResultType(tIFJ op1Type, tIFJ op2Type, Token operator) {
	switch (operator) {
	case t_plus:
		if (op1Type == op2Type && (op1Type == iInt || op1Type == iString))
			return op1Type;
		else if (op1Type >= iInt && op1Type <= iReal && // int or real
				op2Type >= iInt && op2Type <= iReal)
			return iReal;
		break;
	case t_minus:
	case t_asterisk:
		if (op1Type == iInt && op2Type == iInt)
			return iInt;
		else if (op1Type >= iInt && op1Type <= iReal && // int or real
				op2Type >= iInt && op1Type <= iReal)
			return iReal;
		break;
	case t_slash:
		if (op1Type >= iInt && op1Type <= iReal && // int or real
				op2Type >= iInt && op2Type <= iReal)
			return iReal;
		break;
	case t_less:
	case t_greater:
	case t_lessOrEqv:
	case t_greaterOrEqv:
	case t_eqv:
	case t_notEqv:
		if (op1Type == op2Type && op1Type >= iBool && op1Type <= iString) // any type
			return iBool;
		break;
	default:
		syntaxError("Unknown operator", -1, "");
	}
	sem_Error("Incompatible types", -1);
	return iUnknown;
}

void reduceParams(ExprParser * self, int paramCount, ParamsListItem * paramNode) { // ')' already found and popped
	ExprToken *TopMost;
	ExprToken result;
	TopMost = malloc(sizeof(ExprToken));
	*TopMost = self->stack.StackArray[self->stack.top];

	if (TopMost->type == nonterminal) { // Got parameter
		//printf("%s\n", iVar_type2str(paramNode->data->type));

		result = exprStack_pop(&self->stack); // parameter
		if (!paramNode || result.datatype != paramNode->data->type)
			sem_Error("Bad function parameter", self->tokenBuff->lp->lineNum);

		*TopMost = self->stack.StackArray[self->stack.top];
		if (TopMost->content == t_comma) { // this must be a function
			exprStack_pop(&self->stack); // pop comma

			if (paramNode)
				return reduceParams(self, ++paramCount, paramNode->prev);

		} else if (TopMost->content == t_lParenthessis) {
			exprStack_pop(&self->stack); // Pop '('
			*TopMost = self->stack.StackArray[self->stack.top];
			// There must be function id, an operator or nothing on the left from '('
			if (TopMost->content == t_func) {
				result.datatype = TopMost->id->val.fn->retVal.type;
				InstrParam * p = malloc(sizeof(InstrParam));
				InstrParam * pCount = malloc(sizeof(InstrParam));
				pCount->iInt = paramCount;
				//printf("Function parameter = %d\n", TopMost->id->val.fn->params->next->param->type);
				p->iInt = TopMost->id->val.fn->bodyInstrIndex;

				Builtins b = TopMost->id->val.fn->builtin;
				if (b) {
					InstrQueue_insert(self->instructions,
							(Instruction ) { b, result.datatype, pCount,
									NULL, p });

				} else
					InstrQueue_insert(self->instructions, (Instruction ) { i_call,
									result.datatype, pCount,
									NULL, p });
				exprStack_pop(&self->stack); // pop t_func on stack
			} else if (paramNode) //!(isOperator(TopMost->content)) && TopMost->Content != t_eof)
				syntaxError("Expected function id", -1,
						getTokenName(self->stack.StackArray[self->stack.top].content));
			exprStack_push(&self->stack, result);

			self->stack.StackArray[self->stack.top].type = nonterminal; // From now, function result must be considered as nonterminal
			return;
		} else if (Token_isOperator(TopMost->content)) // just a regular expression in brackets (maybe useless)
			return;
		else
			syntaxError("Expected function parameter", -1, "");
	} else {
		printf("LastToken = %s\n", getTokenName(TopMost->content));
		syntaxError("Expected something else", -1, "");
	}
}

void Expr_reduceBinaryOperator(ExprParser * self) {
	ExprToken operand2 = exprStack_pop(&self->stack);
	ExprToken operator = exprStack_pop(&self->stack);
	ExprToken operand1 = exprStack_pop(&self->stack);
	ExprToken result;

	if (operand2.type != nonterminal || operand1.type != nonterminal) {
		syntaxError("Expression Error - Operands error",
				self->tokenBuff->lp->lineNum, "nonterminal probably ','");
	}
	ExprToken_Init(&result);

	SemAnalyzer_typeconvert(self->instructions, operand1.datatype, operand2.datatype, operator.content);
	operand1.datatype = iReal;
	operand2.datatype = iReal;
	result.datatype = getResultType(operand1.datatype, operand2.datatype,
			operator.content);
	InstrParam * paramCnt = malloc(sizeof(InstrParam));
	paramCnt->iInt = 0;
	InstrQueue_insert(self->instructions,
			(Instruction ) { Token2Instruction(operator.content),
							operand1.datatype, paramCnt,
							NULL, NULL });

	result.type = nonterminal;
	result.content = t_id;
	exprStack_push(&self->stack, result);
}

void reduceParenthesis(ExprParser * self) {
	ExprToken nonTerm = exprStack_pop(&self->stack);
	if (nonTerm.content != t_id || nonTerm.type != nonterminal) {
		syntaxError("parenthesis without expr inside", self->tokenBuff->lp->lineNum, "");
	}
	if (exprStack_pop(&self->stack).content != t_lParenthessis) {
		syntaxError("Expected left parenthessis ", self->tokenBuff->lp->lineNum, "");
	}
	exprStack_push(&self->stack, nonTerm);
}

void reduceRule(ExprParser *self, ExprToken *TopMostTerminal) {
	Instruction instr;
	InstrParam * p = NULL;
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
					sem_Error("Uninitialized variable", -1);
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
			InstrQueue_insert(self->instructions, instr);
			TopMostTerminal->type = nonterminal;
		} else {
			syntaxError("Reduction of token which was already reducted", -1,
					ExprToken_getName(*TopMostTerminal));
		}
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
		if (findHandle(&self->stack) < 4)
			syntaxError("Expression syntax error - missing operands",
					self->tokenBuff->lp->lineNum, ",");
		if (TopMostTerminal
				!= &(self->stack.StackArray[self->stack.top - 1])) // Check if TopMostTerminal is operator - terminal
			syntaxError("Expression Error - Operator error",
					self->tokenBuff->lp->lineNum, ",");

		Expr_reduceBinaryOperator(self);
		break;
	case t_rParenthessis:
		if (findHandle(&self->stack) < 4)
			syntaxError("Expression syntax error - not enough operands",
					self->tokenBuff->lp->lineNum, ",");
		if (TopMostTerminal->content
				!= self->stack.StackArray[self->stack.top].content) // ')' Must be on top of stack
			syntaxError("Expression syntax error - expected )",
					self->tokenBuff->lp->lineNum, "");

		exprStack_pop(&self->stack); // Pop ')'
		ExprToken last = exprStack_pop(&self->stack);
		if (last.content == t_lParenthessis) {
			last = exprStack_pop(&self->stack);
			if (last.content != t_func) 	// function with no parameter
				syntaxError("Empty brackets are not allowed",
						self->tokenBuff->lp->lineNum, "");
			InstrQueue_insert(self->instructions,
					(Instruction ) { i_call,
									last.id->val.fn->retVal.type, NULL,
									NULL, NULL });
		}

		exprStack_push(&self->stack, last);
		iFunction * func = findFunction(&self->stack);
		if (func)
			reduceParams(self, 1, func->params.Last);
		else
			reduceParenthesis(self);

		break;
	default:
		syntaxError("unknown content of ExprToken", -1, "");
	}

}

void parseWrite(ExprParser * self) {
	Token lastToken = TokenBuff_next(self->tokenBuff);
	iVar * lastSymbol;
	Instruction instr;
	InstrParam * param;

	if (lastToken != t_lParenthessis)
		syntaxError("writeCall expects '(' after write",
				self->tokenBuff->lp->lineNum, getTokenName(lastToken));
	instr.code = i_push;
	instr.a2 = NULL;
	instr.dest = NULL;

	while (true) {
		lastToken = TokenBuff_next(self->tokenBuff);
		param = malloc(sizeof(InstrParam));
		if (lastToken == t_id) {
			lastSymbol = self->tokenBuff->lp->lastSymbol;
			if (lastSymbol->type == iFn)
				syntaxError("Function call cannot be in write call",
						self->tokenBuff->lp->lineNum, getTokenName(lastToken));
			if (!lastSymbol->isInitialized)
				sem_Error("Use of uninitialized variable",
						self->tokenBuff->lp->lineNum);

			param->stackAddr = lastSymbol->stackIndex;
			instr.type = iStackRef;

		} else if (Token_isValue(lastToken)) {
			*param = iVal2InstrP(
					str2iVal(self->tokenBuff->lp->str.buff, lastToken,
							self->tokenBuff->lp->lineNum),
					Token_getType(lastToken));
			instr.type = Token_getType(lastToken);
		} else {
			syntaxError("write call unexpected argument",
					self->tokenBuff->lp->lineNum, getTokenName(lastToken));
		}
		instr.a1 = param;
		InstrQueue_insert(self->instructions, instr);
		if (instr.type == iStackRef) {
			instr.type = lastSymbol->type;
		}
		InstrQueue_insert(self->instructions,
				(Instruction ) { i_write, instr.type,
						NULL, NULL, NULL });
		lastToken = TokenBuff_next(self->tokenBuff);
		if (lastToken == t_comma)
			continue;
		else if (lastToken == t_rParenthessis)
			return;
		else
			syntaxError("write call unexpected argument",
					self->tokenBuff->lp->lineNum, getTokenName(lastToken));
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

tIFJ ExprParser_parse(ExprParser * self) {
	ExprToken *TopMostTerminal;
	ExprToken ExprLastToken;
	ExprToken_Init(&ExprLastToken);

	Token lastToken = TokenBuff_next(self->tokenBuff);
	if (lastToken == t_id && self->tokenBuff->lp->lastSymbol->type == iFn
			&& self->tokenBuff->lp->lastSymbol->val.fn->builtin) {
		Builtins b = self->tokenBuff->lp->lastSymbol->val.fn->builtin;
		switch (b) {
		case b_readLn:
			parseReadLn(self->tokenBuff, self->instructions);
			return iVoid;
		case b_write:
			parseWrite(self);
			return iVoid;
		default:
			break;
		}
	}EXPR_DEBUGING(printf("<Expr Line: %d>\n", tokenBuff->lp->lineNum);)
	tokenToExpr(&ExprLastToken, lastToken, self->tokenBuff->lp); // "copy" content of LastToken to ExprLastToken

	do {
		TopMostTerminal = findTopMostTerminal(&self->stack);
		EXPR_DEBUGING(
				printStack(stack); printf("prtable indexes [%d][%d]\n", TopMostTerminal->content, ExprLastToken.content);)

		switch (prTable[TopMostTerminal->content][ExprLastToken.content]) {
		case shift:		// Vloz zacatek handle
			EXPR_DEBUGING(printf("shift\n");)
			TopMostTerminal->shifted = true;
			exprStack_push(&self->stack, ExprLastToken);
			lastToken = TokenBuff_next(self->tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, self->tokenBuff->lp);
			break;

		case equal:	// push ExprLastToken
			EXPR_DEBUGING(printf("equal\n");)
			exprStack_push(&self->stack, ExprLastToken);
			lastToken = TokenBuff_next(self->tokenBuff);
			tokenToExpr(&ExprLastToken, lastToken, self->tokenBuff->lp);
			break;

		case reduce: // Search for handle on stack and reduce expression
			EXPR_DEBUGING(printf("reduce\n");)
			reduceRule(self, TopMostTerminal);
			TopMostTerminal = findTopMostTerminal(&self->stack);
			TopMostTerminal->shifted = false;
			break;

		case error:
			syntaxError("Expression Error, error state from prTable",
					self->tokenBuff->lp->lineNum, getTokenName(lastToken));
		};
	} while (!(Token_isKeyword(lastToken) || lastToken == t_scolon)); // cann't  require anything else

	while (true) {
		TopMostTerminal = findTopMostTerminal(&self->stack);
		if (self->stack.top == 1
				&& self->stack.StackArray[self->stack.top].type
						== nonterminal) { // only $ and S
			break;
		}
		if (prTable[TopMostTerminal->content][t_eof] == reduce) {
			EXPR_DEBUGING(printf("reduce\n");)
			reduceRule(self, TopMostTerminal);
			TopMostTerminal = findTopMostTerminal(&self->stack);
			TopMostTerminal->shifted = false;
		} else {
			EXPR_DEBUGING(printStack(stack);)
			syntaxError("Expression Error Everything read, can't reduce",
					self->tokenBuff->lp->lineNum,
					ExprToken_getName(ExprLastToken));
		}
	}EXPR_DEBUGING(
			printf("Last stack status\n"); printStack(stack); printf("</Expr lastToken:%d - %s >\n\n", lastToken, getTokenName(lastToken));)

	TokenBuff_pushBack(self->tokenBuff, lastToken);
	return self->stack.StackArray[self->stack.top].datatype;
}

void ExprParser__dell__(ExprParser * self) {
	exprStack__dell__(&self->stack);
}
