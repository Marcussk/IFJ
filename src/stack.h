#ifndef STACK_h
#define STACK_h

#include "stdlib.h"
#include "stdio.h"
#include "errorHandler.h"
#include "instruction.h"

typedef Instruction stackElementT;

typedef struct stackTag {
	stackElementT data;
	struct stackTag *next;
} stackNodeT;

typedef struct {
	stackNodeT *top;
} Stack;

Stack * Stack__init__();
void Stack_push(Stack * s, stackElementT data) ;
stackElementT Stack_pop(Stack * s);
//void Stack_debug(Stack * s);

#endif
