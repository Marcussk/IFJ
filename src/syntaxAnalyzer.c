#include "syntaxAnalyzer.h"

#define NEXT_TOK(expected, errMsg)                                  \
 lastToken = TokenBuff_next(&self->tokBuff);                        \
 if(lastToken != expected){                                         \
	 syntaxError(errMsg,self->lp->lineNum, getTokenName(lastToken));\
 }

#define NEW_INSTR(code, types, a1, a2, dest)\
	InstrQueue_insert(&self->instr,	(Instruction ) { code, types, a1, a2, dest});

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	TokenBuff__init__(&self->tokBuff, lp);
	InstrQueue__init__(&(self->instr));
	self->stackIndexCntr = 0;
}

inline void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self) {
	expression(&self->tokBuff, &self->instr);
}

void SyntaxAnalyzer_parseAsigment(SyntaxAnalyzer * self, iVar * variableTo) {
	iVar * asigmentTo = self->lp->lastSymbol;
	SyntaxAnalyzer_parseExpr(self);
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_assign, iStackRef, NULL,	NULL, (InstrParam*) &(asigmentTo->stackIndex) });
	asigmentTo->isInitialied = true;
}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	Token lastToken;
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_rParenthessis) {    // ) - args are empty
		syntaxError("expected var declaration\n", self->lp->lineNum, getTokenName(lastToken));
		return;
	}
	else {
		TokenBuff_pushBack(&self->tokBuff, lastToken);
	}

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
		self->lp->lastSymbol->type = lastToken;

		NEXT_TOK(t_scolon, "expected \";\"\n");

		InstrQueue_insert(&self->instr,
				(Instruction ) { i_push, self->lp->lastSymbol->type, NULL, NULL, NULL });
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
			secTok = TokenBuff_next(&self->tokBuff);
			if (secTok == t_asigment) {
				SyntaxAnalyzer_parseAsigment(self, varForLastId);
			} else {
				TokenBuff_pushBack(&self->tokBuff, secTok);
				TokenBuff_pushBack(&self->tokBuff, lastToken); //t_id
				SyntaxAnalyzer_parseExpr(self);
			}
			lastToken = TokenBuff_next(&self->tokBuff);
			if (lastToken == t_end)
				return;
			else if (lastToken == t_scolon) {
			} else {
				TokenBuff_pushBack(&self->tokBuff, lastToken);
			}
			break;
		case t_scolon:
			continue;
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
	InstrParam * StackAddrelse = malloc(sizeof(InstrParam));
	if(!StackAddrelse) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if(!StackAddrend) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	//COND
	SyntaxAnalyzer_parseExpr(self);              		
	//jmpz else
	InstrQueue_insert(&self->instr,(Instruction){i_jmpz, iVoid, NULL, NULL, StackAddrelse});
	//then
	NEXT_TOK(t_then, "expected then")
	//begin then block
	NEXT_TOK(t_begin, "expected begin for if block")
	//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST
	//jmp end
	InstrQueue_insert(&self->instr,(Instruction){i_jmp, iVoid, NULL, NULL, StackAddrend});
	//else
	NEXT_TOK(t_else, "expected else")
	//begin else block
	NEXT_TOK(t_begin, "expected begin for if else block")
	//else:
	InstrQueue_insert(&self->instr,(Instruction){i_noop, iVoid, NULL, NULL, NULL});
	StackAddrelse->stackAddr = self->instr.index;
	//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST			
	// end:
	InstrQueue_insert(&self->instr,(Instruction){i_noop, iVoid, NULL, NULL, NULL});
	StackAddrend->stackAddr = self->instr.index;
	return;

	//[TODO] instructions
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	Token lastToken;
	InstrParam * StackAddrbegin = malloc(sizeof(InstrParam));
	if(!StackAddrbegin) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if(!StackAddrend) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	//Cond
	InstrQueue_insert(&self->instr,(Instruction){i_noop, iVoid, NULL, NULL, NULL});
	StackAddrbegin->stackAddr = self->instr.index;
	SyntaxAnalyzer_parseExpr(self);					
	//do
	NEXT_TOK(t_do, "expected do")
	//begin:
	
	//jmpz end
	InstrQueue_insert(&self->instr,(Instruction){i_jmpz, iVoid, NULL, NULL, StackAddrend});
	lastToken = TokenBuff_next(&self->tokBuff);		//begin
	SyntaxAnalyzer_parse_block(self);				//STMTLIST
	//jmp begin
	InstrQueue_insert(&self->instr,(Instruction){i_jmp, iVoid, NULL, NULL, StackAddrbegin});
	//end
	InstrQueue_insert(&self->instr,(Instruction){i_noop, iVoid, NULL, NULL, NULL});
	StackAddrend->stackAddr = self->instr.index;
	//[TODO] instructions

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
		lastToken = TokenBuff_next(&self->tokBuff);
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
	iVar * returnVal;
	iVar * fn;
	char * name;
	self->lp->idMode = lp_insertOnly;
	NEXT_TOK(t_id, "id of function expected")
	name = strdup(self->lp->str.buff);
	fn = self->lp->lastSymbol;
	fn->type = iFn;
	fn->val.fn = iFunction__init__();

	SyntaxAnalyzer_parse_paramList(self);

	// [TODO] check and implement forward
	NEXT_TOK(t_colon, "expected \":\"")

	lastToken = TokenBuff_next(&self->tokBuff); // typ

	LexParser_fnBodyEnter(self->lp, lastToken);
	fn->val.fn->retType = lastToken;

	NEXT_TOK(t_scolon, "expected \";\" after function declaration")

	lastToken = TokenBuff_next(&self->tokBuff);
	HashTable_insert(self->lp->symbolTable, name, &returnVal);
	returnVal->stackIndex =-1;
	returnVal->type = fn->val.fn->retType;
	switch (lastToken) {
	case t_var:
		SyntaxAnalyzer_parse_varDeclr(self);
		NEXT_TOK(t_begin, "expected begin (function body)")
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
	LexParser_fnBodyLeave(self->lp);
	self->lp->idMode = lp_searchOnly;
	free(name);
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	Token tok;
	while (true) {
		tok = TokenBuff_next(&self->tokBuff);
		/*
		 self->lp->idMode = lp_debug;
		 printf("line %d: %s\n", self->lp->lineNum, getTokenName(tok));
		 if (tok == t_eof) {
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
			tok = TokenBuff_next(&self->tokBuff);
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
