#include "lexParser.h"

void LexParser__init__(LexParser * p, FILE * inFile) {
	String__init__(&p->str, 32);
	BuffFile__init__(&(p->input), inFile);
	p->lineNum = 0;
	p->tParser = TokenParser__init__();
	p->lastToken = t_empty;
//	p->lastSymbol = NULL;
}

void LexParser_readString(LexParser * p) {
	char ch;
	while ((ch = BuffFile_get(&(p->input))) != '\'') {
		if (ch == EOF)
			lexError("String missing right ' (end of string).\n", p->str.buff,
					p->lineNum);
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
		String_append(&(p->str), ch);
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
/*generates Tokens, name is LexParser_iterator, accepts  LexParser * p, return t_eof as a default return type
 *
 * when token is id name is in p->str.buff and variable is pushed in symboltable and reference is p->lastSymbol
 * when token is some constant (string, int, real) string value is in p->str.buff and this value is parsed to iVar an can be accessed
 * trough the  p->lastSymbol
 * */
GENERATOR(Token, LexParser_gen, LexParser *, pa, t_eof) {
	static LexParser * p;
	static char ch;
	static Token tokBackup = t_empty;
	p = pa;

	while ((ch = BuffFile_get(&(p->input))) != EOF) {
		if (ch == '{') {
			LexParser_readComment(p);
			String_clear(&(p->str));
			TokenParser_reset(&(p->tParser));
		} else if (ch == '\'') {
			if (p->lastToken != t_empty)
				YIELD(LexParser_gen, p->lastToken);
			String_clear(&(p->str));
			LexParser_readString(p);
			YIELD(LexParser_gen, p->lastToken);
			String_clear(&(p->str));
			p->lastToken = t_empty;
			TokenParser_reset(&(p->tParser));
		} else if (ch == '#') {
			if (p->lastToken != t_empty) {
				YIELD(LexParser_gen, p->lastToken);
				String_clear(&(p->str));
				p->lastToken = t_empty;
			}
			LexParser_readEscape(p);
			YIELD(LexParser_gen, p->lastToken);
			String_clear(&(p->str));
			p->lastToken = t_empty;
			TokenParser_reset(&(p->tParser));
		} else {
			ch = tolower(ch);
			tokBackup = p->lastToken;
			p->lastToken = TokenParser_push(&(p->tParser), ch);
			if (p->lastToken == t_invalid)
				lexError("syntax does not fit\n", p->str.buff, p->lineNum);

			if (p->lastToken == t_empty && tokBackup != t_empty) {
				YIELD(LexParser_gen, tokBackup);
				String_clear(&(p->str));
				TokenParser_reset(&(p->tParser));
				if (ch != '\'' && ch != '{' && ch != '#')
					BuffFile_pushBack(&(p->input), ch);
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
	YIELD(LexParser_gen, t_eof);
}

void LexParser__dell__(LexParser * p) {
	String__dell_(&(p->str));
	TokenParser__dell__(&(p->tParser));
}
