#include "expr.h"

IMPLEMENT_STACK(expr, ExprToken);

HashTable *SymbolTable;
exprStack *stack;
ExprToken *ExprEndToken;
ExprToken *ExprLastToken;
ExprToken *TopMostTerminal;

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

void ExprTokenInit(ExprToken *token)
{
	token->content = '$';
	token->type = terminal;
	token->datatype = none;
}

void tokenToExpr(ExprToken *Expr, Token token)
{
	Expr->content = tokenToChar(token);
	Expr->type = terminal;
	Expr->datatype = getTokenType(token);
}

ExprToken *findTopMostTerminal(exprStack *s)
{
	exprStackNodeT * itr = s->top;
	while (itr != NULL){
		if (itr->data.type == terminal)
			return &(itr->data);
		itr = itr->next;
	}
	return NULL;
}

void ExprInit(exprStack *TMPstack, HashTable *TMPSymbolTable)
{
	SymbolTable = TMPSymbolTable;
	stack = TMPstack;
	ExprTokenInit(ExprEndToken);
	ExprTokenInit(ExprLastToken);
	exprStack_push(stack, *ExprEndToken);
}

void expression(SyntaxAnalyzer *self, exprStack *TMPstack, HashTable *TMPSymbolTable)
{
	ExprInit(TMPstack, TMPSymbolTable);
	self->lastToken = LexParser_gen(self->lp);

	do{
		tokenToExpr(ExprLastToken, self->lastToken); // "copy" content of LastToken to ExprLastToken
		TopMostTerminal = findTopMostTerminal(stack);

		switch(prTable[TopMostTerminal->content][ExprLastToken->content])
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

	} while (stack->top != ExprEndToken->content || (self->lastToken != t_end || self->lastToken != t_scolon));
}
