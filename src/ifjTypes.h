#ifndef IFJ_TYPES_h
#define IFJ_TYPES_h

#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "errorHandler.h"

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
	iBool = 15,
	iInt,
	iReal,
	iString,
	iChar,
	iFn,
	iEnumerable,
	iGreedStop
} tIFJ;

// alias function body
typedef int CodeStack;

typedef union {
	float iReal;
	int iInt;
	char * iString;
	struct s_iFunction * fn;
} iVal;

//first n iVars in
typedef struct s_iFunction{
	tIFJ retType;
	struct s_ParamsListItem * params;
	CodeStack * body;
} iFunction;

// basic variable can represents everything, even function *
typedef struct s_iVar {
	tIFJ type;
	bool isInitialied;
	iVal val;
} iVar;

//list of parameters for function
typedef struct s_ParamsListItem {
	iVar * param;
	struct s_ParamsListItem * next;
} ParamsListItem;

iVar * iVar__init__();
void iVar_debug(iVar * v);
void iVar__dell__(iVar * self);

char * iVar_type2str(tIFJ t);

iFunction * iFunction__init__();
void iFunction_addParam(iFunction * self, iVar * var);
void iFunction__dell__(iFunction * self);

#endif
