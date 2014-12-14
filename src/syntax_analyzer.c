#include "syntax_analyzer.h"

#define NEXT_TOK(expected, errMsg)                                  \
 lastToken = TokenBuff_next(&self->tokBuff);                        \
 if(lastToken != expected){                                         \
	 Syntax_err_throw_s(self, lastToken,errMsg);\
 }

#define ASSERT_NEXT_IS_END_OR_SEMICOLON()                                          \
secTok = TokenBuff_next(&self->tokBuff);                                           \
if (secTok == t_end ||secTok == t_scolon ) {                                       \
	TokenBuff_pushBack(&self->tokBuff, secTok);                                    \
}else{                                                                             \
	Syntax_err_throw_s(self, secTok,"Expected end or ; after statement");\
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
			Semantic_err_throw(self->lp, "trying to assign to function");
		}
	}
	tIFJ exprtype = SyntaxAnalyzer_parseExpr(self);
	SemAnalyzer_typeconvert((&self->instr), asigmentTo->type, exprtype, -1);
	SemAnalyzer_checktypes(asigmentTo->type, exprtype, self->tokBuff.lp);
	InstrQueue_insert(&self->instr, (Instruction ) { i_assign, globalOrLocal,
			NULL, (InstrParam*) &(asigmentTo->stackIndex) });
}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	Token lastToken;
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_id && lastToken != t_func) {    // ) - args are empty
		Syntax_err_throw_s(self, lastToken, "Expected variable declarations\n");
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
			Syntax_err_throw_s(self, lastToken, "expected type name\n");
			return;
		}
		self->lp->lastSymbol->type = lastToken;
		self->lp->lastSymbol->isGlobal = self->isInGlobals;

		NEXT_TOK(t_scolon, "expected \";\"\n");

		InstrQueue_insert(&self->instr,
				(Instruction ) { i_push, self->lp->lastSymbol->type,
						NULL, NULL });
		self->lp->lastSymbol->stackIndex = self->stackIndexCntr;
		self->stackIndexCntr++;
	}
}

void SyntaxAnalyzer_parse_statement(SyntaxAnalyzer * self) {
	Token secTok;
	Token lastToken = TokenBuff_next(&self->tokBuff);
	switch (lastToken) {
	case t_if:
		SyntaxAnalyzer_parse_if(self);
		break;
	case t_begin:
		SyntaxAnalyzer_parse_block(self);
		break;
	case t_while:
		SyntaxAnalyzer_parse_while(self);
		break;
	case t_repeat:
		SyntaxAnalyzer_parse_repeat(self);
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
		break;
	default:
		Syntax_err_throw_s(self, lastToken, "Unexpected syntax in statement\n");
		return;
	}
}
bool isStatementEnd(Token t) {
	return t == t_until || t == t_end;
}

void SyntaxAnalyzer_parse_statementlist(SyntaxAnalyzer * self,
bool statRequired) {
	Token lastToken;
	lastToken = TokenBuff_next(&self->tokBuff);
	TokenBuff_pushBack(&self->tokBuff, lastToken);
	if (lastToken != t_end) {
		lastToken = t_scolon;
	} else {
		return;
	}
	while (lastToken == t_scolon) {
		if (statRequired) {
			lastToken = TokenBuff_next(&self->tokBuff);
			if (isStatementEnd(lastToken))
				Syntax_err_throw_s(self, lastToken,
						"one or more statements are required");
			TokenBuff_pushBack(&self->tokBuff, lastToken);
			statRequired = false;
		}
		SyntaxAnalyzer_parse_statement(self);
		lastToken = TokenBuff_next(&self->tokBuff);
	}
	TokenBuff_pushBack(&self->tokBuff, lastToken);
}

//(begin ... end) ("begin" already found)
void SyntaxAnalyzer_parse_block(SyntaxAnalyzer * self) {
	Token lastToken;
	Token secTok = t_empty;

	SyntaxAnalyzer_parse_statementlist(self, false);
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_end)
		Syntax_err_throw_s(self, secTok, "missing end of block");
}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {	//if
	Token lastToken;
	InstrParam * StackAddress = malloc(sizeof(InstrParam));
	if (!StackAddress) {
		Error_memory("Cannot allocate instrParam for Label ");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		Error_memory("Cannot allocate instrParam for Label ");
	}
//COND
	SemAnalyzer_checkcond(SyntaxAnalyzer_parseExpr(self), self->lp);
//jmpz else
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL,
					StackAddress });
//then
	NEXT_TOK(t_then, "expected then")
