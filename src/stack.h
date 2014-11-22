#ifndef STACK_h
#define STACK_h

#include "stdlib.h"
#include "stdio.h"
#include "errorHandler.h"
#include "instruction.h"

typedef Instruction stackElementT;

typedef struct s_stackNodeT {
	stackElementT data;
	struct s_stackNodeT *next;
} stackNodeT;

typedef struct {
	stackNodeT *top;
	int size;
} Stack;

void Stack__init__(Stack * self);
void Stack_push(Stack * s, stackElementT data) ;
stackElementT Stack_pop(Stack * s);
stackElementT * Stack_getAt(Stack * self, int index);
//void Stack_debug(Stack * s);

#endif
