#include "token_map.h"

//describes string form of token
TokenMeaning tokenMeanings[] = {
		(TokenMeaning ) { t_begin, "begin" }, //keywords
		(TokenMeaning ) { t_boolean, "boolean" },
		(TokenMeaning ) { t_do, "do" },
		(TokenMeaning ) { t_else, "else" },
		(TokenMeaning ) { t_end, "end" },
		(TokenMeaning ) { t_false, "false" },
		(TokenMeaning ) { t_func, "function" },
		(TokenMeaning ) { t_if,	"if" },
		(TokenMeaning ) { t_integer, "integer" },
		(TokenMeaning ) { t_real, "real" },
		(TokenMeaning ) { t_char, "char" },
		(TokenMeaning ) { t_string, "string" },
		(TokenMeaning ) { t_then, "then" },
		(TokenMeaning ) { t_true, "true" },
		(TokenMeaning ) { t_var, "var" },
		(TokenMeaning ) { t_while, "while" },
		(TokenMeaning ) { t_plus, "+" }, // operators
		(TokenMeaning ) { t_minus, "-" },
		(TokenMeaning ) { t_asterisk, "*" },
		(TokenMeaning ) { t_slash, "/" },
		(TokenMeaning ) { t_eqv, "=" },
		(TokenMeaning ) { t_less, "<" },
		(TokenMeaning ) { t_greater, ">" },
		(TokenMeaning ) { t_lBracket, "[" },
		(TokenMeaning ) { t_rBracket, "]" },
		(TokenMeaning ) { t_period, "." },
		(TokenMeaning ) { t_comma, "," },
		(TokenMeaning ) { t_colon, ":" },
		(TokenMeaning ) { t_scolon, ";" },
		(TokenMeaning ) { t_pointer, "^" },
		(TokenMeaning ) { t_lParenthessis, "(" },
		(TokenMeaning ) { t_rParenthessis, ")" },
		(TokenMeaning ) { t_notEqv, "<>" },
		(TokenMeaning ) { t_lessOrEqv, "<=" },
		(TokenMeaning ) { t_greaterOrEqv, ">=" },
		(TokenMeaning ) { t_asigment, ":=" },
		(TokenMeaning ) { t_doubleDot, ".." },
		(TokenMeaning ) { t_lcBracket, "{" }, //others
		(TokenMeaning ) { t_rcBracket, "}" } };

bool Token_isType(Token t) {
	return t >= t_boolean && t <= t_string;
}

bool Token_isKeyword(Token t) {
	return  (t >= t_begin && t <= t_while);
}
bool Token_isValue(Token t) {
	return t == t_num_int || t == t_num_real || t == t_str_val || t == t_true
			|| t == t_false;
}

// reverz function, can get string form of token
char * getTokenStr(Token t) {
	int ti;
	int tokensCnt = STATIC_ARR_LEN(tokenMeanings);
	if (t == t_id)
		return "<Id>";
	if (t == t_num_int)
		return "<intNumber>";
	for (ti = 0; ti < tokensCnt; ti++) {
		TokenMeaning tm = tokenMeanings[ti];
		if (tm.token == t)
			return tm.str;
	}
	if (t == t_empty)
		return "<EmptyToken>";
	return NULL;
}

char * getTokenName(Token t) {
	switch (t) {
	case t_str_val:
		return "stringValue";
	case t_id:
		return "identificator";
	case t_num_int:
		return "integerValue";
	case t_num_real:
		return "realValue";
	case t_invalid:
		return "invalidToken";
	case t_eof:
		return "EOF";
	default:
		return getTokenStr(t);
	}
}

