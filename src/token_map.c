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
		(TokenMeaning ) { t_forward, "forward" },
		(TokenMeaning ) { t_if,	"if" },
		(TokenMeaning ) { t_integer, "integer" },
		(TokenMeaning ) { t_real, "real" },
		(TokenMeaning ) { t_char, "char" },
		(TokenMeaning ) { t_string, "string" },
		(TokenMeaning ) { t_then, "then" },
		(TokenMeaning ) { t_true, "true" },
		(TokenMeaning ) { t_var, "var" },
		(TokenMeaning ) { t_while, "while" },
		//
		(TokenMeaning ) { t_repeat, "repeat"},
		(TokenMeaning ) { t_until, "until"},
		//
		(TokenMeaning ) { t_plus, "+" }, // operators
		(TokenMeaning ) { t_minus, "-" },
		(TokenMeaning ) { t_asterisk, "*" },
		(TokenMeaning ) { t_slash, "/" },
		(TokenMeaning ) { t_eqv, "=" },
		(TokenMeaning ) { t_less, "<" },
		(TokenMeaning ) { t_greater, ">" },
		//(TokenMeaning ) { t_lBracket, "[" },
		//(TokenMeaning ) { t_rBracket, "]" },
		(TokenMeaning ) { t_period, "." },
		(TokenMeaning ) { t_comma, "," },
		(TokenMeaning ) { t_colon, ":" },
		(TokenMeaning ) { t_scolon, ";" },
		//(TokenMeaning ) { t_pointer, "^" },
		(TokenMeaning ) { t_lParenthessis, "(" },
		(TokenMeaning ) { t_rParenthessis, ")" },
		(TokenMeaning ) { t_notEqv, "<>" },
		(TokenMeaning ) { t_lessOrEqv, "<=" },
		(TokenMeaning ) { t_greaterOrEqv, ">=" },
		(TokenMeaning ) { t_asigment, ":=" }
		//(TokenMeaning ) { t_doubleDot, ".." },
	    //(TokenMeaning ) { t_lcBracket, "{" }, //others
		//(TokenMeaning ) { t_rcBracket, "}" }
		};

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
bool Token_isOperator(Token t) {
	return (t >= t_plus && t <= t_notEqv);
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
