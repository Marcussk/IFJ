#include "syntax_analyzer.h"

#define NEXT_TOK(expected, errMsg)                                  \
 lastToken = TokenBuff_next(&self->tokBuff);                        \
 if(lastToken != expected){                                         \
	 syntaxError(errMsg,self->lp->lineNum, getTokenName(lastToken));\
 }

#define ASSERT_NEXT_IS_END_OR_SEMICOLON()                                          \
secTok = TokenBuff_next(&self->tokBuff);                                           \
if (secTok == t_end ||secTok == t_scolon ) {                                       \
	TokenBuff_pushBack(&self->tokBuff, secTok);                                    \
}else{                                                                             \
	syntaxError("Expected end after cmd", self->lp->lineNum, getTokenName(secTok));\
}

#define NEW_INSTR(code, types, a1, a2, dest)\
	InstrQueue_insert(&self->instr,	(Instruction ) { code, types, a1, a2, dest});

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	TokenBuff__init__(&self->tokBuff, lp);
	InstrQueue__init__(&(self->instr));
	self->stackIndexCntr = 0;
}

tIFJ SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self) {
	ExprParser expr;
	tIFJ exprType;
	ExprParser__init__(&expr, &self->tokBuff, &self->instr);
	exprType = ExprParser_parse(&expr);
	ExprParser__dell__(&expr);
	return exprType;
}

void SyntaxAnalyzer_parseAsigment(SyntaxAnalyzer * self) {
	iVar * asigmentTo = self->lp->lastSymbol;
	if (asigmentTo->type == iFn) {
		if (asigmentTo == self->lp->symbolTable->masterItem) {
			asigmentTo = &(asigmentTo->val.fn->retVal);
		} else {
			sem_Error("trying to assign to function", self->lp->lineNum);
		}
	}
	tIFJ exprtype = SyntaxAnalyzer_parseExpr(self);
	SemAnalyzer_typeconvert((&self->instr), asigmentTo->type, exprtype, -1);
	SemAnalyzer_checktypes(asigmentTo->type, exprtype,
			self->tokBuff.lp->lineNum);
	InstrQueue_insert(&self->instr, (Instruction ) { i_assign, iStackRef, NULL,
			NULL, (InstrParam*) &(asigmentTo->stackIndex) });
	asigmentTo->isInitialized = true;
}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	Token lastToken;
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_id && lastToken != t_func) {    // ) - args are empty
		syntaxError("Expected variable declarations\n", self->lp->lineNum,
				getTokenName(lastToken));
		return;
	} else {
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
			ASSERT_NEXT_IS_END_OR_SEMICOLON()
			break;
		case t_begin:
			SyntaxAnalyzer_parse_block(self);
			ASSERT_NEXT_IS_END_OR_SEMICOLON()
			break;
		case t_while:
			SyntaxAnalyzer_parse_while(self);
			ASSERT_NEXT_IS_END_OR_SEMICOLON()
			break;
		/////
		case t_repeat:
			SyntaxAnalyzer_parse_reapat(self);
			ASSERT_NEXT_IS_END_OR_SEMICOLON()
			break;
		case t_id:
			secTok = TokenBuff_next(&self->tokBuff);
			if (secTok == t_asigment) {
				SyntaxAnalyzer_parseAsigment(self);
			} else {
				TokenBuff_pushBack(&self->tokBuff, secTok);
				TokenBuff_pushBack(&self->tokBuff, lastToken); //t_id
				SyntaxAnalyzer_parseExpr(self);
			}
			ASSERT_NEXT_IS_END_OR_SEMICOLON()
			break;
		case t_scolon:
			secTok = TokenBuff_next(&self->tokBuff);
			if(secTok == t_scolon){
				syntaxError("unexpected \";\" after \";\"",
										self->lp->lineNum, ";");
			}else if (secTok == t_end)
				syntaxError("unexpected \";\" before last cmd in block",
						self->lp->lineNum, ";");
			else
				TokenBuff_pushBack(&self->tokBuff, secTok);
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
	tIFJ condtype;
	InstrParam * StackAddress = malloc(sizeof(InstrParam));
	if (!StackAddress) {
		memoryError("Cannot allocate instrParam for Label ");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		memoryError("Cannot allocate instrParam for Label ");
	}
	//COND
	condtype = SyntaxAnalyzer_parseExpr(self);
	SemAnalyzer_checkcond(condtype);
	//jmpz else
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL, NULL,
					StackAddress });
	//then
	NEXT_TOK(t_then, "expected then")
	//begin then block
	NEXT_TOK(t_begin, "expected begin for if block")
	//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST
	//jmp end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrend });
	//else
	NEXT_TOK(t_else, "expected else")
	//begin else block
	NEXT_TOK(t_begin, "expected begin for if else block")
	//else:
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	StackAddress->iInt = self->instr.actual;
	//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST			
	// end:
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	StackAddrend->stackAddr = self->instr.actual;
	return;

	//[TODO] instructions
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	Token lastToken;
	InstrParam * StackAddrbegin = malloc(sizeof(InstrParam));
	if (!StackAddrbegin) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	//Cond
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	StackAddrbegin->stackAddr = self->instr.actual;
	SyntaxAnalyzer_parseExpr(self);
	//do
	NEXT_TOK(t_do, "expected do");
	//begin:

	//jmpz end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL, NULL,
					StackAddrend });
	lastToken = TokenBuff_next(&self->tokBuff);		//begin
	SyntaxAnalyzer_parse_block(self);				//STMTLIST
	//jmp begin
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrbegin });
	//end
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	StackAddrend->stackAddr = self->instr.actual;
	//[TODO] instructions

}

