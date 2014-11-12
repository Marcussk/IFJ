#include "tokenMap.h"

//describes string form of token
TokenMeaning tokenMeanings[] = {
		(TokenMeaning ) { t_begin, "begin" }, //keywords
		(TokenMeaning ) { t_boolean, "boolean" },
		(TokenMeaning ) { t_do, "do" },
		(TokenMeaning ) { t_else, "else" },
		(TokenMeaning ) { t_end, "end" },
		(TokenMeaning ) { t_false,"false" },
		(TokenMeaning ) { t_find, "find" },
		(TokenMeaning ) { t_forward, "forward" },
		(TokenMeaning ) { t_func, "function" },
		(TokenMeaning ) { t_if,"if" },
		(TokenMeaning ) { t_integer,"integer" },
		(TokenMeaning ) { t_readln, "readln" },
		(TokenMeaning ) { t_real, "real" },
		(TokenMeaning ) { t_sort,"sort" },
		(TokenMeaning ) { t_string, "string" },
		(TokenMeaning ) { t_then,"then" },
		(TokenMeaning ) { t_true, "true" },
		(TokenMeaning ) { t_var, "var" },
		(TokenMeaning ) { t_while,"while" },
		(TokenMeaning ) { t_write, "write" },
		(TokenMeaning ) { t_plus, "+" }, // operators
		(TokenMeaning ) { t_minus, "-" },
		(TokenMeaning ) { t_asterisk, "*" },
		(TokenMeaning ) { t_slash, "/" },
		(TokenMeaning ) { t_eqv, "=" },
		(TokenMeaning ) { t_less, "<" },
		(TokenMeaning ) { t_greater, ">" },
		(TokenMeaning ) { t_lBracket,"[" },
		(TokenMeaning ) { t_rBracket, "]" },
		(TokenMeaning ) { t_period, "." },
		(TokenMeaning ) { t_comma, "," },
		(TokenMeaning ) { t_colon, ":" },
		(TokenMeaning ) { t_scolon, ";" },
		(TokenMeaning ) { t_pointer,"^" },
		(TokenMeaning ) { t_lParenthessis, "(" },
		(TokenMeaning ) { t_rParenthessis, ")" },
		(TokenMeaning ) { t_notEqv, "<>" },
		(TokenMeaning ) { t_lessOrEqv,"<=" },
		(TokenMeaning ) { t_greaterOrEqv, ">=" },
		(TokenMeaning ) { t_asigment, ":=" },
		(TokenMeaning ) { t_doubleDot, ".." },
		(TokenMeaning ) { t_lcBracket, "{" } , //others
		(TokenMeaning ) { t_rcBracket, "}"}
		};

// alias can be double char operator?
bool canContinueWithNonWordChar(Token t) {
	return t == t_less || t == t_greater || t == t_period || t == t_colon;
}

// reverz function, can get string form of token
char * getTokenStr(Token t) {
	int ti;
	int tokensCnt = STATIC_ARR_LEN(tokenMeanings);
	for (ti = 0; ti < tokensCnt; ti++) {
		TokenMeaning tm = tokenMeanings[ti];
		if (tm.token == t)
			return tm.str;
	}
	if (t == t_empty)
		return "<EmptyToken>";
	return NULL;
}

TokenMapElement * TokenMap_newLevel(){
	int i;
	TokenMapElement * newArr= malloc(TOKENMAP_NODESIZE* sizeof(TokenMapElement));
	//[TODO] check allocation
	for(i=0; i<TOKENMAP_NODESIZE; i++){
		newArr[i].next = NULL;
		newArr[i].token = t_empty;
	}
	return newArr;
}

// include token rule in token map
TokenMapElement * TokenMap_add(TokenMapElement map[], char * str, Token t) {
	int i;
	TokenMapElement * lastElm;
	char ch;
	for (i = 0; str[i]; i++) { //foreach char
		ch = str[i];
		if (str[i + 1]) { // if is not last char
			lastElm =  &(map[(int) ch]);
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
void TokenParser_addNumbers(TokenMapElement map[]) {
	int i;
	TokenMapElement * elm;
	TokenMapElement * iElm;
	TokenMapElement * rElm;
	TokenMapElement * intMap = TokenMap_newLevel();
	TokenMapElement * realMap = TokenMap_newLevel();

	for (i = '0'; i <= '9'; i++) {
		elm = &(map[i]);
		iElm = &(intMap[i]);
		rElm = &(realMap[i]);

		elm->token = t_num_int;
		elm->next = intMap;

		iElm->token = t_num_int;
		iElm->next = intMap;

		rElm->token = t_num_real;
		rElm->next = realMap;
	}
	intMap['.'].token = t_num_real;
	intMap['.'].next = realMap;
}

TokenParser TokenParser__init__() {
	int ti;
	TokenParser p;
	p.map = TokenMap_newLevel();
	TokenMapElement * map = p.map;
	int tokensCnt = STATIC_ARR_LEN(tokenMeanings);
	for (ti = 0; ti < tokensCnt; ti++) {
		TokenMeaning tm = tokenMeanings[ti];
		if (!TokenMap_add(map, tm.str, tm.token)) {
			memoryError("could't allocate token map");
		}
	}
	p.possition = p.map;
	TokenParser_addNumbers(p.map);
	return p;
}

void TokenMap__dell__(TokenMapElement * map) {
	int i;
	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		TokenMapElement m = map[i];
		if (m.next)
			TokenMap__dell__(m.next);
	}
	free(map);
}

void TokenParser__dell__(TokenParser * p) {
	int i;
	TokenMapElement map;
	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		map = p->map[i];
		if (map.token != t_num_int && map.next)
			TokenMap__dell__(map.next);
	}
	TokenMapElement * intMap = p->map['0'].next;
	TokenMapElement * realMap = intMap['.'].next;
	free(intMap);
	free(realMap);

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
	if (p->possition)
		m = p->possition[(int) ch];
	else
		m = p->map[(int) ch];

	p->possition = m.next;

	return m.token;
}

void TokenMap_debug(TokenMapElement map[], int indent) {
	int i;
	int ii;
	TokenMapElement elm;

	for (i = 0; i < TOKENMAP_NODESIZE; i++) {
		if (i < '0' || i > '9') { // skip numbers because cyclic structure
			elm = map[i];
			if (elm.token != t_empty || elm.next) {
				for (ii = 0; ii < indent; ii++)
					putchar(' ');
				printf("< %c(%d) , %s, next: %p>\n", (char) i, i,
						getTokenStr(elm.token), elm.next);
				if (elm.next)
					TokenMap_debug(elm.next, indent + 4);
			}
		}
	}
}
