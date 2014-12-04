#include "lex_parser.h"

void LexParser_readEscape(LexParser * self);
void LexParser_readString(LexParser * self);

void LexParser__init__(LexParser * self, FILE * inFile) {
	String__init__(&self->str, 32);
	BuffFile__init__(&(self->input), inFile);
	self->lineNum = 1;
	self->tParser = TokenParser__init__();
	self->lastToken = t_empty;
	self->preLastToken = t_empty;
	self->planedJob = j_continue;
	self->idMode = lp_searchOnly;
	self->lastSymbol = NULL;
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
	self->lastToken = t_str_val;
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
	self->lastToken = t_str_val;
	if (ch == '\'')
		LexParser_readString(self);
	else if (ch == '#')
		LexParser_readEscape(self);
	else
		BuffFile_pushBack(&(self->input), ch);
}

void LexParser_syncLastVar(LexParser * self, Token t) {
	HashTableItem * i;
	if (t == t_id) {
		switch (self->idMode) {
		case lp_insertOnly:
			if (HashTable_insert(self->symbolTable, self->str.buff,
					&(self->lastSymbol)) != ht_inserted) {
				sem_definitionError(self->lineNum, self->str.buff); // redefinition
			}
			break;
		case lp_parseParams:
			if (HashTable_insert(self->symbolTable, self->str.buff,
					&(self->lastSymbol)) != ht_inserted) {
				sem_definitionError(self->lineNum, self->str.buff); // redefinition
			}
			iFunction_addParam(self->symbolTable->masterItem->val.fn, self->lastSymbol);
			break;
		case lp_searchOnly:
			i = HashTable_lookupEverywhere(self->symbolTable, self->str.buff);
			if (!i) {
				sem_definitionError(self->lineNum, self->str.buff);
			}
			self->lastSymbol = i->var;
			break;
		//case lp_debug:
		//	i = HashTable_lookupEverywhere(self->symbolTable, self->str.buff);
		//	if (i)
		//		self->lastSymbol = i->var;
		//	break;

		default:
			lexError("LexParser don't know if search or insert new id\n",
					self->str.buff, self->lineNum);
		}
	}

}

/* when token is id name is in p->str.buff and variable is pushed in symbol table (not jet) and  reference is p->lastSymbol (not jet)
 * when token is some constant (string, int, real) string value is in p->str.buff and this value is parsed to iVar an can be accessed
 * trough the  p->lastSymbol
 * */
Token LexParser_next(LexParser *self) {
	char ch;

	switch (self->planedJob) {
	case j_continue:
		break;
	case j_reset:
		self->planedJob = j_continue;
		String_clear(&(self->str));
		self->lastToken = t_empty;
		TokenParser_reset(&(self->tParser));
		break;
	case j_readStr:
		String_clear(&(self->str));
		LexParser_readString(self);
		self->planedJob = j_reset;
		return self->lastToken;
	case j_readEscape:
		String_clear(&(self->str));
		LexParser_readEscape(self);
		self->planedJob = j_reset;
		return self->lastToken;

	}

	while ((ch = BuffFile_get(&(self->input))) != EOF) {
		if (ch == '{') {
			LexParser_readComment(self);
			String_clear(&(self->str));
			TokenParser_reset(&(self->tParser));
		} else if (ch == '\'') {
			if (self->lastToken != t_empty) {
				self->planedJob = j_readStr;
				return self->lastToken;
			}
			self->lastToken = t_str_val;
			String_clear(&(self->str));
			LexParser_readString(self);
			self->planedJob = j_reset;

			return self->lastToken;

		} else if (ch == '#') {
			if (self->lastToken != t_empty) {
				self->planedJob = j_readEscape;
				return self->lastToken;
			}
			String_clear(&(self->str));
			self->lastToken = t_empty;
			LexParser_readEscape(self);
			self->planedJob = j_reset;
			return self->lastToken;
		} else {
			ch = tolower(ch);
			self->preLastToken = self->lastToken;
			self->lastToken = TokenParser_push(&(self->tParser), ch);
			if (self->lastToken == t_invalid)
				lexError("syntax does not fit\n", self->str.buff,
						self->lineNum);

			if (self->lastToken == t_empty && self->preLastToken != t_empty) {
				BuffFile_pushBack(&(self->input), ch);
				self->planedJob = j_reset;
				LexParser_syncLastVar(self, self->preLastToken);
				//	self->lastToken = t_empty;
				return self->preLastToken;
			} else {
				if (self->lastToken == t_empty) {
					String_clear(&(self->str));
					TokenParser_reset(&(self->tParser));
				} else {
					String_append(&(self->str), ch);
				}
			}
		}
		if (ch == '\n')
			self->lineNum += 1;
	}
	return t_eof;
}

bool CanAddAsParam(LexParser * self) {
	iVar * mi = self->symbolTable->masterItem;
	return (mi && mi->type);
}

//create new Symbol table for this function, save next Ids to fn params
void LexParser_fnParamsEnter(LexParser * self) {
	HashTable * fnSymTable = HashTable__init__(SYMBOL_TABLE_SIZE);
	iVar * fnRecurse = NULL;
	self->idMode = lp_parseParams;
	if( HashTable_insert(fnSymTable,self->str.buff, &fnRecurse) !=ht_inserted){
		lexError("Error while creating symbol for function recurse",self->str.buff, self->lineNum);
	}
	fnSymTable->masterTable = self->symbolTable;
	fnSymTable->masterItem = self->lastSymbol;
	*fnRecurse = *fnSymTable->masterItem;
	self->symbolTable = fnSymTable;
}

// end saving ids to params, save function return type and return variable to this fn symbol table
void LexParser_fnDefEnter(LexParser * self, tIFJ fnReturnType) {
	iVar * fn = self->symbolTable->masterItem;
	fn->val.fn->retVal.type = fnReturnType;
}
//set up symbol table back to global
void LexParser_fnBodyLeave(LexParser * self) {
	HashTable * st = self->symbolTable;
	st->masterItem->isInitialized = true;
	self->symbolTable = st->masterTable;
}

void LexParser__dell__(LexParser * self) {
	String__dell_(&(self->str));
	TokenParser__dell__(&(self->tParser));
	HashTable__dell__(self->symbolTable);
}
