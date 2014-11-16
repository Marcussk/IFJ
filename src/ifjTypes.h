#ifndef IFJ_TYPES_h
#define IFJ_TYPES_h

#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

// !!compatible!! with tokens
typedef enum {
	iUnknown, iVoid, iBool =15, iInt, iReal, iString, iChar, iFn, iAny
} tIFJ;

// alias function body
typedef int CodeStack;

//list of parameters for function
/*typedef struct {
	int size;
	struct iVar ** list;
} ParamsList;*/

//first n iVars in
typedef struct {
	tIFJ retType;
	int paramsCnt;
	struct iVar * scope;
	CodeStack * body;
} iFunction;

// basic variable can represet everythink, even function *
typedef struct {
	tIFJ type;
	char * name;
	bool isInitialied;
	union {
		float iReal;
		int iInt;
		char * iString;
		iFunction * fn;
	} val;
} iVar;

iVar * iVar__init__(char * name);
void iVar_debug(iVar * v);
void iVar__dell__(iVar * self);

char * iVar_type2str(tIFJ t);

#endif
