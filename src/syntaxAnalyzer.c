#include "syntaxAnalyzer.h"
/*
 #define ldAndCheck(expected, errMsg)\
 self->lastToken = LexParser_gen(self->lp);	\
 if(self->lastToken != expected){\
	 syntaxError(errMsg,self->lp.lineNum);\
 }
 */

void printToken(LexParser * p, Token t) {
	char * str;
	switch (t) {
	case t_str_val:
		printf("Line %d:<str:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_id:
		//if (HashTable_insert(ht, p->str.buff) == 2) {
		//	/*printf("Line %d:<id:'%s', already in table >\n", p->lineNum,
		//	 p->str.buff);*/
		//}
		printf("Line %d:<id:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_num_int:
		printf("Line %d:<int:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_num_real:
		printf("Line %d:<real:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_invalid:
		printf("Line %d:<invalid token str: \"%s\">\n", p->lineNum,
				p->str.buff);
		break;
	case t_eof:
		printf("Line %d:<EOF>\n", p->lineNum);
		break;
	default:
		str = getTokenName(t);
		if (str) {
			printf("Line %d: [%s]\n", p->lineNum, str);
		} else {
			printf("Line %d: <tokenWithoutStr: %d>\n", p->lineNum, t);
		}
	}
}

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp) {
	self->lp = lp;
	self->lastToken = t_empty;
}

//one token can be stored in self->lastToken, second token in secondToken when secondToken == Null there is no secondToken
void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * self, Token * secondToken) {
	if (self->lastToken == t_scolon
			|| (secondToken && *secondToken == t_scolon))
		return;

	while ((self->lastToken = LexParser_gen(self->lp)) != t_scolon) {
		if (self->lastToken == t_end || self->lastToken == t_then) { // because expr can ends without ; (it ends with end of block or if ...)
			LexParser_pushBack(self->lp, self->lastToken);
			return;
		}
	}
	//[TODO]
}

void SyntaxAnalyzer_parseAsigment(SyntaxAnalyzer * self, iVar * variableTo) {
	SyntaxAnalyzer_parseExpr(self, NULL);
	//[TODO]
}

// t_var already found
void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	self->lp->idMode = lp_insertOnly;
	// read all variable declarations
	while (true) {
		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_id) {
			self->lp->idMode = lp_searchOnly;
			return;
		}

		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_colon) {
			syntaxError("expected \":\"\n", self->lp->lineNum,
					getTokenName(self->lastToken));
			return;
		}

		self->lastToken = LexParser_gen(self->lp);
		if (!Token_isType(self->lastToken)) {
			syntaxError("expected type name\n", self->lp->lineNum,
					getTokenName(self->lastToken));
			return;
		}
		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_scolon) {
			syntaxError("expected id\n", self->lp->lineNum,
					getTokenName(self->lastToken));
			return;
		}
	}
	//[TODO]
}

//(begin ... end) ("begin" already found)
void SyntaxAnalyzer_parse_block(SyntaxAnalyzer * self) {
	Token secTok = t_empty;
	iVar * varForLastId = NULL;
	while (self->lastToken != t_end) {
		self->lastToken = LexParser_gen(self->lp);
		switch (self->lastToken) {
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
				SyntaxAnalyzer_parseExpr(self, &secTok);
			}
			break;
		default:
			syntaxError("Unexpected syntax in code block\n", self->lp->lineNum,
					getTokenName(self->lastToken));
		}
	}
	//[TODO]

}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {	//if
	SyntaxAnalyzer_parseExpr(self, NULL);              //COND

	self->lastToken = LexParser_gen(self->lp);			//then
	if (self->lastToken != t_then) {
		syntaxError("expected then", self->lp->lineNum, getTokenName(self->lastToken));
		return;
	}
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_begin) {
		syntaxError("expected begin for if block", self->lp->lineNum, getTokenName(self->lastToken));
		return;
	}
	SyntaxAnalyzer_parse_block(self);					//STMTLIST

	self->lastToken = LexParser_gen(self->lp);			//else
	if (self->lastToken != t_else) {
		syntaxError("expected else", self->lp->lineNum, getTokenName(self->lastToken));
		return;
	}
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_begin) {
		syntaxError("expected begin for if else block", self->lp->lineNum, getTokenName(self->lastToken));
		return;
	}
	SyntaxAnalyzer_parse_block(self);					//STMTLIST			
	return;
	//[TODO]
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {   //while
	SyntaxAnalyzer_parseExpr(self, NULL);					//COND
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_do) {							//do
		syntaxError("expected id\n", self->lp->lineNum, getTokenName(self->lastToken));
		return;
	}
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
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken == t_rParenthessis)     // ) - args are empty
		return;
	else
		LexParser_pushBack(self->lp, self->lastToken);

	while (self->lastToken != t_rParenthessis) {
		SyntaxAnalyzer_parseExpr(self, NULL);
		self->lastToken = LexParser_gen(self->lp);
			if (self->lastToken == t_rParenthessis)     // ) - args are empty
				return;
			else if(self->lastToken == t_comma)
				continue;
			else
				syntaxError("expected \",\" or \")\"", self->lp->lineNum, getTokenName(self->lastToken));
	}
}

