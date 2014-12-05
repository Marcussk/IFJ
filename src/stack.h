
#ifndef STACK_h
#define STACK_h

#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"
#include "instruction.h"


#define DECLARE_STACK(namePrefix, stackElementT) \
typedef struct { \
	stackElementT * StackArray; \
	int top; \
	int prealocated; \
} namePrefix##Stack; \
\
void namePrefix##Stack__init__( namePrefix##Stack *self );\
void namePrefix##Stack__dell__( namePrefix##Stack *self );\
void namePrefix##Stack_push( namePrefix##Stack *self, stackElementT data );\
stackElementT namePrefix##Stack_pop( namePrefix##Stack *self );\
stackElementT *namePrefix##Stack_getAt( namePrefix##Stack *self, int index )


#define IMPLEMENT_STACK(namePrefix, stackElementT) \
\
void namePrefix##Stack__init__(namePrefix##Stack *self) { \
	self->StackArray = malloc( 32 * sizeof(stackElementT));\
	if (self->StackArray == NULL) \
		memoryError("Can't allocate memory for Stack");\
\
	self->top = -1;\
	self->prealocated = 32;\
} \
\
void namePrefix##Stack_push(namePrefix##Stack *self, stackElementT data) { \
	if (self->top == self->prealocated - 1) { \
		int newCapacity = self->prealocated * 2;\
		stackElementT *newArray = realloc(self->StackArray, newCapacity * sizeof(stackElementT));\
		if (newArray == NULL) \
			memoryError("Cano't reallocate memory for stack ");\
		self->StackArray = newArray;\
		self->prealocated = newCapacity;\
	}\
	self->top++;\
	self->StackArray[self->top] = data;\
}\
\
stackElementT namePrefix##Stack_pop( namePrefix##Stack *self) { \
	stackElementT data;\
	if (self->top < 0) \
		rt_error("Stack underflow");\
\
	data = self->StackArray[self->top];\
	self->top--;\
	return data;\
}\
\
stackElementT *namePrefix##Stack_getAt(namePrefix##Stack *self, int index) { \
	if (index > self->top || index < 0) { \
		rt_error("Stack invalid access");\
	} \
	return &self->StackArray[index];\
} \
\
void namePrefix##Stack__dell__(namePrefix##Stack * self) {\
	free(self->StackArray);\
	self->top = -1;\
	self->prealocated = -1;\
}

/*void namePrefix##Stack_debug(namePrefix##Stack *self) {\
	int i;\
	printf("Velkost stacku: %d \n ", self->prealocated);\
	printf("Vypis poloziek staku:\n ");\
	for (i = 0; i <= self->top; i++) {\
		if (i == self->top) {\
			printf("top->");\
		}\
		printf("%d.pos -> %d \n ", i, (namePrefix##Stack_getAt(self, i)).iInt);\
	}\
}*/

#endif
