#ifndef TOKEN_BUFF_h
#define TOKEN_BUFF_h

#include "lexParser.h"
#include "tokenMap.h"
#include "errorHandler.h"

typedef struct {
	LexParser * lp;
	Token next2;
	Token next1;
} TokenBuff;

void TokenBuff__init__(TokenBuff * self, LexParser * lp);
Token TokenBuff_next(TokenBuff * self);
void TokenBuff_pushBack(TokenBuff * self, Token t);
void TokenBuff__dell__(TokenBuff * self);

#endif