// ( - already found ;( params are in function declarations)
/*
 During definition of user function
 f(params) -> f(id : typ; id : typ)
 */
void SyntaxAnalyzer_parse_paramList(SyntaxAnalyzer * self) {
	LexParser_fnParamsEnter(self->lp);
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken == t_rParenthessis) {            // ) - params are empty
		return;
	}
	while (true) {
		if (self->lastToken != t_id) {						//id
			syntaxError("expected id in argument list", self->lp->lineNum, getTokenName(self->lastToken));
			return;
		}

		self->lastToken = LexParser_gen(self->lp);			//:
		if (self->lastToken != t_colon) {
			syntaxError("expected \":\"", self->lp->lineNum, getTokenName(self->lastToken));
			return;
		}

		self->lastToken = LexParser_gen(self->lp);			//typ
		if (!Token_isType(self->lastToken)) {
			syntaxError("expected type name", self->lp->lineNum, getTokenName(self->lastToken));
			return;
		}

		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_scolon) {
			if (self->lastToken == t_rParenthessis) {			// )
				return;
			}
			syntaxError("expected \";\" or \")\" at the end of ",
					self->lp->lineNum, getTokenName(self->lastToken));
			return;
		} else {											 // ;
			self->lastToken = LexParser_gen(self->lp);
			if (self->lastToken != t_id) {						//id
				syntaxError("expected id after semicolon", self->lp->lineNum, getTokenName(self->lastToken));
				return;
			}
		}
	}
}

//"function" already found
void SyntaxAnalyzer_parse_func(SyntaxAnalyzer * self) {
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_id) {
		syntaxError("id of function expected", self->lp->lineNum, getTokenName(self->lastToken));
	}

	LexParser_fnParamsEnter(self->lp);
	SyntaxAnalyzer_parse_paramList(self);

	//[TODO] check and implement foward
	self->lastToken = LexParser_gen(self->lp); //:
	self->lastToken = LexParser_gen(self->lp); // typ
	LexParser_fnBodyEnter(self->lp, self->lastToken);
	self->lastToken = LexParser_gen(self->lp); // ;
	self->lastToken = LexParser_gen(self->lp);
	switch (self->lastToken) {
	case t_var:
		SyntaxAnalyzer_parse_varDeclr(self);
		SyntaxAnalyzer_parse_block(self);
		break;
	case t_begin:
		SyntaxAnalyzer_parse_block(self);
		break;
	default:
		syntaxError(
				"Expected \"begin\" or \"var\" after function declaration",
				self->lp->lineNum, getTokenName(self->lastToken));
	}
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	Token tok;
	while (true) {
		//get new token or reuse last if is
		if (self->lastToken == t_empty)
			tok = LexParser_gen(self->lp);
		else {
			tok = self->lastToken;
			self->lastToken = t_empty;
		}
		//printToken(self->lp, tok);
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
				syntaxError("No input expected", self->lp->lineNum, getTokenName(tok));
				return;
			}
		case t_eof:
			syntaxError("Expected \".\"", self->lp->lineNum, getTokenName(tok));
			return;
		default:
			syntaxError("syntax corrupted", self->lp->lineNum, getTokenName(tok));
		}
	}
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
}
