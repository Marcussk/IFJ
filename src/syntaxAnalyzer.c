#include "syntaxAnalyzer.h"

#define NEXT_TOK(expected, errMsg)                                 \
 lastToken = TokenBuff_next(&self->tokBuff);                         \
 if(lastToken != expected){                                          \
	 syntaxError(errMsg,self->lp->lineNum, getTokenName(lastToken));  \
 }

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	TokenBuff__init__(&self->tokBuff, lp);
	InstrQueue__init__(&(self->instr));
	self->stackIndexCntr = 0;
}

void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self) {
	expression(&self->tokBuff, &self->instr);
}

void SyntaxAnalyzer_parseAsigment(SyntaxAnalyzer * self, iVar * variableTo) {
	iVar * asigmentTo = self->lp->lastSymbol;
	SyntaxAnalyzer_parseExpr(self);
	InstrQueue_insert(&self->instr, (Instruction ) { i_assign, iStackRef, NULL,
			NULL, (InstrParam*) &(asigmentTo->stackIndex) });
	asigmentTo->isInitialied = true;

}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	Token lastToken;
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	while (true) {
		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_id) {
			self->lp->idMode = lp_searchOnly;
			TokenBuff_pushBack(&self->tokBuff, lastToken);
			return;
		}
		NEXT_TOK(t_colon, "expected \":\"\n");

		lastToken = TokenBuff_next(&self->tokBuff);
		if (!Token_isType(lastToken)) {
			syntaxError("expected type name\n", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		NEXT_TOK(t_scolon, "expected id\n");

		InstrQueue_insert(&self->instr,
				(Instruction ) { i_push, self->lp->lastSymbol->type,
						NULL, NULL, NULL });
		self->lp->lastSymbol->stackIndex = self->stackIndexCntr;
		self->stackIndexCntr++;
	}
}

//(begin ... end) ("begin" already found)
void SyntaxAnalyzer_parse_block(SyntaxAnalyzer * self) {
	Token lastToken;
	Token secTok = t_empty;
	iVar * varForLastId = NULL;
	lastToken = TokenBuff_next(&self->tokBuff);

	if (lastToken == t_end)
		return;
	else
		TokenBuff_pushBack(&self->tokBuff, lastToken);

	while (1) {
		lastToken = TokenBuff_next(&self->tokBuff);
		switch (lastToken) {
		case t_if:
			SyntaxAnalyzer_parse_if(self);
			break;
		case t_while:
			SyntaxAnalyzer_parse_while(self);
			break;
		case t_id:
			varForLastId = self->lp->lastSymbol;
			secTok = LexParser_gen(self->lp);
			if (secTok == t_asigment) {
				SyntaxAnalyzer_parseAsigment(self, varForLastId);
			} else {
				TokenBuff_pushBack(&self->tokBuff, secTok);
				TokenBuff_pushBack(&self->tokBuff, t_id);
				SyntaxAnalyzer_parseExpr(self);
			}
			break;
		case t_end:
			return;
		default:
			syntaxError("Unexpected syntax in code block\n", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}
	}
}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {	//if
	Token lastToken;
	SyntaxAnalyzer_parseExpr(self);              //COND

	NEXT_TOK(t_then, "expected then")
	NEXT_TOK(t_begin, "expected begin for if block")
	SyntaxAnalyzer_parse_block(self);					//STMTLIST
	NEXT_TOK(t_else, "expected else")
	NEXT_TOK(t_begin, "expected begin for if else block")

	SyntaxAnalyzer_parse_block(self);					//STMTLIST			
	return;
	//[TODO]
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	Token lastToken;
	SyntaxAnalyzer_parseExpr(self);					//COND

	NEXT_TOK(t_do, "expected do")

	lastToken = TokenBuff_next(&self->tokBuff);
	SyntaxAnalyzer_parse_block(self);						//STMTLIST
	//[TODO]
}

// "("  already found (args are in function call)
/*
 paramaters during function call
 id := f(args)
 args -> seznam termu
 */
void SyntaxAnalyzer_parse_argList(SyntaxAnalyzer * self) {
	Token lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_rParenthessis)     // ) - args are empty
		return;
	else
		TokenBuff_pushBack(&self->tokBuff, lastToken);

	while (lastToken != t_rParenthessis) {
		SyntaxAnalyzer_parseExpr(self);
		lastToken = LexParser_gen(self->lp);
		if (lastToken == t_rParenthessis)     // ) - end of args
			return;
		else if (lastToken == t_comma)
			continue;
		else
			syntaxError("expected \",\" or \")\"", self->lp->lineNum,
					getTokenName(lastToken));
	}
}

