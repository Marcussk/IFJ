#ifndef IFUNCTION_h
#define IFUNCTION_h

#include "ifj_types.h"
#include "instruction.h"

//list of parameters for function
typedef struct s_ParamsListItem {
	iVar * data;
	struct s_ParamsListItem * next;
	struct s_ParamsListItem * prev;
} ParamsListItem;

typedef struct {
	ParamsListItem * First;
	ParamsListItem * Last;
} ParamsList;

typedef enum {
	b_none, b_readLn = 20, b_write, b_find,  b_copy, b_length, b_sort
} Builtins;

//first n iVars in
typedef struct s_iFunction{
	iVar retVal;
	ParamsList params;
	int bodyInstrIndex;
	Builtins builtin;
} iFunction;


iFunction * iFunction__init__();
void iFunction_buildParamIndexes(iFunction * self);
void iFunction_addParam(iFunction * self, iVar * var);
void iFunction__dell__(iFunction * self);

#endif
