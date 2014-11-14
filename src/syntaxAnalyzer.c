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
		str = getTokenStr(t);
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
	printf("not implemented/SyntaxAnalyzer_parseExpr\n");
	if (self->lastToken == t_scolon
			|| (secondToken && *secondToken == t_scolon))
		return;
	printf("<expr>\n");
	self->lastToken = LexParser_gen(self->lp);
	printf("1\n");
	self->lastToken = LexParser_gen(self->lp);
	printf("1\n");
	while ((self->lastToken = LexParser_gen(self->lp)) != t_scolon) {
		printf("1\n");
	}
	printf("</expr>\n");
}

void SyntaxAnalyzer_parseAsigment(SyntaxAnalyzer * self, iVar * variableTo) {
	SyntaxAnalyzer_parseExpr(self, NULL);
}

void SyntaxAnalyzer_parse_varDeclr(SyntaxAnalyzer * self) {
	// read all variable declarations
	while (true) {
		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_id) {
			return;
		}

		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_colon) {
			syntaxError("expected :\n", self->lp->lineNum);
			return;
		}

		self->lastToken = LexParser_gen(self->lp);
		if (!Token_isType(self->lastToken)) {
			syntaxError("expected type name\n", self->lp->lineNum);
			return;
		}
		self->lastToken = LexParser_gen(self->lp);
		if (self->lastToken != t_scolon) {
			syntaxError("expected id\n", self->lp->lineNum);
			return;
		}
	}
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
			syntaxError("Unnexpected syntax in code block\n",
					self->lp->lineNum);
		}
	}

}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {
	SyntaxAnalyzer_parseExpr(self, NULL);
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_then) {
		syntaxError("expected then\n", self->lp->lineNum);
		return;
	}
	SyntaxAnalyzer_parse_block(self);

	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken == t_else) {
		SyntaxAnalyzer_parse_block(self);
	}
}

//"while" already found
void SyntaxAnalyzer_parse_while(SyntaxAnalyzer * self) {
	SyntaxAnalyzer_parseExpr(self, NULL);
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_do) {
		syntaxError("expected id\n", self->lp->lineNum);
		return;
	}
	SyntaxAnalyzer_parse_block(self);
}

void SyntaxAnalyzer_parse_argList(SyntaxAnalyzer * self) {
	printf("not implemented/ SyntaxAnalyzer_parse_argList\n");
}

//"function" already found
void SyntaxAnalyzer_parse_func(SyntaxAnalyzer * self) {
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_id) {
		syntaxError("id of function expected\n", self->lp->lineNum);
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
		printToken(self->lp, tok);
		switch (tok) {
		//case t_var:
		//	SyntaxAnalyzer_parse_varDeclr(self);
		//	break;
		//case t_begin:
		//	SyntaxAnalyzer_parse_block(self);
		//	break;
		//case t_func:
		//	SyntaxAnalyzer_parse_func(self);
		//	break;
		//case t_period:
		//	tok = LexParser_gen(self->lp);
		//	if (tok == t_eof)
		//		return;
		//	else {
		//		syntaxError("No input expected\n", self->lp->lineNum);
		//		return;
		//	}
		case t_eof:
			syntaxError("Expected \".\"\n", self->lp->lineNum);
			return;
			//default:
			//	syntaxError("syntax corrupted\n", self->lp->lineNum);
		}
	}
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
}
