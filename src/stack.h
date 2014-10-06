#ifndef STACK_h
#define STACK_h

#include "stdlib.h"
#include "stdio.h"

typedef char stackElementT;

typedef struct {
	stackElementT *contents;
	int top;
	int maxSize;
} stackT;

void Stack__init__(stackT *stackP, int maxSize);
void Stack__dell__(stackT *stackP);
int Stack_IsEmpty(stackT *stackP);
int Stack_IsFull(stackT *stackP);
void Stack_Push(stackT *stackP, stackElementT element);
stackElementT Stack_Pop(stackT *stackP);

#endif
