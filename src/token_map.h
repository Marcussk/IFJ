#ifndef TOKEN_MAP_h
#define TOKEN_MAP_h

/*
 * in token map index is char
 * */
#include <stdlib.h>
#include <stdbool.h>

#include "error_handler.h"

#define STATIC_ARR_LEN(x)  (sizeof(x) / sizeof(x[0]))
#define TOKENMAP_NODESIZE 128

typedef enum {
	t_empty = -1, // nodes witch have no meaning are empty
	t_plus, // operators
	t_minus,
	t_asterisk,
	t_slash,
	t_less,
	t_greater,
	t_lessOrEqv,
	t_greaterOrEqv,
	t_eqv,
	t_notEqv,
	t_lParenthessis, //10
	t_rParenthessis,
	t_comma,
	t_eof,
	t_id,
	t_func,

	t_boolean=16,
	t_integer,
	t_real,
	t_string,
	t_char,

	t_begin, //keywords
	t_do,
	t_else,
	t_end,
	t_false,
	t_forward,

	t_if,
	t_then,
	t_true, 
	t_var, //30
	t_while,
	t_period,
	t_colon,
	t_scolon,
//	t_pointer,
//	t_lBracket,
//	t_rBracket,
	t_asigment,
//	t_doubleDot,
//	t_lcBracket, //others 40
//	t_rcBracket,
	t_num_int,
	t_num_real,
	t_str_val,
	t_invalid
} Token;

typedef struct {
	Token token;
	char * str;
} TokenMeaning;

typedef struct TokenMapElement TokenMapElement;
struct TokenMapElement {
	Token token;
	TokenMapElement * next;
};

typedef struct {
	TokenMapElement * map;
	TokenMapElement * possition;
} TokenParser;

TokenParser TokenParser__init__();
void TokenParser__dell__(TokenParser * p);
Token TokenParser_push(TokenParser * p, char ch);
void TokenParser_reset(TokenParser * p);

bool Token_isType(Token t);
bool Token_isKeyword(Token t);
bool Token_isValue(Token t);
bool Token_isOperator(Token t);
bool canContinueWithNonWordChar(Token t);

char * getTokenName(Token t);
void TokenMap_debug(TokenMapElement map[], int indent);

#endif
