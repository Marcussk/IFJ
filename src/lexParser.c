#include "lexParser.h"

void LexParser__init__(LexParser * p, FILE * inFile) {
	String__init__(&p->str, 32);
	p->input = inFile;
	p->state = lp_read;
	p->lineNum = 0;
	p->tParser = TokenParser__init__();
	p->lastToken = t_empty;
	p->lastSymbol = NULL;
}

bool endOfIdOrNum(int idLen, char ch) {
	if (ch == '_')
		return false; // _ can be anywhere
	return !isalnum(ch);
}

// name have to be longerr than 0
bool isValidId(char * name){
	return isalpha(name[0]) || name[0]== '_';
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
						YIELD(LexParser_gen, p->lastToken);
					} else {
						if(isValidId(p->str.buff)){
							HashTable_insert(symbolTable, p->str.buff, &(p->lastSymbol));
							YIELD(LexParser_gen, t_id);
						}else{
							YIELD(LexParser_gen, t_invalid);
						}
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
					YIELD(LexParser_gen, tokBackup);
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
				YIELD(LexParser_gen, t_string);
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
				YIELD(LexParser_gen, t_string);
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
	YIELD(LexParser_gen, t_eof);
}

void LexParser__dell__(LexParser * p) {
	String__dell_(&(p->str));
	TokenParser__dell__(&(p->tParser));
}
