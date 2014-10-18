#include "lexParser.h"

LexParser LexParser__init__(void (*newTokenEv)(LexParser * p, Token t)) {
	LexParser p;
	String__init__(&p.str, 32);
	p.newTokenEv = newTokenEv;
	p.state = lp_read;
	p.lineNum = 0;
	p.tParser = TokenParser__init__();
	p.lastToken = t_empty;
	return p;
}

bool endOfId(int idLen, char ch) {
	if (ch == '_')
		return false; // _ can be anywhere
	if (idLen == 0) {
		return !isalpha(ch);
	} else {
		return !isalnum(ch);
	}
}

void LexParser_push(LexParser * p, char ch) {
	Token tokBackup = t_empty;

	switch (p->state) {
	case lp_read:
		switch (ch) {
		case '{':
			p->state = lp_comment;
			break;
		case '\'':
			p->state = lp_string;
			break;
		case '#':
			p->state = lp_escape;
			break;
		}

		if (endOfId(p->str.len, ch)) {
			if (p->str.len > 0) {
				if (p->lastToken != t_empty) {
					p->newTokenEv(p, p->lastToken);
				} else {
					p->newTokenEv(p, t_id);
				}
				String_clear(&(p->str));
				TokenParser_reset(&(p->tParser));
				p->lastToken = t_empty;
			}
		} else {
			String_append(&(p->str), ch);
		}
		if (p->state == lp_read) {
			tokBackup = p->lastToken;
			p->lastToken = TokenParser_push(&(p->tParser), tolower(ch));
			if (p->str.len == 0 && tokBackup != t_empty
					&& p->lastToken == t_empty) {
				p->newTokenEv(p, tokBackup);
			}
		} else {
			p->lastToken = t_empty;
		}
		break;
	case lp_comment:
		if (ch == '}') {
			p->state = lp_read;
			String_clear(&(p->str));
		}
		break;
	case lp_string:
		if (ch == '\'') {
			p->state = lp_read;
			p->newTokenEv(p, t_string);
			String_clear(&(p->str));
		} else {
			String_append(&(p->str), ch);
		}
		break;
	case lp_escape:
		if (isdigit(ch)) {
			String_append(&(p->str), ch);
		} else {
			//end of escape, send escaped character as a string
			char escp = atoi(p->str.buff);
			String_clear(&(p->str));
			String_append(&(p->str), escp);
			p->newTokenEv(p, t_string);
			String_clear(&(p->str));
			TokenParser_reset(&(p->tParser));
			if (ch == '\'')
				p->state = lp_string;
			else if (ch == '#')
				p->state = lp_escape;
			else
				p->state = lp_error;
		}
		break;
	case lp_error:
		break;
	default:
		syntaxError("Lex parser in unknown state.", p->lineNum);
	}
	if (ch == '\n')
		p->lineNum += 1;
}

void LexParser_flush(LexParser * p) {
	switch (p->state) {
	case lp_read:
		LexParser_push(p, ' '); // end for shure
		break;
	case lp_comment:
		syntaxError("Unexpected end of comment.", p->lineNum);
		break;
	case lp_string:
		syntaxError("Unexpected end of string.", p->lineNum);
		break;
	default:
		syntaxError("Something missing at the end.", p->lineNum);
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
