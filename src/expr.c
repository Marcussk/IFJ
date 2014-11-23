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
	/*
	 switch(token)
	 {
	 case t_notEqv:
	 return '!';
	 case t_lessOrEqv:
	 return 'L';
	 case t_greaterOrEqv:
	 return 'G';
	 case t_func:
	 return 'f';
	 case t_id:
	 case t_num_int:
	 case t_num_real:
	 case t_str_val:
	 return 'i';
	 default:
	 return token;
	 }
	 */
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
	printf("--Calling expression--\n");

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
	} while (lastToken != t_eof && lastToken != t_end && lastToken != t_scolon
			&& lastToken != t_do && lastToken != t_then);
	if (stack->top->data.content != ExprEndToken->content) {
		printf("Syntax error in expression\n");
	}
	printf("Last token - %d - %s\n--Returning from expression\n\n", lastToken,
			getTokenName(lastToken));

	if (Token_isKeyword(lastToken) || lastToken == t_scolon){
		TokenBuff_pushBack(tokenBuff, lastToken);
	}

	/*
	 printf("-------\n%d\n-------\n", self->lastToken);

	 tokenToExpr(ExprLastToken, self->lastToken); // "copy" content of LastToken to ExprLastToken
	 TopMostTerminal = findTopMostTerminal(stack);

	 exprStack_push(stack, *ExprLastToken);


	 ExprToken *token = ExprTokenInit(token);
	 ExprToken *token2 = ExprTokenInit(token2);
	 token2->content = 'b';
	 token2->type=nonterminal;

	 exprStack_push(stack, *token);
	 exprStack_push(stack, *token2);

	 printStack(stack);

	 TopMostTerminal = findTopMostTerminal(stack);
	 printf("%c, %d, %d", TopMostTerminal->content, TopMostTerminal->type, TopMostTerminal->datatype);
	 */
	//printf("--Exiting expression--\n");
}

/*


 char terminals[] = {['+']=0, ['-']=1, ['*']=2, ['/']=3,
 ['<']=4, ['>']=5, ['L']=6, ['G']=7,
 ['=']=8, ['!']=9, ['(']=10, [')']=11,
 [',']=12, ['$']=13, ['i']=14, ['f']=15};

 char tokenToChar(Token token)
 {
 switch(token)
 {
 case t_notEqv:
 return '!';
 case t_lessOrEqv:
 return 'L';
 case t_greaterOrEqv:
 return 'G';
 case t_func:
 return 'f';
 case t_id:
 case t_num_int:
 case t_num_real:
 case t_str_val:
 return 'i';
 default:
 return token;
 }
 }

 int getTokenType(Token token)
 {
 switch (token)
 {
 case t_true:
 case t_false:
 return boolean;
 case t_num_int:
 case t_num_real:
 case t_str_val:
 return token;
 default:
 return none;
 };
 }

 void tokenToExpr(ExprToken *Expr, Token token)
 {
 Expr->content = tokenToChar(token);
 Expr->type = terminal;
 Expr->datatype = getTokenType(token);
 }

 void expression(SyntaxAnalyzer *self, exprStack *TMPstack, HashTable *TMPSymbolTable)
 {
 ExprInit(TMPstack, TMPSymbolTable);
 self->lastToken = LexParser_gen(self->lp);

 do{
 tokenToExpr(ExprLastToken, self->lastToken); // "copy" content of LastToken to ExprLastToken
 TopMostTerminal = findTopMostTerminal(stack);

 switch(prTable[(int)TopMostTerminal->content][(int)ExprLastToken->content])
 {
 case shift:
 // Vloz zacatek handle
 case equal:
 // push ExprLastToken
 break;
 case reduce:
 // Prohledavej zasobnik, dokud nenarazis na handle, najdi pravidlo
 // a zredukuj
 break;
 case error:
 ;
 // Zahlas syntaktickou chybu
 };

 self->lastToken = LexParser_gen(self->lp);

 } while (stack->top->data.content != ExprEndToken->content || (self->lastToken != t_end || self->lastToken != t_scolon));
 }
 */
