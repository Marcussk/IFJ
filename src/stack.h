#ifndef STACK_h
#define STACK_h

#include <stdlib.h>
#include <stdio.h>
#include "errorHandler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define DECLARE_STACK(namePrefix, stackElementT)                               \
typedef struct namePrefix##s_stackNodeT {                                      \
	stackElementT data;                                                        \
	struct namePrefix##s_stackNodeT *next;                                     \
} namePrefix##StackNodeT;                                                      \
                                                                               \
typedef struct {                                                               \
	namePrefix##StackNodeT *top;                                               \
	int size;                                                                  \
	int actualIndex;                                                           \
} namePrefix##Stack;                                                           \
void namePrefix##Stack__init__(namePrefix##Stack * self);                      \
void namePrefix##Stack_push(namePrefix##Stack * s, stackElementT data) ;       \
stackElementT namePrefix##Stack_pop(namePrefix##Stack * s);                    \
stackElementT * namePrefix##Stack_getAt(namePrefix##Stack * self, int index);  \


#define IMPLEMENT_STACK(namePrefix, stackElementT)                             \
                                                                               \
void namePrefix##Stack__init__(namePrefix##Stack * self) {                     \
	self->top = NULL;                                                          \
	self->size = 0;                                                            \
	self->actualIndex = 0;                                                     \
}                                                                              \
                                                                               \
void namePrefix##Stack_push(namePrefix##Stack * self, stackElementT data) {    \
	namePrefix##StackNodeT * tmp = malloc(sizeof(namePrefix##StackNodeT));     \
	if (!tmp)                                                                  \
		memoryError("Cano't allocate memory for stack ");                      \
	tmp->next = self->top;                                                     \
	tmp->data = data;                                                          \
	self->size ++;                                                             \
	self->top = tmp;                                                           \
    self->actualIndex ++;                                                      \
}                                                                              \
                                                                               \
stackElementT namePrefix##Stack_pop(namePrefix##Stack * self) {                \
	stackElementT data;                                                        \
	namePrefix##StackNodeT * tmp = self->top;                                  \
    if(!tmp) rt_error("Stack underflow");                                      \
	data = self->top->data;                                                    \
	self->top = self->top->next;                                               \
	self->size --;                                                             \
	self->actualIndex --;                                                      \
    free(tmp);                                                                 \
	return data;                                                               \
}                                                                              \
                                                                               \
stackElementT * namePrefix##Stack_getAt(namePrefix##Stack * self, int index) { \
	int i;                                                                     \
	namePrefix##StackNodeT * tmp = self->top;                                  \
    if(index >= self->size)  rt_error("Stack invalid access");                 \
	for(i=self->size -1; i>index; i--)                                         \
		tmp= tmp->next;                                                        \
	return &(tmp->data);                                                       \
}                                                                              \


#endif