//begin then block
	NEXT_TOK(t_begin, "expected begin for if block")
//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST
//else
	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_else) {
		TokenBuff_pushBack(&self->tokBuff, lastToken);
		InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL,
				NULL });
		StackAddress->stackAddr = self->instr.actual;
		return;
	} else {  //found else...
		InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL,
						StackAddrend });
	}

//begin else block
	NEXT_TOK(t_begin, "expected begin for if else block")
//else:
	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL,
			NULL });
	StackAddress->iInt = self->instr.actual;
//block
	SyntaxAnalyzer_parse_block(self);					//STMTLIST
// end:

	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL,
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
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_noop, iVoid, NULL, NULL });
	StackAddrbegin->stackAddr = self->instr.actual;
	SemAnalyzer_checkcond(SyntaxAnalyzer_parseExpr(self), self->lp);
//do
	NEXT_TOK(t_do, "expected do");
//begin:

//jmpz end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL,
					StackAddrend });
	lastToken = TokenBuff_next(&self->tokBuff);		//begin
	SyntaxAnalyzer_parse_block(self);				//STMTLIST
//jmp begin
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL,
					StackAddrbegin });
//end
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_noop, iVoid, NULL, NULL });
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
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_noop, iVoid, NULL, NULL });
//begin
	StackAddrbegin->stackAddr = self->instr.actual;

	SyntaxAnalyzer_parse_statementlist(self, true);

	NEXT_TOK(t_until, "expected until");

//Cond
	SemAnalyzer_checkcond(SyntaxAnalyzer_parseExpr(self), self->lp);
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_not, iBool, NULL, NULL });

//jmpz end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL,
					StackAddrend });
//jmp begin
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL,
					StackAddrbegin });

	InstrQueue_insert(&self->instr, (Instruction ) { i_noop, iVoid, NULL,
			NULL });
//end
	StackAddrend->stackAddr = self->instr.actual;

}

