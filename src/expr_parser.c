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
	for (i = stack->top; i > 0; i--) {
		if (stack->StackArray[i].content == t_lParenthessis) {
			if (stack->StackArray[i - 1].content == t_func)
				return stack->StackArray[i - 1].id->val.fn;
			else
				return NULL;
		}
	}
	return NULL;
}

tIFJ getResultType(ExprParser * self, tIFJ op1Type, tIFJ op2Type,
		Token operator) {
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
		Error_syntax("Unknown operator", self->tokenBuff->lp->input.line, "");
	}
	Error_sem_Type("Incompatible types", self->tokenBuff->lp->input.line);
	return iUnknown;
}

void reduceParams(ExprParser * self, iFunction * fn) { // ')' already found and popped
	ExprToken result;
	ExprToken last;
	ParamsListItem * paramNode = fn->params.Last;
	int i;
	for (i = 0; i < fn->params.size; i++) {
		last = exprStack_pop(&self->stack);
		if (last.type != nonterminal)
			Error_syntax(
					"Expected nontermial got terminal (meaning bad parameter)",
					self->tokenBuff->lp->input.line, getTokenName(last.content));
		if (last.datatype != paramNode->data->type)
			Error_syntax("Argument of incompatible type",
					self->tokenBuff->lp->input.line, getTokenName(last.content));
		paramNode = paramNode->prev;

		if (i < fn->params.size -1) { // berore first arg there is no ","
			last = exprStack_pop(&self->stack);
			if (last.content != t_comma)
				Error_syntax("Expected , because function has more parameters",
						self->tokenBuff->lp->input.line,
						getTokenName(last.content));
		}
	}
	last = exprStack_pop(&self->stack); // Pop '('
	if (last.content != t_lParenthessis)
		Error_syntax(
				"Expected \"(\" probably received more arguments than expected",
				self->tokenBuff->lp->input.line, getTokenName(last.content));
	last = exprStack_pop(&self->stack); // function Id
	if (last.content != t_func)
		Error_syntax("Function id", self->tokenBuff->lp->input.line,
				getTokenName(last.content));
	if (last.id->val.fn != fn)
		Error_syntax(
				"Over-reduced, attempt to reduce function call resulted in finding different function",
				self->tokenBuff->lp->input.line, getTokenName(last.content));

	ExprToken_Init(&result);
	result.datatype = fn->retVal.type;

	InstrParam * paramsCnt = malloc(sizeof(InstrParam));
	paramsCnt->iInt = fn->params.size;

	Builtins b = fn->builtin;
	if (b) {
		InstrQueue_insert(self->instructions,
				(Instruction ) { b, result.datatype, paramsCnt,
						NULL, NULL });
	} else {
		InstrQueue_insert(self->instructions,
				(Instruction ) { i_call, result.datatype, paramsCnt,
						NULL, fn->bodyInstrIndex });
	}
	result.type = nonterminal;
	exprStack_push(&self->stack, result);
}

void Expr_reduceBinaryOperator(ExprParser * self) {
	ExprToken operand2 = exprStack_pop(&self->stack);
	ExprToken operator = exprStack_pop(&self->stack);
	ExprToken operand1 = exprStack_pop(&self->stack);
	ExprToken result;
	tIFJ instrType;

	if (operand2.type != nonterminal || operand1.type != nonterminal) {
		Error_syntax("Expression Error - Operands error",
				self->tokenBuff->lp->input.line, "nonterminal probably ','");
	}
	ExprToken_Init(&result);

	SemAnalyzer_typeconvert(*(&self->instructions), operand1.datatype,
			operand2.datatype, operator.content);
	result.datatype = getResultType(self, operand1.datatype, operand2.datatype,
			operator.content);

	if (operator.content >= t_less && operator.content <= t_notEqv) {
		instrType = operand1.datatype;
	} else {
		instrType = result.datatype;
	}
	InstrQueue_insert(self->instructions,
			(Instruction ) { Token2Instruction(operator.content),
							instrType, NULL,
							NULL, NULL });
	result.type = nonterminal;
	result.content = t_id;
	exprStack_push(&self->stack, result);
}

