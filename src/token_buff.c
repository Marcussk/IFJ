#include "token_buff.h"

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
			Error_syntax("Can't pushback so many tokens", self->lp->input.line,
					getTokenName(t));
	} else {
		self->next1 = t;
	}
}
void TokenBuff__dell__(TokenBuff * self) {
	if (self->next1 != t_empty || self->next2 != t_empty) {
		Error_syntax("Trying to dispose token buffer which is not empty",
				self->lp->input.line, NULL);
	}
}
