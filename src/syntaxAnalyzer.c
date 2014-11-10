#include "syntaxAnalyzer.h"


#define SYNNEXT LexParser_iterator_reNew();		    \
	self->lastToken = LexParser_iterator(self->lp); \

#define SYNCHECK(expectedToken, errMsg)                                 \
		if (self->lastToken != expectedToken) {                         \
			fprintf(stderr, "token:%s\n", getTokenStr(self->lastToken));\
			syntaxError(errMsg, self->lp->lineNum);                     \
			id_Stack__dell__(&idStack);                                 \
			return;                                                     \
		}

IMPL_STACK(char *, id_);

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

//id -> id, id
//id:type;

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

//if token != id || ,  return while they are push ids on stack
void pushBareIds(id_stackT * idStack, SyntaxAnalyzer * self) {
	SYNNEXT
	printf("<pushBareIds>\n");
	while (self->lastToken == t_id) {
		if (self->lastToken != t_id) {
			syntaxError("In declaration fist shod be id\n", self->lp->lineNum);
			return;
		} else {
			printf("push id\n");
			id_Stack_Push(idStack, strdup(self->lp->str.buff));
		}
		SYNNEXT
		if (self->lastToken != t_comma) {
			syntaxError("Expect comma\n", self->lp->lineNum);
			return;
		}
		SYNNEXT;
	}
}

void ids2Table(id_stackT * stack, tIFJ type, int lineNum) {
	char * name;
	iVar * var;
	while (!id_Stack_IsEmpty(stack)) {
		name = id_Stack_Pop(stack);
		if (HashTable_insert(ht, name, &var) == 2) {
			syntaxError("Symbol redefinition after var block", lineNum);
			return;
		}
		var->type = type;
	}
}

void read_declaration(SyntaxAnalyzer * self) {
	id_stackT idStack;
	id_Stack__init__(&idStack, 4096);
	tIFJ typeBackup;
	printf("<var declr.>\n");
	while (true) {
		pushBareIds(&idStack, self);
		SYNCHECK(t_doubleDot, "Expected \":\" \n")
		SYNNEXT
		typeBackup = token2Type(self->lastToken);
		if (typeBackup == iUnknown) {
			syntaxError("Missing type specificator\n", self->lp->lineNum);
			id_Stack__dell__(&idStack);
			return;
		}
		SYNCHECK(t_scolon, "In declaration fist shod be id\n")
		if (self->lastToken != t_id) {
			id_Stack__dell__(&idStack);
			ids2Table(&idStack, typeBackup, self->lp->lineNum);
			return;
		}
	}
}

void SyntaxAnalyzer_parse(SyntaxAnalyzer * self) {
	ht = HashTable__init__(128);
	while ((self->lastToken = LexParser_iterator(self->lp)) != t_eof) {
		printf("<SyntaxAnalyzer_parse>\n");
		//printToken(self->lp, t);
		switch (self->lastToken) {
		case t_var:
			read_declaration(self);
			printf("read_declaration end\n");
			break;
		case t_begin:
			printf("<begin>\n");
			break;
		default:
			HashTable_print(ht);
			syntaxError("Token type does not fit to syntax\n",
					self->lp->lineNum);
		}
		//printToken(self->lp, self->lastToken);
		HashTable__dell__(ht);
	}
}

void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self) {
	LexParser__dell__(self->lp);
}
