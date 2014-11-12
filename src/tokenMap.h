#ifndef TOKEN_MAP_h
#define TOKEN_MAP_h

/*
 * in token map index is char
 * */
#include <stdlib.h>
#include <stdbool.h>
#include "errorHandler.h"

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
	t_lBracket,
	t_rBracket,
	t_comma,
	t_eof,
	t_id,

	t_begin=15, //keywords
	t_boolean,
	t_do,
	t_else,
	t_end,
	t_false,
	t_find,
	t_forward,
	t_func,
	t_if,
	t_integer,
	t_readln,
	t_real,
	t_sort,
	t_string,
	t_then,
	t_true,
	t_var,
	t_while,
	t_write,

	t_period,
	t_colon,
	t_scolon,
	t_pointer,
	t_lParenthessis,
	t_rParenthessis,
	t_notEqv,
	t_asigment,
	t_doubleDot,
	t_lcBracket, //others
	t_rcBracket,
	t_num_int,
	t_num_real



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

bool isKeyword(Token t);
bool canContinueWithNonWordChar(Token t);

char * getTokenStr(Token t);
void TokenMap_vizualize(TokenMapElement * map, int indent);


#endif
