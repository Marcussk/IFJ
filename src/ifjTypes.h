#ifndef IFJ_TYPES_h
#define IFJ_TYPES_h

#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

typedef enum {
	iUnknown, iVoid, iInt =25, iString=29, iReal =27, iFn
} tIFJ;

// alias function body
typedef int CodeStack;

//list of parameters for function
typedef struct {
	int size;
	struct iVar ** list;
} ParamsList;

typedef struct {
	tIFJ retType;
	ParamsList params;
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

#endif
