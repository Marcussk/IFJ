#include "lexParser.h"

void LexParser__init__(LexParser * p, FILE * inFile) {
	String__init__(&p->str, 32);
	BuffFile__init__(&(p->input), inFile);
	p->lineNum = 0;
	p->tParser = TokenParser__init__();
	p->lastToken = t_empty;
	p->preLastToken = t_empty;
	p->planedJob = j_continue;
	p->pushBackTok = t_empty;
//	p->lastSymbol = NULL;
}

void LexParser_readString(LexParser * p) {
	char ch;
	while ((ch = BuffFile_get(&(p->input))) != '\'') {
		if (ch == EOF)
			lexError("String missing right ' (end of string).\n", p->str.buff,
					p->lineNum);
		if (ch == '\n')
			p->lineNum = p->lineNum + 1;
		String_append(&(p->str), ch);
	}
	p->lastToken = t_str_val;
}

void LexParser_readComment(LexParser * p) {
	char ch;
	while ((ch = BuffFile_get(&(p->input))) != '}') {
		if (ch == EOF)
			lexError("String missing right } (end of comment).\n", p->str.buff,
					p->lineNum);
		else if (ch == '\n')
			p->lineNum = p->lineNum + 1;
	}
}
void LexParser_readEscape(LexParser * p) {
	char ch = '0';
	while (isdigit(ch)) {
		ch = BuffFile_get(&(p->input));
		String_append(&(p->str), ch);
	}
	BuffFile_pushBack(&(p->input), ch);
	if (p->str.len < 1)
		lexError("Uncomplet escape sequention.\n", p->str.buff, p->lineNum);
	int escp = atoi(p->str.buff);
	if (escp > 255)
		lexError("Unknown escape sequention.\n", p->str.buff, p->lineNum);
	String_clear(&(p->str));
	String_append(&(p->str), (char) escp);
	p->lastToken = t_str_val;
}

void LexParser_pushBack(LexParser * p, Token t){
	if(	p->pushBackTok != t_empty)
		syntaxError("Can push back to lex parser only one token needs more\n", p->lineNum);
	p->pushBackTok = t;
}


/* when token is id name is in p->str.buff and variable is pushed in symbol table (not jet) and  reference is p->lastSymbol (not jet)
 * when token is some constant (string, int, real) string value is in p->str.buff and this value is parsed to iVar an can be accessed
 * trough the  p->lastSymbol
 * */
Token LexParser_gen(LexParser *p) {
	char ch;
	Token tokPushBackBackup;

	if(p->pushBackTok != t_empty){
		tokPushBackBackup = p->pushBackTok;
		p->pushBackTok = t_empty;
		return tokPushBackBackup;
	}

	switch (p->planedJob) {
	case j_continue:
		break;
	case j_reset:
		p->planedJob = j_continue;
		String_clear(&(p->str));
		p->lastToken = t_empty;
		TokenParser_reset(&(p->tParser));
		break;
	case j_readStr:
		String_clear(&(p->str));
		LexParser_readString(p);
		p->planedJob = j_reset;
		return p->lastToken;
	case j_readEscape:
		String_clear(&(p->str));
		p->lastToken = t_empty;
		LexParser_readEscape(p);
		p->planedJob = j_reset;
		return p->lastToken;

	}

	while ((ch = BuffFile_get(&(p->input))) != EOF) {
		if (ch == '{') {
			LexParser_readComment(p);
			String_clear(&(p->str));
			TokenParser_reset(&(p->tParser));
		} else if (ch == '\'') {
			if (p->lastToken != t_empty) {
				p->planedJob = j_readStr;
				return p->lastToken;
			}
			String_clear(&(p->str));
			LexParser_readString(p);
			p->planedJob = j_reset;
			return p->lastToken;

		} else if (ch == '#') {
			if (p->lastToken != t_empty) {
				p->planedJob = j_readEscape;
				return p->lastToken;
			}
			String_clear(&(p->str));
			p->lastToken = t_empty;
			LexParser_readEscape(p);
			p->planedJob = j_reset;
			return p->lastToken;
		} else {
			ch = tolower(ch);
			p->preLastToken = p->lastToken;
			p->lastToken = TokenParser_push(&(p->tParser), ch);
			if (p->lastToken == t_invalid)
				lexError("syntax does not fit\n", p->str.buff, p->lineNum);

			if (p->lastToken == t_empty && p->preLastToken != t_empty) {
				if (ch != '\'' && ch != '{' && ch != '#')
					BuffFile_pushBack(&(p->input), ch);
				p->planedJob = j_reset;
				return p->preLastToken;
			} else {
				if (p->lastToken == t_empty) {
					String_clear(&(p->str));
					TokenParser_reset(&(p->tParser));
				} else {
					String_append(&(p->str), ch);
				}
			}
		}
		if (ch == '\n')
			p->lineNum += 1;
	}
	return t_eof;
}


void LexParser__dell__(LexParser * p) {
	String__dell_(&(p->str));
	TokenParser__dell__(&(p->tParser));
}
