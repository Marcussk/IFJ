#include "lex_parser.h"

typedef enum {
	np_int,
	np_needAfterDot,
	np_needExponent,
	np_real,
	np_expReal,
	np_expReal_needAfterDot,
	np_expReal_realExp
} NumParsingState;

void LexParser_readEscape(LexParser * self);
void LexParser_readString(LexParser * self);

void LexParser__init__(LexParser * self, FILE * inFile) {
	String__init__(&self->str, 32);
	BuffFile__init__(&(self->input), inFile);
	self->lineNum = 1;
	self->lastFunction = NULL;
	self->lastSymbol = NULL;
	self->idMode = lp_searchOnly;
	self->symbolTable = HashTable__init__(SYMBOL_TABLE_SIZE);
	registrBuiltins(self->symbolTable);
}

void LexParser_readString(LexParser * self) {
	char ch, ch2;
	while (true) {
		ch = BuffFile_get(&(self->input));
		if (ch == EOF)
			lexError("String missing right ' (end of string).\n",
					self->str.buff, self->lineNum);
		else if (ch == '\n')
			self->lineNum = self->lineNum + 1;
		else if (ch == '\'') {
			ch2 = BuffFile_get(&(self->input));
			if (ch2 != '\'') {
				ch = ch2;
				break;
			}
		}
		String_append(&(self->str), ch);
	}
	/*BuffFile_pushBack(&(self->input), ch);
	 ch = BuffFile_get(&(self->input));*/
	while (true) {
		if (ch == '\'') {
			LexParser_readString(self);
		} else if (ch == '#') {
			LexParser_readEscape(self);
		} else {
			break;
		}
		ch = BuffFile_get(&(self->input));
	}
	BuffFile_pushBack(&(self->input), ch);
}

void LexParser_readComment(LexParser * self) {
	char ch;
	while ((ch = BuffFile_get(&(self->input))) != '}') {
		if (ch == EOF)
			lexError("String missing right } (end of comment).\n",
					self->str.buff, self->lineNum);
		else if (ch == '\n')
			self->lineNum = self->lineNum + 1;
	}
}
void LexParser_readEscape(LexParser * self) {
	char ch = '0';
	String str;
	String__init__(&str, 4);
	while (isdigit(ch)) {
		ch = BuffFile_get(&(self->input));
		String_append(&str, ch);
	}
	if (str.len < 1)
		lexError("Uncomplet escape sequention.\n", str.buff, self->lineNum);
	int escp = atoi(str.buff);
	String__dell_(&str);
	if (escp > 255)
		lexError("Unknown escape sequention.\n", self->str.buff, self->lineNum);
	String_append(&(self->str), (char) escp);
	if (ch == '\'')
		LexParser_readString(self);
	else if (ch == '#')
		LexParser_readEscape(self);
	else
		BuffFile_pushBack(&(self->input), ch);
}