// "(" - already found ;( params are in function declarations)
/*
 During definition of user function
 f(params) -> f(id : typ; id : typ)
 */
void SyntaxAnalyzer_parse_paramList(SyntaxAnalyzer * self) {
	Token lastToken;
	LexParser_fnParamsEnter(self->lp);
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_rParenthessis) {            // ) - params are empty
		return;
	}
	while (true) {
		NEXT_TOK(t_id, "expected id in argument list")
		NEXT_TOK(t_colon, "expected \":\"")

		lastToken = TokenBuff_next(&self->tokBuff);			//typ
		if (!Token_isType(lastToken)) {
			syntaxError("expected type name", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			if (lastToken != t_rParenthessis) {			// )
				syntaxError("expected \";\" or \")\" at the end of ",
						self->lp->lineNum, getTokenName(lastToken));
			}
			return;
		} else {											 // ;
			lastToken = TokenBuff_next(&self->tokBuff);
			NEXT_TOK(t_id, "expected id in argument list after semicolon")
		}
	}
}

//"function" already found
void SyntaxAnalyzer_parse_func(SyntaxAnalyzer * self) {
	int stackCntrBackup = self->stackIndexCntr;
	Token lastToken;
	iVar * fn;

	NEXT_TOK(t_id, "id of function expected")
	fn = self->lp->lastSymbol;
	fn->type = iFn;
	fn->val.fn = iFunction__init__();

	LexParser_fnParamsEnter(self->lp);
	SyntaxAnalyzer_parse_paramList(self);

	//[TODO] check and implement forward
	NEXT_TOK(t_colon, "expected \":\"")

	lastToken = TokenBuff_next(&self->tokBuff);       // typ

	LexParser_fnBodyEnter(self->lp, lastToken);
	fn->val.fn->retType = lastToken;

	NEXT_TOK(t_scolon, "expected \";\" after function declaration")

	lastToken = TokenBuff_next(&self->tokBuff);
	switch (lastToken) {
	case t_var:
		SyntaxAnalyzer_parse_varDeclr(self);
		SyntaxAnalyzer_parse_block(self);

		break;
	case t_begin:
		SyntaxAnalyzer_parse_block(self);
		break;
	default:
		syntaxError("Expected \"begin\" or \"var\" after function declaration",
				self->lp->lineNum, getTokenName(lastToken));
	}
	fn->isInitialied = true;
	self->stackIndexCntr = stackCntrBackup;
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	Token tok;
	while (true) {
		tok = TokenBuff_next(&self->tokBuff);
		/*
		 printf("%s \n", getTokenName(tok));
		 if(tok == t_eof){
		 return;
		 }*/

		switch (tok) {
		case t_var:
			SyntaxAnalyzer_parse_varDeclr(self);
			break;
		case t_begin:
			SyntaxAnalyzer_parse_block(self);
			break;
		case t_func:
			SyntaxAnalyzer_parse_func(self);
			break;
		case t_period:
			tok = LexParser_gen(self->lp);
			if (tok == t_eof)
				return;
			else {
				syntaxError("No input expected after end of program",
						self->lp->lineNum, getTokenName(tok));
				return;
			}
		case t_eof:
			syntaxError("Expected \".\" on the end of program",
					self->lp->lineNum, getTokenName(tok));
			return;
		case t_end:
			return;
		default:
			syntaxError("syntax corrupted", self->lp->lineNum,
					getTokenName(tok));
		}
	}
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
	TokenBuff__dell__(&self->tokBuff);
}
