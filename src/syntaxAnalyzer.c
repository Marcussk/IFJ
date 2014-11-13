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
	case t_string:
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

void SyntaxAnalyzer_parseCond(SyntaxAnalyzer * se) {

}
void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * se) {

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

}

//"if" already found
void SyntaxAnalyzer_parse_if(SyntaxAnalyzer * self) {
	SyntaxAnalyzer_parseCond(self);
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
	SyntaxAnalyzer_parseCond(self);
	self->lastToken = LexParser_gen(self->lp);
	if (self->lastToken != t_do) {
		syntaxError("expected id\n", self->lp->lineNum);
		return;
	}
	SyntaxAnalyzer_parse_block(self);
}

void SyntaxAnalyzer_parse_argList(SyntaxAnalyzer * self) {

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
				syntaxError("No input expected\n", self->lp->lineNum);
				return;
			}
		case t_eof:
			syntaxError("Expected \".\"\n", self->lp->lineNum);
			return;
		default:
			syntaxError("syntax corrupted\n", self->lp->lineNum);
		}
	}
	printToken(self->lp, tok);

}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
}