void LexParser_syncLastVar(LexParser * self) {
	HashTableItem * i;
	iVar * v;
	switch (self->idMode) {
	case lp_insertOnly:
		if (HashTable_insertNew(self->symbolTable, self->str.buff,
				&(self->lastSymbol)) != ht_inserted) {
			sem_definitionError(self->lineNum, self->str.buff); // redefinition
		}
		break;
	case lp_parseParams:
		v = malloc(sizeof(iVar));
		iVar__init__(v);
		self->lastSymbol = v;
		iFunction_addParam(self->lastFunction, self->lastSymbol,
				strdup(self->str.buff));
		break;
	case lp_fnSearch:
		i = HashTable_lookup(self->symbolTable, self->str.buff);
		if (!i) {
			if (HashTable_insertNew(self->symbolTable, self->str.buff,
					&(self->lastSymbol)) != ht_inserted) {
				sem_definitionError(self->lineNum, self->str.buff); // redefinition
			}
			self->lastSymbol->type = iFn;
			self->lastSymbol->val.fn = iFunction__init__();
			self->lastSymbol->val.fn->name = strdup(self->str.buff);
		} else {
			if (i->var->isInitialized == true || i->var->type != iFn)
				sem_definitionError(self->lineNum, self->str.buff);
			self->lastSymbol = i->var;
		}
		self->lastFunction = self->lastSymbol->val.fn;
		break;
	case lp_searchOnly:
		i = HashTable_lookupEverywhere(self->symbolTable, self->str.buff);
		if (!i) {
			sem_definitionError(self->lineNum, self->str.buff);
		}
		self->lastSymbol = i->var;
		break;
	case lp_ignore:
		break;
	default:
		lexError("LexParser don't know if search or insert new id\n",
				self->str.buff, self->lineNum);
	}
}
Token LexParser_keywordCheck(String * str) {
	if (str->len > 1 && str->len < 9) { // skip strings which have different lengths than keywords
		switch (str->buff[0]) {
		case 'b':
			if (str->len == 5 && !strcmp(str->buff, "begin"))
				return t_begin;
			if (str->len == 7 && !strcmp(str->buff, "boolean"))
				return t_boolean;
			break;
		case 'c':
			if (str->len == 4 && !strcmp(str->buff, "char"))
				return t_char;
			break;
		case 'd':
			if (str->len == 2 && str->buff[1] == 'o')
				return t_do;
			break;
		case 'e':
			if (str->len == 4 && !strcmp(str->buff, "else"))
				return t_else;
			if (str->len == 3 && !strcmp(str->buff, "end"))
				return t_end;
			break;
		case 'f':
			if (str->len == 5 && !strcmp(str->buff, "false"))
				return t_false;
			if (str->len == 8 && !strcmp(str->buff, "function"))
				return t_func;
			if (str->len == 7 && !strcmp(str->buff, "forward"))
				return t_false;
			break;
		case 'i':
			if (str->len == 2 && str->buff[1] == 'f')
				return t_if;
			if (str->len == 7 && !strcmp(str->buff, "integer"))
				return t_integer;
			break;
		case 'r':
			if (str->len == 4 && !strcmp(str->buff, "real"))
				return t_real;
			if (str->len == 6 && !strcmp(str->buff, "repeat"))
				return t_repeat;
			break;
		case 's':
			if (str->len == 6 && !strcmp(str->buff, "string"))
				return t_real;
			break;
		case 't':
			if (str->len == 4 && !strcmp(str->buff, "then"))
				return t_then;
			if (str->len == 4 && !strcmp(str->buff, "true"))
				return t_true;
			break;
		case 'u':
			if (str->len == 5 && !strcmp(str->buff, "until"))
				return t_until;
			break;
		case 'v':
			if (str->len == 3 && !strcmp(str->buff, "var"))
				return t_var;
			break;
		case 'w':
			if (str->len == 5 && !strcmp(str->buff, "while"))
				return t_while;
			break;
		}
	}
	return t_id;
}

Token LexParser_readIdOrKeyword(LexParser *self, char ch) {
	Token found;
	String_clear(&self->str);
	String_append(&self->str, tolower(ch));
	ch = tolower(BuffFile_get(&(self->input)));
	while ( isalnum(ch) || ch == '_') {
		String_append(&self->str, tolower(ch));
		ch = BuffFile_get(&(self->input));
	}
	BuffFile_pushBack(&self->input, ch);
	found = LexParser_keywordCheck(&self->str);
	if (found == t_id)
		LexParser_syncLastVar(self);
	return found;
}

Token LexParser_colonOrAssigment(LexParser *self) {
	char ch = BuffFile_get(&(self->input));
	if (ch == '=') {
		return t_asigment;
	} else {
		BuffFile_pushBack(&self->input, ch);
		return t_colon;
	}
}

Token LexParser_lessFound(LexParser * self) {
	char ch = BuffFile_get(&(self->input));
	switch (ch) {
	case '>':
		return t_notEqv;
	case '=':
		return t_lessOrEqv;
	default:
		BuffFile_pushBack(&self->input, ch);
	}
	return t_less;
}

Token LexParser_greaterFound(LexParser * self) {
	char ch = BuffFile_get(&(self->input));
	if (ch == '=') {
		return t_greaterOrEqv;
	} else {
		BuffFile_pushBack(&self->input, ch);
		return t_greater;
	}
}

