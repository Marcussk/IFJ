#include "lexParser.h"

void LexParser__init__(LexParser * p, FILE * inFile) {
	String__init__(&p->str, 32);
	p->input = inFile;
	p->state = lp_read;
	p->lineNum = 0;
	p->tParser = TokenParser__init__();
	p->lastToken = t_empty;
}
bool endOfIdOrNum(int idLen, char ch) {
	if (ch == '_')
		return false; // _ can be anywhere
	return !isalnum(ch);
}

// generates Tokens, name is LexParser_iterator, accepts  LexParser * p, return t_eof as a default return type
GENERATOR(Token, LexParser_iterator, LexParser *, pa, t_eof) {
	static LexParser * p;
	static char ch;
	static Token tokBackup = t_empty;
	p = pa;

	while ((ch = fgetc(p->input)) != EOF) {
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

			if (endOfIdOrNum(p->str.len, ch)) {
				if (p->str.len > 0) {
					if (p->lastToken != t_empty) {
						YIELD(LexParser_iterator, p->lastToken);
					} else {
						YIELD(LexParser_iterator, t_id);
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
					YIELD(LexParser_iterator, tokBackup);
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
				YIELD(LexParser_iterator, t_string);
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
				YIELD(LexParser_iterator, t_string);
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
		default:
			syntaxError("Lex parser in unknown state.", p->lineNum);
		}
		if (ch == '\n')
			p->lineNum += 1;

	}
	YIELD(LexParser_iterator, t_eof);
}

//GENERATOR(Token, LexParser_iterator_var,  LexParser *, pa, t_eof){
//	while(true){
//
//		YIELD(LexParser_iterator_var, )
//	}
//}

void LexParser_clear(LexParser * p) {
	String_clear(&(p->str));
	p->lineNum = 0;
}

void LexParser__dell__(LexParser * p) {
	String__dell_(&(p->str));
	TokenParser__dell__(&(p->tParser));
}