void reduceParenthesis(ExprParser * self) {
	ExprToken nonTerm = exprStack_pop(&self->stack);
	if (nonTerm.content != t_id || nonTerm.type != nonterminal) {
		Error_syntax("parenthesis without expr inside",
				self->tokenBuff->lp->input.line, "");
	}
	if (exprStack_pop(&self->stack).content != t_lParenthessis) {
		Error_syntax("Expected left parenthesis ", self->tokenBuff->lp->input.line,
				"");
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
				p = malloc(sizeof(InstrParam));
				p->stackAddr = TopMostTerminal->id->stackIndex;
				if (TopMostTerminal->id->isGlobal)
					instr.type = iStackGRef;
				else
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
			Error_syntax("Reduction of token which was already reducted", -1,
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
			Error_syntax("Expression syntax error - missing operands",
					self->tokenBuff->lp->input.line, ",");
		if (TopMostTerminal != &(self->stack.StackArray[self->stack.top - 1])) // Check if TopMostTerminal is operator - terminal
			Error_syntax("Expression Error - Operator error",
					self->tokenBuff->lp->input.line, ",");

		Expr_reduceBinaryOperator(self);
		break;
	case t_rParenthessis:
		if (findHandle(&self->stack) < 4)
			Error_syntax("Expression syntax error - not enough operands",
					self->tokenBuff->lp->input.line, ",");
		if (TopMostTerminal->content
				!= self->stack.StackArray[self->stack.top].content) // ')' Must be on top of stack
			Error_syntax("Expression syntax error - expected )",
					self->tokenBuff->lp->input.line, "");

		ExprToken last = exprStack_pop(&self->stack);
		if (last.content != t_rParenthessis)
			Error_syntax("Trying to reduce braces but there are not",
					self->tokenBuff->lp->input.line, "");

		iFunction * func = findFunction(&self->stack);
		if (func)
			reduceParams(self, func);
		else
			reduceParenthesis(self);

		break;
	default:
		Error_syntax("Unknown content of ExprToken",
				self->tokenBuff->lp->input.line, "");
	}

}

void parseWrite(ExprParser * self) {
	Token lastToken = TokenBuff_next(self->tokenBuff);
	iVar * lastSymbol;
	Instruction instr;
	InstrParam * param;

	if (lastToken != t_lParenthessis)
		Error_syntax("writeCall expects '(' after write",
				self->tokenBuff->lp->input.line, getTokenName(lastToken));
	instr.code = i_push;
	instr.a2 = NULL;
	instr.dest = NULL;

	while (true) {
		lastToken = TokenBuff_next(self->tokenBuff);
		param = malloc(sizeof(InstrParam));
		if (lastToken == t_id) {
			lastSymbol = self->tokenBuff->lp->lastSymbol;
			if (lastSymbol->type == iFn)
				Error_syntax("Function call can't be in write call",
						self->tokenBuff->lp->input.line, getTokenName(lastToken));

			param->stackAddr = lastSymbol->stackIndex;
			if (lastSymbol->isGlobal)
				instr.type = iStackGRef;
			else
				instr.type = iStackRef;

		} else if (Token_isValue(lastToken)) {
			*param = iVal2InstrP(
					str2iVal(self->tokenBuff->lp->str.buff, lastToken,
							self->tokenBuff->lp->input.line),
					Token_getType(lastToken));
			instr.type = Token_getType(lastToken);
		} else {
			Error_syntax("write call unexpected argument",
					self->tokenBuff->lp->input.line, getTokenName(lastToken));
		}
		instr.a1 = param;
		InstrQueue_insert(self->instructions, instr);
		if (instr.type == iStackRef || instr.type == iStackGRef) {
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
			Error_syntax("write call unexpected argument",
					self->tokenBuff->lp->input.line, getTokenName(lastToken));
	}
}

void parseReadLn(TokenBuff * tokenBuff, InstrQueue * instructions) {
	Token lastToken = TokenBuff_next(tokenBuff);
	if (lastToken != t_lParenthessis)
		Error_syntax("Expected (", tokenBuff->lp->input.line,
				getTokenName(lastToken));

	iVar * lastSymbol;
	InstrParam * param;

	lastToken = TokenBuff_next(tokenBuff);
	if (lastToken == t_id) {
		lastSymbol = tokenBuff->lp->lastSymbol;
		param = malloc(sizeof(InstrParam));
		param->stackAddr = lastSymbol->stackIndex;
	} else
		Error_syntax("Expected identificator", tokenBuff->lp->input.line,
				getTokenName(lastToken));

	InstrQueue_insert(instructions, (Instruction ) { i_readln, lastSymbol->type,
			NULL, NULL, param });

	lastToken = TokenBuff_next(tokenBuff);
	if (lastToken == t_rParenthessis)
		return;
	else
		Error_syntax("write call unexpected argument", tokenBuff->lp->input.line,
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
	}EXPR_DEBUGING(printf("<Expr Line: %d>\n", tokenBuff->lp->input.line);)
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
			Error_syntax("Expression Error, error state from prTable",
					self->tokenBuff->lp->input.line, getTokenName(lastToken));
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
			Error_syntax("Expression Error Everything read, can't reduce",
					self->tokenBuff->lp->input.line,
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