Token LexParser_parseNum(LexParser * self, char ch) {
	NumParsingState st;
	if (ch == '.')
		st = np_needAfterDot;
	else
		st = np_int;
	String_clear(&self->str);
	String_append(&self->str, ch);

	while (true) {
		ch = BuffFile_get(&self->input);
		switch (st) {
		case np_int:
			if (isdigit(ch)) {
				break;
			} else if (ch == '.') {
				st = np_needAfterDot;
			} else if (ch == 'e' || ch == 'E') {
				ch = 'e';
				st = np_needExponent;
			} else {
				BuffFile_pushBack(&self->input, ch);
				return t_num_int;
			}
			break;
		case np_needAfterDot:
			if (isdigit(ch))
				st = np_real;
			else
				lexError("expected number after dot", self->str.buff,
						self->lineNum);
			break;
		case np_needExponent:
			if (isdigit(ch))
				st = np_expReal;
			else
				lexError("expected number as exponent", self->str.buff,
						self->lineNum);
			break;
		case np_real:
			if (isdigit(ch)) {
			} else if (ch == 'e' || ch == 'E') {
				ch = 'e';
				st = np_needExponent;
			} else {
				BuffFile_pushBack(&self->input, ch);
				return t_num_real;
			}
			break;
		case np_expReal:
			if (ch == '.')
				st = np_expReal_needAfterDot;
			else if (isdigit(ch)) {
				break;
			} else {
				BuffFile_pushBack(&self->input, ch);
				return t_num_real;
			}
			break;
		case np_expReal_needAfterDot:
			if (isdigit(ch))
				st = np_expReal_realExp;
			else
				lexError("expected number after dot in exponent",
						self->str.buff, self->lineNum);
			break;
		case np_expReal_realExp:
			if (!isdigit(ch)) {
				BuffFile_pushBack(&self->input, ch);
				return t_num_real;
			}
		}

		String_append(&self->str, ch);
	}
	lexError("Lex parser is not able to parse number", self->str.buff,
			self->lineNum);
	return t_invalid;
}

Token LexParser_next(LexParser *self) {
	char ch;
	char ch2;
	while ((ch = BuffFile_get(&(self->input))) != EOF) {
		if (isspace(ch)) {
			if (ch == '\n')
				self->lineNum++;
		} else
			switch (ch) {
			case ';':
				return t_scolon;
			case '-':
				return t_minus;
			case '+':
				return t_plus;
			case '*':
				return t_asterisk;
			case '/':
				return t_slash;
			case ',':
				return t_comma;
			case '=':
				return t_eqv;
			case '(':
				return t_lParenthessis;
			case ')':
				return t_rParenthessis;
			case '{':
				LexParser_readComment(self);
				break;
			case '\'':
				String_clear(&self->str);
				LexParser_readString(self);
				return t_str_val;
			case '.':
				ch2 = BuffFile_get(&(self->input));
				if (isdigit(ch2)) {
					BuffFile_pushBack(&(self->input), ch2);
					return LexParser_parseNum(self, ch);
				}
				return t_period;
			case ':':
				return LexParser_colonOrAssigment(self);
			case '<':
				return LexParser_lessFound(self);
			case '>':
				return LexParser_greaterFound(self);
			default:
				if (isdigit(ch)) {

					return LexParser_parseNum(self, ch);
				}
				if (isalpha(ch) || ch == '_')
					return LexParser_readIdOrKeyword(self, ch);
				else
					lexError("Cannot resolve token from string", self->str.buff,
							self->lineNum);
			}
	}
	return t_eof;
}

//create new Symbol table for this function, save next Ids to fn params
void LexParser_fnParamsEnter(LexParser * self) {
	self->idMode = lp_parseParams;
}

//set up symbol table back to global
void LexParser_fnBodyLeave(LexParser * self) {
	HashTable * st = self->symbolTable;
//st->masterItem->isInitialized = true;
	self->symbolTable = st->masterTable;
}

void LexParser__dell__(LexParser * self) {
	String__dell_(&(self->str));
	HashTable__dell__(self->symbolTable);
}

void LexParser_fnBodyEnter(LexParser * self, iVar * fn) {
	ParamsListItem * param = fn->val.fn->params.First;
	HashTable * fnSymTable = HashTable__init__(SYMBOL_TABLE_SIZE);

	if (HashTable_insert(fnSymTable, self->str.buff, fn) != ht_inserted) {
		lexError("Error while creating symbol for function recurse",
				self->str.buff, self->lineNum);
	}
	fnSymTable->masterTable = self->symbolTable;
	fnSymTable->masterItem = fn;
	self->symbolTable = fnSymTable;

	while (param) {
		if (HashTable_insert(self->symbolTable, param->name, param->data)
				!= ht_inserted) {
			sem_definitionError(self->lineNum, self->str.buff); // redefinition of param
		}
		param = param->next;
	}

}

