#include "token_buff.h"

void TokenBuff_throw_syntaxErr(char * msg, TokenBuff * self, Token t) {
	Err_syntax err = Syntax_err_create(self->lp);
	err.actualTokenName = getTokenName(t);
	err.msg = msg;
	Error_syntax(err);
}

void TokenBuff__init__(TokenBuff * self, LexParser * lp) {
	self->lp = lp;
	self->next2 = t_empty;
	self->next1 = t_empty;
}

Token TokenBuff_next(TokenBuff * self) {
	Token ret;
	if (self->next1 != t_empty) {
		ret = self->next1;
		self->next1 = self->next2;
		self->next2 = t_empty;
		return ret;
	}
	return LexParser_next(self->lp);
}
void TokenBuff_pushBack(TokenBuff * self, Token t) {
	if (self->next1 != t_empty) {
		if (self->next2 == t_empty) {
			self->next2 = self->next1;
			self->next1 = t;
		} else
			TokenBuff_throw_syntaxErr("Can't pushback so many tokens", self, t);
	} else {
		self->next1 = t;
	}
}
void TokenBuff__dell__(TokenBuff * self) {
	char* errMsg = "Trying to dispose token buffer which is not empty";
	if (self->next1 != t_empty)
		TokenBuff_throw_syntaxErr(errMsg, self, self->next1);
	else if (self->next2 != t_empty)
		TokenBuff_throw_syntaxErr(errMsg, self, self->next2);
}