//////////////////
//"repeat" already found
void SyntaxAnalyzer_parse_reapat(SyntaxAnalyzer * self) { //repeat 
	Token lastToken;
	InstrParam * StackAddrbegin = malloc(sizeof(InstrParam));
	if (!StackAddrbegin) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		memoryError("Cannot allocate instrParam for writeFn");
	}
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	//begin
	StackAddrbegin->stackAddr = self->instr.actual;

	lastToken = TokenBuff_next(&self->tokBuff);		//begin
	SyntaxAnalyzer_parse_block(self);				//STMTLIST

	//until 
	NEXT_TOK(t_until, "expected until");
	
	//Cond
	SyntaxAnalyzer_parseExpr(self);

	//jmpz end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL, NULL,
					StackAddrend });			
	//jmp begin
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrbegin });

	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL, NULL,
			NULL });
	//end
	StackAddrend->stackAddr = self->instr.actual;

	//free(StackAddrend);
	//free(StackAddrbegin);	
}

//////////////////

/*
 * ( params are in function declarations)
 During definition of user function
 f(params) -> f(id : typ; id : typ)
 */
void SyntaxAnalyzer_parse_paramList(SyntaxAnalyzer * self) {
	Token lastToken;

	NEXT_TOK(t_lParenthessis, "expected \"(\"")
	LexParser_fnParamsEnter(self->lp);
	while (true) {
		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken == t_rParenthessis) {            // ) - params are empty
			return;
		} else if (lastToken != t_id) {
			syntaxError("expected id in argument list", self->lp->lineNum,
					getTokenName(lastToken));
		}
		self->lp->lastSymbol->isInitialized = true;
		NEXT_TOK(t_colon, "expected \":\"")

		lastToken = TokenBuff_next(&self->tokBuff);			//typ
		if (!Token_isType(lastToken)) {
			syntaxError("expected type name", self->lp->lineNum,
					getTokenName(lastToken));
			return;
		}
		self->lp->lastSymbol->type = lastToken;

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			if (lastToken != t_rParenthessis) {			// )
				syntaxError("expected \";\" or \")\" at the end of ",
						self->lp->lineNum, getTokenName(lastToken));
			}
			return;
		} else {											 // ;
			lastToken = TokenBuff_next(&self->tokBuff);
			if (lastToken != t_id) {
				syntaxError("expected id in argument list after semicolon",
						self->lp->lineNum, getTokenName(lastToken));\
			} else {
				TokenBuff_pushBack(&self->tokBuff, lastToken);
			}
		}
	}
}

