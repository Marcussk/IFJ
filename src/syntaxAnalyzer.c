#include "syntaxAnalyzer.h"

hashTable * ht;

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
}

tIFJ token2Type(Token t) {
	switch (t) {
	case t_string:
		return iString;
	case t_integer:
		return iInt;
	case t_real:
		return iReal;
	default:
		return iUnknown;
	}
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	ht = HashTable__init__(128);
	while ((self->lastToken = LexParser_iterator(self->lp)) != t_eof) {

	}
	HashTable__dell__(ht);
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
}