void SyntaxAnalyzer_parse_for(SyntaxAnalyzer * self) {
	tIFJ Uboundtype;
	Token lastToken, secTok;
	int step;
	InstrParam * param;
	InstrParam * StackAddrcond = malloc(sizeof(InstrParam));
	if (!StackAddrcond) {
		Error_memory("Cannot allocate instrParam in for");
	}
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));
	if (!StackAddrend) {
		Error_memory("Cannot allocate instrParam in for");
	}
	param = malloc(sizeof(InstrParam));
	if (!param) {
		Error_memory("Can't allocate memory for Instrparam in for");
	}

	//initialize
	NEXT_TOK(t_id, "expected id in for initialization")

	secTok = TokenBuff_next(&self->tokBuff);
	if (secTok == t_asigment) {
		//lastToken = TokenBuff_next(&self->tokBuff);
		TokenBuff_pushBack(&self->tokBuff, secTok);
		TokenBuff_pushBack(&self->tokBuff, lastToken); //t_id
		SyntaxAnalyzer_parseAsigment(self);
	} else {
		Syntax_err_throw_s(self, lastToken,
				"expected assignment in for initialization");
	}
	printf("assignment presiel\n");

	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken != t_to && lastToken != t_downto) {
		Syntax_err_throw_s(self, lastToken,
				"expected to/downto in for initialization");
	} else if (lastToken == t_to) {
		step = +1;
	} else if (lastToken == t_downto) {
		step = -1;
	}
	param->iInt = step;

	//lastToken = TokenBuff_next(&self->tokBuff);
	TokenBuff_pushBack(&self->tokBuff, secTok);
	TokenBuff_pushBack(&self->tokBuff, lastToken);
	Uboundtype = SyntaxAnalyzer_parseExpr(self);
	if (Uboundtype != iInt) {
		Error_unimplemented(
				"Other types than int in condition not implemented\n");
	}
	NEXT_TOK(t_do, "expected do in for Statement block")

	//condition
	// downto: a < b
	// to : a > b
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_noop, iVoid, NULL, NULL });
	StackAddrcond->stackAddr = self->instr.actual;
	//TODO:
	//pop a
	//pop b
	//parse cond

	//jmpz end
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmpz, iVoid, NULL,
					StackAddrend });
	//Code
	NEXT_TOK(t_begin, "expected begin for block in for cycle")
	SyntaxAnalyzer_parse_block(self);
	//increment
	// downto : a--
	// to a+
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_push, iInt, param, NULL });
	InstrQueue_insert(&self->instr, (Instruction ) { i_add, iInt,
			NULL, NULL });
	//jmpcnd
	InstrQueue_insert(&self->instr, (Instruction ) { i_jmp, iVoid, NULL,
					StackAddrcond });
	//End
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_noop, iVoid, NULL, NULL });
	StackAddrend->stackAddr = self->instr.actual;

	//Error_unimplemented("For not implemented yet\n");
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
			Syntax_err_throw_s(self, lastToken, "expected id in argument list");
		}
		NEXT_TOK(t_colon, "expected \":\"")

		lastToken = TokenBuff_next(&self->tokBuff);			//typ
		if (!Token_isType(lastToken)) {
			Syntax_err_throw_s(self, lastToken, "expected type name");
			return;
		}
		self->lp->lastSymbol->type = lastToken;

		lastToken = TokenBuff_next(&self->tokBuff);
		if (lastToken != t_scolon) {
			if (lastToken != t_rParenthessis) {			// )
				Syntax_err_throw_s(self, lastToken,
						"expected \";\" or \")\" at the end of ");
			}
			return;
		} else {											 // ;
			lastToken = TokenBuff_next(&self->tokBuff);
			if (lastToken != t_id) {
				Syntax_err_throw_s(self, lastToken,
						"expected id in argument list after semicolon");\
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
			Syntax_err_throw_s(self, lastToken,
					"names of parameters have to be the same as in forward");
		NEXT_TOK(t_colon, "expected \":\"")
		NEXT_TOK((Token )param->data->type, "type from forward expected")
		if (param->next) {
			NEXT_TOK(t_scolon, "expected \";\" after type (follows by param)")
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
		Syntax_err_throw_s(self, lastToken,
				"Function have to have the same type as it's forward declr.");
	}
	self->lp->lastFunction->retVal.type = lastToken;

	NEXT_TOK(t_scolon, "expected \";\" after function declaration")

	lastToken = TokenBuff_next(&self->tokBuff);
	if (lastToken == t_forward) {
		if (fn->val.fn->forwardFound)
			Syntax_err_throw_s(self, lastToken,
					"Multiple forwards are not allowed");
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
		Syntax_err_throw_s(self, lastToken,
				"Expected \"begin\" or \"var\" after function declaration");
	}
	self->stackIndexCntr = stackCntrBackup;
	LexParser_fnBodyLeave(self->lp);
	self->lp->idMode = lp_searchOnly;
	NEXT_TOK(t_scolon, "expected \";\" after function definition")
	fn->val.fn->bodyFound = true;
	InstrQueue_insert(&self->instr,
			(Instruction ) { i_return, fn->val.fn->retVal.type,
					NULL });
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	Token tok;
	InstrParam * i = malloc(sizeof(InstrParam));
	Instruction jmpToMainBody = { .code = i_jmp, .type = iStackRef, .a1 =
	NULL, .dest = i };

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
				Syntax_err_throw_s(self, tok,
						"Multiple var sections on global level are not allowed");
			SyntaxAnalyzer_parse_varDeclr(self);
			InstrQueue_insert(&self->instr, jmpToMainBody);
			self->isInGlobals = false;
			break;
		case t_begin:
			if (self->mainBodyParsed)
				Syntax_err_throw_s(self, tok,
						"Redefinition of main body is not allowed");
			i->iInt = self->instr.actual + 1; // save jmp after vars allocation to main body
			SyntaxAnalyzer_parse_block(self);
			self->mainBodyParsed = true;
			break;
		case t_func:
			SyntaxAnalyzer_parse_func(self);
			break;
		case t_period:
			self->lp->idMode = lp_ignore;
			tok = TokenBuff_next(&self->tokBuff);
			InstrQueue_insert(&self->instr, (Instruction ) { i_stop, 0, NULL,
					NULL });
			if (tok == t_eof)
				return;
			else {
				Syntax_err_throw_s(self, tok,
						"No input expected after end of the program");
				return;
			}
		case t_eof:
			Syntax_err_throw_s(self, tok,
					"Expected \".\" at the end of program");
			return;
		default:
			Syntax_err_throw_s(self, tok, "Syntax corrupted");
		}
#endif
	}
}

// syntax err throw from syntax analyzer
void Syntax_err_throw_s(SyntaxAnalyzer * self, Token t, char * msg) {
	Err_syntax err;
	err.actualTokenName = getTokenName(t);
	err.column = self->lp->input.column;
	err.filename = self->lp->input.filename;
	err.line = self->lp->input.line;
	err.msg = msg;
	Error_syntax(err);
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
	TokenBuff__dell__(&self->tokBuff);
}
