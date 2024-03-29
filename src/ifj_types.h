#ifndef IFJ_TYPES_h
#define IFJ_TYPES_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error_handler.h"
#include "token_map.h"

/*
 * !!compatible!! with tokens
 *
 * if iVoid is used in fn. params any type is allowed (used for example for readln)
 * IEnumerable is used for expandable parameters like in write
 * iGreedStop is used after last parameter with argument of iEnumerable
 *
 * */
typedef enum {
	iUnknown =0,
	iVoid,
	iBool = 16,
	iInt,
	iReal,
	iString,
	iFn,
	iStackRef,
	iStackGRef
} tIFJ;

typedef union {
	float iReal;
	int iInt;
	char * iString;
	struct s_iFunction * fn;
} iVal;

typedef struct s_sVal {
	bool isInitialized;
	iVal val;
} sVal;

// basic variable can represents everything, even function *
typedef struct s_iVar {
	tIFJ type;
	bool isGlobal;
	int stackIndex;
	iVal val;
} iVar;


iVar * iVar__init__();
void iVar_debug(iVar * v);
void iVar__dell__(iVar * self);
tIFJ Token_getType(Token t);
char * iVar_type2str(tIFJ t);

#endif
