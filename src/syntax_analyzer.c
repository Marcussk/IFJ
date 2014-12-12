#include "syntax_analyzer.h"

#define NEXT_TOK(expected, errMsg)                                  \
 lastToken = TokenBuff_next(&self->tokBuff);                        \
 if(lastToken != expected){                                         \
	 Error_syntax(errMsg,self->lp->input.line, getTokenName(lastToken));\
 }

#define ASSERT_NEXT_IS_END_OR_SEMICOLON()                                          \
secTok = TokenBuff_next(&self->tokBuff);                                           \
if (secTok == t_end ||secTok == t_scolon ) {                                       \
	TokenBuff_pushBack(&self->tokBuff, secTok);                                    \
}else{                                                                             \
	Error_syntax("Expected end or ; after cmd", self->lp->input.line, getTokenName(secTok));\
}

#define NEW_INSTR(code, types, a1, a2, dest)\
	InstrQueue_insert(&self->instr,	(Instruction ) { code, types, a1, a2, dest});

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	TokenBuff__init__(&self->tokBuff, lp);
	InstrQueue__init__(&(self->instr));
	self->stackIndexCntr = 0;
	self->isInGlobals = true;
	self->mainBodyParsed = false;
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
	tIFJ globalOrLocal;
	if (asigmentTo->isGlobal)
		globalOrLocal = iStackGRef;
	else
		globalOrLocal = iStackRef;

	if (asigmentTo->type == iFn) {
		if (asigmentTo == self->lp->symbolTable->masterItem) {
			asigmentTo = &(asigmentTo->val.fn->retVal);
		} else {
			Error_sem("trying to assign to function", self->lp->input.line);
		}
	}
	tIFJ exprtype = SyntaxAnalyzer_parseExpr(self);
	SemAnalyzer_typeconvert((&self->instr), asigmentTo->type, exprtype, -1);
	SemAnalyzer_checktypes(asigmentTo->type, exprtype,
			self->tokBuff.lp->input.line);
	InstrQueue_insert(&self->instr, (Instruction ) { i_assign, globalOrLocal,
			NULL,
			NULL, (InstrParam*) &(asigmentTo->stackIndex) });
}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	Token lastToken;
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_id && lastToken != t_func) {    // ) - args are empty
		Error_syntax("Expected variable declarations\n", self->lp->input.line,
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
			Error_syntax("expected type name\n", self->lp->input.line,
					getTokenName(lastToken));
			return;
		}
		self->lp->lastSymbol->type = lastToken;
		self->lp->lastSymbol->isGlobal = self->isInGlobals;

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
		case t_repeat:
			SyntaxAnalyzer_parse_repeat(self);
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
			if (secTok == t_scolon) {
				Error_syntax("unexpected \";\" after \";\"", self->lp->input.line,
						";");
			} else if (secTok == t_end)
				Error_syntax("unexpected \";\" before last cmd in block",
						self->lp->input.line, ";");
			else
				TokenBuff_pushBack(&self->tokBuff, secTok);
			continue;
		case t_end:
			return;
		default:
			Error_syntax("Unexpected syntax in code block\n", self->lp->input.line,
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
		Error_memory("Cannot allocate instrParam for Label ");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		Error_memory("Cannot allocate instrParam for Label ");
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
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	Token lastToken;
	InstrParam * StackAddrbegin = malloc(sizeof(InstrParam));
	if (!StackAddrbegin) {
		Error_memory("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		Error_memory("Cannot allocate instrParam for writeFn");
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

}

//"repeat" already found
void SyntaxAnalyzer_parse_repeat(SyntaxAnalyzer * self) { //repeat 
	Token lastToken;
	InstrParam * StackAddrbegin = malloc(sizeof(InstrParam));
	if (!StackAddrbegin) {
		Error_memory("Cannot allocate instrParam for writeFn");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		Error_memory("Cannot allocate instrParam for writeFn");
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

}

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
			Error_syntax("expected id in argument list", self->lp->input.line,
					getTokenName(lastToken));
		}
		NEXT_TOK(t_colon, "expected \":\"")

		lastToken = TokenBuff_next(&self->tokBuff);			//typ
		if (!Token_isType(lastToken)) {
			Error_syntax("expected type name", self->lp->input.line,
					getTokenName(lastToken));
			return;
		}
		self->lp->lastSymbol->type = lastToken;

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			if (lastToken != t_rParenthessis) {			// )
				Error_syntax("expected \";\" or \")\" at the end of ",
						self->lp->input.line, getTokenName(lastToken));
			}
			return;
		} else {											 // ;
			lastToken = TokenBuff_next(&self->tokBuff);
			if (lastToken != t_id) {
				Error_syntax("expected id in argument list after semicolon",
						self->lp->input.line, getTokenName(lastToken));\
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
			Error_syntax("names of parameters have to be same as in forward",
					self->lp->input.line, "id");
		NEXT_TOK(t_colon, "expected \":\"")
		NEXT_TOK((Token )param->data->type, "type from forward expected")
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

	fn = self->lp->lastSymbol;

	if (fn->val.fn->forwardFound) {
		SyntaxAnalyzer_check_ParamsList(self, fn->val.fn->params);
	} else {
		SyntaxAnalyzer_parse_paramList(self);
	}
	NEXT_TOK(t_colon, "expected \":\"")

	lastToken = TokenBuff_next(&self->tokBuff); // typ
	if (fn->val.fn->forwardFound
			&& fn->val.fn->retVal.type != (tIFJ) lastToken) {
		Error_syntax("Function have to have same type as its forward declr.",
				self->lp->input.line, getTokenName(lastToken));
	}
	self->lp->lastFunction->retVal.type = lastToken;

	NEXT_TOK(t_scolon, "expected \";\" after function declaration")

	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_forward) {
		if (fn->val.fn->forwardFound)
			Error_syntax("Multiple forward is not allowed", self->lp->input.line,
					getTokenName(lastToken));
		NEXT_TOK(t_scolon, "expected \";\" after forward")
		self->stackIndexCntr = stackCntrBackup;
		self->lp->idMode = lp_searchOnly;
		fn->val.fn->forwardFound = true;
		return;
	} else {
		TokenBuff_pushBack(&self->tokBuff, lastToken);
	}
	LexParser_fnBodyEnter(self->lp, fn);
	lastToken = TokenBuff_next(&self->tokBuff);
	fn->val.fn->bodyInstrIndex->iInt = self->instr.actual + 1;
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
		Error_syntax("Expected \"begin\" or \"var\" after function declaration",
				self->lp->input.line, getTokenName(lastToken));
	}
	self->stackIndexCntr = stackCntrBackup;
	LexParser_fnBodyLeave(self->lp);
	self->lp->idMode = lp_searchOnly;
	NEXT_TOK(t_scolon, "expected \";\" after function definition")
	fn->val.fn->bodyFound = true;
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
#ifdef TOKENS_ONLY
		self->lp->idMode = lp_ignore;
		printf("line %d: %s\n", self->lp->input.line, getTokenName(tok));
		if (tok == t_eof)
		return;
#else
		switch (tok) {
		case t_var:
			if (!self->isInGlobals)
				Error_syntax(
						"Multiple var section on global level is not allowed",
						self->lp->input.line, getTokenName(tok));
			SyntaxAnalyzer_parse_varDeclr(self);
			InstrQueue_insert(&self->instr, jmpToMainBody);
			self->isInGlobals = false;
			break;
		case t_begin:
			if (self->mainBodyParsed)
				Error_syntax("Redefinition of main body is not allowed",
						self->lp->input.line, getTokenName(tok));
			i->iInt = self->instr.actual + 1; // save jmp after vars allocation to main body
			SyntaxAnalyzer_parse_block(self);
			self->mainBodyParsed = true;
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
				Error_syntax("No input expected after end of program",
						self->lp->input.line, getTokenName(tok));
				return;
			}
		case t_eof:
			Error_syntax("Expected \".\" at the end of program",
					self->lp->input.line, getTokenName(tok));
			return;
		default:
			Error_syntax("syntax corrupted", self->lp->input.line,
					getTokenName(tok));
		}
#endif
	}
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
	TokenBuff__dell__(&self->tokBuff);
}