// "function" and id already found , only chceck params, params types and return type
void SyntaxAnalyzer_check_ParamsList(SyntaxAnalyzer * self,
		ParamsList paramsList) {
	ParamsListItem * param = paramsList.First;
	Token lastToken;
	self->lp->idMode = lp_ignore;
	NEXT_TOK(t_lParenthessis, "expected \"(\"")
	while (param) {
		NEXT_TOK(t_id, "id of param from forward expected")
		if (strcmp(param->name, self->lp->str.buff))
			syntaxError("names of parameters have to be same as in forward",
					self->lp->lineNum, "id");
		NEXT_TOK(t_colon, "expected \":\"")
		NEXT_TOK((tIFJ )param->data->type, "type from forward expected")
		if (param->next) {
			NEXT_TOK(t_scolon,
					"expected \";\" after type (and then next param)")
		}
		param = param->next;
	}
	NEXT_TOK(t_rParenthessis, "expected \")\"")
}

//"function" already found
void SyntaxAnalyzer_parse_func(SyntaxAnalyzer * self) {
	int stackCntrBackup = self->stackIndexCntr;
	self->stackIndexCntr = 4; // because 0 is return val, ...
	Token lastToken;
	iVar * fn;
	self->lp->idMode = lp_fnSearch;
	NEXT_TOK(t_id, "id of function expected")
	bool haveForward;

	fn = self->lp->lastSymbol;
	haveForward = fn->val.fn->retVal.type != iUnknown;

	if (haveForward) {
		SyntaxAnalyzer_check_ParamsList(self, fn->val.fn->params);
	} else {
		SyntaxAnalyzer_parse_paramList(self);
	}
	NEXT_TOK(t_colon, "expected \":\"")

	lastToken = TokenBuff_next(&self->tokBuff); // typ
	if (haveForward && fn->val.fn->retVal.type != (tIFJ) lastToken) {
		syntaxError("Function have to have same type as its forward declr.",
				self->lp->lineNum, getTokenName(lastToken));
	}
	self->lp->lastFunction->retVal.type = lastToken;

	NEXT_TOK(t_scolon, "expected \";\" after function declaration")

	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_forward) {
		NEXT_TOK(t_scolon, "expected \";\" after forward")
		self->stackIndexCntr = stackCntrBackup;
		self->lp->idMode = lp_searchOnly;
		return;
	} else {
		TokenBuff_pushBack(&self->tokBuff, lastToken);
	}
	LexParser_fnBodyEnter(self->lp, fn);
	lastToken = TokenBuff_next(&self->tokBuff);
	fn->val.fn->bodyInstrIndex = self->instr.actual + 1;
	switch (lastToken) {
	case t_var:
		SyntaxAnalyzer_parse_varDeclr(self);
		NEXT_TOK(t_begin, "expected begin (function body)")
		iFunction_buildParamIndexes(fn->val.fn);
		SyntaxAnalyzer_parse_block(self);
		break;
	case t_begin:
		self->lp->idMode = lp_searchOnly;
		SyntaxAnalyzer_parse_block(self);
		break;
	default:
		syntaxError("Expected \"begin\" or \"var\" after function declaration",
				self->lp->lineNum, getTokenName(lastToken));
	}
	fn->isInitialized = true;
	self->stackIndexCntr = stackCntrBackup;
	LexParser_fnBodyLeave(self->lp);
	self->lp->idMode = lp_searchOnly;
	NEXT_TOK(t_scolon, "expected \";\" after function definition")
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_return, fn->val.fn->retVal.type, NULL,
					NULL, NULL });
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	Token tok;
	InstrParam * i = malloc(sizeof(InstrParam));
	Instruction jmpToMainBody = { .code = i_jmp, .type = iStackRef, .a1 =
	NULL, .a2 = NULL, .dest = i };

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
			InstrQueue_insert(&self->instr, jmpToMainBody);

			break;
		case t_begin:
			i->iInt = self->instr.actual + 1;
			SyntaxAnalyzer_parse_block(self);
			break;
		case t_func:
			SyntaxAnalyzer_parse_func(self);
			break;
		case t_period:
			tok = TokenBuff_next(&self->tokBuff);
			InstrQueue_insert(&self->instr, (Instruction ) { i_stop, 0, NULL,
					NULL, NULL });
			if (tok == t_eof)
				return;
			else {
				syntaxError("No input expected after end of program",
						self->lp->lineNum, getTokenName(tok));
				return;
			}
		case t_eof:
			syntaxError("Expected \".\" at the end of program",
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
