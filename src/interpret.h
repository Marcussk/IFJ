#ifndef INTERPRET_h
#define INTERPRET_h

#include "instruction.h"
#include "stack.h"
#include "builtins.h"
#include <stdbool.h>
#include "error_handler.h"

DECLARE_STACK(i, sVal)

typedef struct  {
	InstrQueue instructions;
	iStack stack;
} Interpret;

void Interpret__init__(Interpret * self, InstrQueue instructions);
void Interpret_run(Interpret * self);
void Interpret__dell__(Interpret * self);

void Interpret_test3();

#endif