void TokenMap_forAllIdChars(TokenMapElement map[], TokenMapElement idsMap[],
		void (*fn)(TokenMapElement * el, TokenMapElement idsMap[])) {
	int ch = '0';
	for (ch = '0'; ch <= '9'; ch++) {
		fn(&(map[ch]), idsMap);
	}
	for (ch = 'A'; ch <= 'Z'; ch++) {
		fn(&(map[ch]), idsMap);
	}
	for (ch = 'a'; ch <= 'z'; ch++) {
		fn(&(map[ch]), idsMap);
	}
	fn(&(map['_']), idsMap);
}

void TokenMap_markAsId(TokenMapElement * elm, TokenMapElement idsMap[]) {
	if (elm->token == t_empty)
		elm->token = t_id;

	if (elm->next) {
		TokenMap_forAllIdChars(elm->next, idsMap, &TokenMap_markAsId);
		TokenMap_markAsId(elm->next, idsMap);
	} else {
		elm->next = idsMap;
	}
}

TokenMapElement * TokenMap_newLevel() {
	int i;
	TokenMapElement * newArr = malloc(
	TOKENMAP_NODESIZE * sizeof(TokenMapElement));
	if (!newArr)
		memoryError("Can't allocate new level of tokenMap");

	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		newArr[i].next = NULL;
		newArr[i].token = t_empty;
	}
	return newArr;
}

// have to be called as last otherwise chars behind keywords will not map
void TokenMap_addIds(TokenMapElement map[]) {
	TokenMapElement * idsMap = TokenMap_newLevel();
	TokenMap_forAllIdChars(idsMap, idsMap, &TokenMap_markAsId);
	int ch = '0';
	for (ch = 'A'; ch <= 'Z'; ch++)
		TokenMap_markAsId(&(map[ch]), idsMap);
	for (ch = 'a'; ch <= 'z'; ch++)
		TokenMap_markAsId(&(map[ch]), idsMap);
	TokenMap_markAsId(&(map['_']), idsMap);
}

// include token rule in token map
TokenMapElement * TokenMap_add(TokenMapElement map[], char * str, Token t) {
	int i;
	TokenMapElement * lastElm;
	char ch;
	for (i = 0; str[i]; i++) { //foreach char
		ch = str[i];
		if (str[i + 1]) { // if is not last char
			lastElm = &(map[(int) ch]);
			if (!lastElm->next) {
				lastElm->next = TokenMap_newLevel();
				if (!lastElm->next)
					memoryError("could't allocate token map");
			}
			map = lastElm->next;
		} else {
			map[(int) ch].token = t;
			return &(map[(int) ch]);
		}
	}
	return NULL;

}

//include rules for numbers in token map
/*
 * supported: d, d.d , d.de+-d, d.de+-d.d // +- resp + or -
 * */
void TokenParser_addNumbers(TokenMapElement map[]) {
	int i;
	TokenMapElement * elm;
	TokenMapElement * iElm;
	TokenMapElement * rElm;
	TokenMapElement * eFirst;
	TokenMapElement * eRInt;
	TokenMapElement * eRReal;
	TokenMapElement * intMap = TokenMap_newLevel();
	TokenMapElement * realMap = TokenMap_newLevel();
	TokenMapElement * expFirstMap = TokenMap_newLevel();
	TokenMapElement * expRestIntMap = TokenMap_newLevel();
	TokenMapElement * expRestRealMap = TokenMap_newLevel();


	for (i = '0'; i <= '9'; i++) {
		elm = &(map[i]);
		iElm = &(intMap[i]);
		rElm = &(realMap[i]);
		eFirst =  &(expFirstMap[i]);
		eRInt = &(expRestIntMap[i]);
		eRReal =&(expRestRealMap[i]);

		elm->token = t_num_int;
		elm->next = intMap;

		iElm->token = t_num_int;
		iElm->next = intMap;

		rElm->token = t_num_real;
		rElm->next = realMap;

		eFirst->token = t_num_real;
		eFirst->next = expRestIntMap;

		eRInt->token = t_num_real;
		eRInt->next = expRestIntMap;

		eRReal->token = t_num_real;
		eRReal->next = expRestRealMap;
	}
	intMap['.'].token = t_num_real;
	intMap['.'].next = realMap;
	intMap['e'].token = t_num_real;
	intMap['e'].next = expFirstMap;

	realMap['e'].token = t_num_real;
	realMap['e'].next = expFirstMap;

	expFirstMap['+'].token = t_num_real;
	expFirstMap['+'].next =expRestIntMap;
	expFirstMap['-'].token = t_num_real;
	expFirstMap['-'].next =expRestIntMap;

	expRestIntMap['.'].token = t_num_real;
	expRestIntMap['.'].next =expRestRealMap;

}

