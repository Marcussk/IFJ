#include "syntaxAnalyzer.h"
/*
 #define ldAndCheck(expected, errMsg)\
 self->lastToken = LexParser_gen(self->lp);	\
 if(self->lastToken != expected){\
	 syntaxError(errMsg,self->lp.lineNum);\
 }
 */

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	TokenBuff__init__(&self->tokBuff, lp);
	InstrQueue__init__(&(self->instr));
	self->stackIndexCntr = 0;
}

//one token can be stored in self->lastToken, second token in secondToken when secondToken == Null there is no secondToken
void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self) {
	expression(&self->tokBuff, &self->instr);
	/*
	 if (self->lastToken == t_scolon
	 || (secondToken && *secondToken == t_scolon))
	 return;

	 while ((self->lastToken = LexParser_gen(self->lp)) != t_scolon) {
	 if (self->lastToken == t_end || self->lastToken == t_then
	 || self->lastToken == t_do) { // because expr can end without ; (it ends with end of block or if ...)
	 LexParser_pushBack(self->lp, self->lastToken);
	 return;
	 }
	 }
	 */
	//[TODO]
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
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_colon) {
			syntaxError("expected \":\"\n", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);
		if (!Token_isType(lastToken)) {
			syntaxError("expected type name\n", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}
		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			syntaxError("expected id\n", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}
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
	//[TODO]
}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {	//if
	Token lastToken;
	SyntaxAnalyzer_parseExpr(self);              //COND

	lastToken = TokenBuff_next(&self->tokBuff);			//then
	if (lastToken != t_then) {
		syntaxError("expected then", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	}
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_begin) {
		syntaxError("expected begin for if block", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	}
	SyntaxAnalyzer_parse_block(self);					//STMTLIST

	lastToken = TokenBuff_next(&self->tokBuff);			//else
	if (lastToken != t_else) {
		syntaxError("expected else", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	}
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_begin) {
		syntaxError("expected begin for if else block", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	}
	SyntaxAnalyzer_parse_block(self);					//STMTLIST			
	return;
	//[TODO]
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	Token lastToken;
	SyntaxAnalyzer_parseExpr(self);					//COND
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_do) {							//do
		syntaxError("expected id\n", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	}
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
		if (lastToken != t_id) {						//id
			syntaxError("expected id in argument list", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);			//:
		if (lastToken != t_colon) {
			syntaxError("expected \":\"", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);			//typ
		if (!Token_isType(lastToken)) {
			syntaxError("expected type name", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			if (lastToken == t_rParenthessis) {			// )
				return;
			}
			syntaxError("expected \";\" or \")\" at the end of ",
					self->lp->lineNum, getTokenName(lastToken));
			return;
		} else {											 // ;
			lastToken = TokenBuff_next(&self->tokBuff);
			if (lastToken != t_id) {						//id
				syntaxError("expected id after semicolon", self->lp->lineNum,
						getTokenName(lastToken));
				return;
			}
		}
	}
}

//"function" already found
void SyntaxAnalyzer_parse_func(SyntaxAnalyzer * self) {
	int stackCntrBackup;
	Token lastToken ;
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_id) {
		syntaxError("id of function expected", self->lp->lineNum,
				getTokenName(lastToken));
	}

	LexParser_fnParamsEnter(self->lp);
	SyntaxAnalyzer_parse_paramList(self);

	//[TODO] check and implement forward
	lastToken = TokenBuff_next(&self->tokBuff);       // :
	lastToken = TokenBuff_next(&self->tokBuff);       // typ
	LexParser_fnBodyEnter(self->lp, lastToken);
	lastToken = TokenBuff_next(&self->tokBuff);       // ;
	lastToken = TokenBuff_next(&self->tokBuff);
	switch (lastToken) {
	case t_var:
		stackCntrBackup = self->stackIndexCntr;
		SyntaxAnalyzer_parse_varDeclr(self);
		SyntaxAnalyzer_parse_block(self);
		self->stackIndexCntr = stackCntrBackup;
		break;
	case t_begin:
		SyntaxAnalyzer_parse_block(self);
		break;
	default:
		syntaxError("Expected \"begin\" or \"var\" after function declaration",
				self->lp->lineNum, getTokenName(lastToken));
	}
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
				syntaxError("No input expected", self->lp->lineNum,
						getTokenName(tok));
				return;
			}
		case t_eof:
			syntaxError("Expected \".\"", self->lp->lineNum, getTokenName(tok));
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
}
