#ifndef EXPR_h
#define EXPR_h

#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"
#include "lex_parser.h"
#include "parser_tables.h"
#include "stack.h"
#include "str_routines.h"
#include "token_buff.h"
#include "token_map.h"
#include "expr_token.h"

//#define EXPR_DEBUG

DECLARE_STACK(expr, ExprToken);

tIFJ expression(TokenBuff * tokenBuff, InstrQueue * istructions);

#endif
