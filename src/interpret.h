#ifndef INTERPRET_h
#define INTERPRET_h

#include "instruction.h"
#include "stack.h"
#include "builtins.h"
#include "errorHandler.h"


typedef struct  {
	InstrQueue instructions;
	Stack stack;
} Interpret;

void Interpret__init__(Interpret * self, Stack instructions);
void Interpret_run(Interpret * self);


#endif
