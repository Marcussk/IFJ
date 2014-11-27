#ifndef IFUNCTION_h
#define IFUNCTION_h

#include "ifjTypes.h"
#include "instruction.h"

//list of parameters for function
typedef struct s_ParamsListItem {
	iVar * param;
	struct s_ParamsListItem * next;
} ParamsListItem;

typedef enum {
	b_none, b_readLn = 20, b_write, b_find,  b_copy, b_length, b_sort
} Builtins;

//first n iVars in
typedef struct s_iFunction{
	tIFJ retType;
	struct s_ParamsListItem * params;
	Instruction * body;
	Builtins builtin;
} iFunction;


iFunction * iFunction__init__();
void iFunction_addParam(iFunction * self, iVar * var);
void iFunction__dell__(iFunction * self);

#endif