TokenParser TokenParser__init__() {
	int ti;
	TokenParser p;
	p.map = TokenMap_newLevel();
	TokenMapElement * map = p.map;
	TokenMeaning tm;
	int tokensCnt = STATIC_ARR_LEN(tokenMeanings);
	for (ti = 0; ti < tokensCnt; ti++) {
		tm = tokenMeanings[ti];
		if (!TokenMap_add(map, tm.str, tm.token)) {
			memoryError("could't allocate token map");
		}
	}
	p.possition = p.map;
	TokenParser_addNumbers(p.map);
	TokenMap_addIds(p.map);
	return p;
}

void TokenMap__dell__(TokenMapElement * map) {
	int i;
	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		TokenMapElement m = map[i];
		if (m.next)
			TokenMap__dell__(m.next);
	}
	if (map->token != t_id)
		free(map);
}

void TokenParser__dell__(TokenParser * p) {
	int i;
	TokenMapElement map;
	TokenMapElement * intMap = p->map['0'].next;
	TokenMapElement * realMap = intMap['.'].next;
	TokenMapElement * expFirstMap = intMap['e'].next;
	TokenMapElement * expRestIntMap = expFirstMap['1'].next;
	TokenMapElement * expRestRealMap =expRestIntMap['.'].next;
	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		map = p->map[i];
		if (map.token != t_num_int && map.token != t_id && map.next)
			TokenMap__dell__(map.next);
	}

	free(intMap);
	free(realMap);
    free(expFirstMap);
    free(expRestIntMap);
    free(expRestRealMap);

	free(p->map);
	p->map = NULL;
	p->possition = NULL;
}

void TokenParser_reset(TokenParser * p) {
	p->possition = p->map;
}

// parse another char
Token TokenParser_push(TokenParser * p, char ch) {
	TokenMapElement m;
	if(ch > TOKENMAP_NODESIZE -1)
		lexError("Unknown char", NULL, -1);
	if (p->possition) {
		m = p->possition[(int) ch];
		p->possition = m.next;
		return m.token;
	} else
		return t_empty;

}
void TokenMap_debug_rec(TokenMapElement map[], int indent,
		TokenMapElement * idsMap, TokenMapElement * intMap,
		TokenMapElement * realMap) {
	int i;
	int ii;
	TokenMapElement elm;

	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		elm = map[i];
		if (elm.token != t_empty || elm.next) {
			for (ii = 0; ii < indent; ii++)
				putchar(' ');

			printf("< %c(%d) , %s, next:", (char) i, i, getTokenStr(elm.token));
			if (elm.next == idsMap) {
				printf("idsMap>\n");
			} else if (elm.next == intMap) {
				printf("intMap>\n");
			} else if (elm.next == realMap) {
				printf("realMap>\n");
			} else {
				printf("%p>\n", elm.next);
				if (elm.next)
					TokenMap_debug_rec(elm.next, indent + 4, idsMap, intMap,
							realMap);
			}
		}
	}
}
void TokenMap_debug(TokenMapElement map[], int indent) {
	TokenMapElement * idsMap = map[(int) 'a'].next;
	TokenMapElement * intMap = map[(int) '1'].next;
	TokenMapElement * realMap = intMap[(int) '.'].next;
	TokenMap_debug_rec(map, 0, idsMap, intMap, realMap);

}
