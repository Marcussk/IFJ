#ifndef IFUNCTION_h
#define IFUNCTION_h

#include "ifj_types.h"
#include "instruction.h"

//list of parameters for function
typedef struct s_ParamsListItem {
	iVar * data;
	char * name;
	struct s_ParamsListItem * next;
	struct s_ParamsListItem * prev;
} ParamsListItem;

typedef struct {
	int size;
	ParamsListItem * First;
	ParamsListItem * Last;
} ParamsList;

typedef enum {
	b_none, b_readLn = 21, b_write, b_find, b_copy, b_length, b_sort
} Builtins;

typedef struct s_iFunction {
	bool forwardFound;
	bool bodyFound;
	iVar retVal;
	char * name;
	ParamsList params;
	InstrParam * bodyInstrIndex;
	Builtins builtin;
} iFunction;

iFunction * iFunction__init__();
void iFunction_buildParamIndexes(iFunction * self);
void iFunction_addParam(iFunction * self, iVar * var, char * name);
void iFunction__dell__(iFunction * self);

#endif
