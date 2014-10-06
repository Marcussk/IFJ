#include "lexParser.h"

LexParser LexParser__init__(void (*newTokenEv)(int)) {
	LexParser p;
	String__init__(&p.str, 32);
	p.newTokenEv = newTokenEv;
	p.state = lp_read;
	p.lineNum = 0;
	p.tParser = TokenParser__init__();
	return p;
}

void LexParser_push(LexParser * p, char ch) {
	Token t;
	char * tokenStr = NULL;
	switch (p->state) {
	case lp_read:
		if (ch == '{')
			p->state = lp_comment;
		if (p->state == lp_read) {
			t = TokenParser_feed(&(p->tParser), tolower(ch));
		}
		break;
	case lp_comment:
		if (ch == '}')
			p->state = lp_read;
		break;
	}
	if (ch == '\n')
		p->lineNum += 1;
	if (p->state == lp_read) {
		String_append(&(p->str), ch);
	}
	if (t) {
		tokenStr = getTokenStr(t);
		if (tokenStr) {
			printf("[%s]\n", tokenStr);
			String_clear(&(p->str));
		}
	}

}

void LexParser_clear(LexParser * p) {
	String_clear(&(p->str));
	p->lineNum = 0;
}

void LexParser__dell__(LexParser * p) {
	String__dell_(&(p->str));
	p->newTokenEv = NULL;
	TokenParser__dell__(&(p->tParser));
}
