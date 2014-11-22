#ifndef STACK_h
#define STACK_h

#include "stdlib.h"
#include "stdio.h"
#include "errorHandler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define IMPLEMENT_STACK(namePrefix, stackElementT)                             \
typedef struct namePrefix##s_stackNodeT {                                      \
	stackElementT data;                                                        \
	struct namePrefix##s_stackNodeT *next;                                     \
} namePrefix##StackNodeT;                                                      \
                                                                               \
typedef struct {                                                               \
	namePrefix##StackNodeT *top;                                               \
	int size;                                                                  \
} namePrefix##Stack;                                                           \
                                                                               \
                                                                               \
void namePrefix##Stack__init__(namePrefix##Stack * self) {                     \
	self->top = NULL;                                                          \
	self->size = 0;                                                            \
}                                                                              \
                                                                               \
void namePrefix##Stack_push(namePrefix##Stack * self, stackElementT data) {    \
	namePrefix##StackNodeT * tmp = malloc(sizeof(namePrefix##stackNodeT));     \
	if (!tmp)                                                                  \
		memoryError("Cano't allocate memory for stack ");                      \
	tmp->next = self->top;                                                     \
	tmp->data = data;                                                          \
	self->size ++;                                                             \
	self->top = tmp;                                                           \
}                                                                              \
                                                                               \
stackElementT namePrefix##Stack_pop(namePrefix##Stack * self) {                \
	stackElementT data;                                                        \
	namePrefix##StackNodeT * tmp = self->top;                                  \
	data = self->top->data;                                                    \
	self->top = self->top->next;                                               \
	free(tmp);                                                                 \
	return data;                                                               \
}                                                                              \
                                                                               \
stackElementT * namePrefix##Stack_getAt(namePrefix##Stack * self, int index) { \
	int i;                                                                     \
	int offset = self->size - index -1;                                        \
	namePrefix##StackNodeT * tmp = self->top;                                  \
                                                                               \
	for(i=0; i<offset; i++)                                                    \
		tmp= tmp->next;                                                        \
	return &(tmp->data);                                                       \
}                                                                              \
                                                                               \
/*\
void namePrefix##Stack__init__(Stack * self);          \
void namePrefix##Stack_push(Stack * s, stackElementT data) ;       \
stackElementT namePrefix##Stack_pop(Stack * s);                    \
stackElementT * namePrefix##Stack_getAt(Stack * self, int index);  \
*/

#endif
