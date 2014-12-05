#ifndef QUEUE_h
#define QUEUE_h

#include "stdlib.h"
#include "error_handler.h"
#include "defs.h"

#define QUEUE_DECLARE(namePref, elementT)                                     \
                                                                              \
typedef struct {                                                                \
	elementT *QueueArr;                                                         \
	int actual;                                                                 \
	int size;                                                                   \
	int prealocated;                                                            \
} namePref##Queue;                                                            \
                                                                                \
void namePref##Queue__init__(namePref##Queue *self);                        \
elementT* namePref##Queue_next(namePref##Queue* self);                      \
void namePref##Queue_insert(namePref##Queue *self, Instruction i);          \
elementT* namePref##Queue_atIndex(namePref##Queue* self, int index);

#define QUEUE_DEFINE(namePref, elementT)                                    \
void namePref##Queue__init__(namePref##Queue *self) {                     \
	self->QueueArr = malloc( 32 * sizeof(elementT));            \
	if (self->QueueArr == NULL)                                               \
		memoryError("Can't allocate memory for QueueArr");                    \
	self->size = 0;                                                           \
	self->actual = -1;                                                        \
	self->prealocated = 32;                                     \
}                                                                             \
                                                                              \
elementT* namePref##Queue_next(namePref##Queue* self) {                   \
	if (self->actual >= self->size - 1) {                                     \
		free(self->QueueArr);                                                 \
		rt_error("Can't return next instruction , end of queue.");            \
	}                                                                         \
	self->actual++;                                                           \
	return &(self->QueueArr[self->actual]);                                   \
}                                                                             \
                                                                              \
void namePref##Queue_insert(namePref##Queue *self, Instruction i) {       \
	if (self->size >= self->prealocated) {                                    \
		int newCapacity = self->prealocated * 2;                              \
		Instruction *newArray = realloc(self->QueueArr,                       \
				newCapacity * sizeof(Instruction));                           \
		if (newArray == NULL) {                                               \
			memoryError("Can't reallocate memory for queue ");                \
		}                                                                     \
		self->QueueArr = newArray;                                            \
		self->prealocated = newCapacity;                                      \
	}                                                                         \
	self->size++;                                                             \
	self->actual++;                                                           \
	self->QueueArr[self->actual] = i;                                         \
}                                                                             \
                                                                              \
elementT* namePref##Queue_atIndex(namePref##Queue* self, int index) {         \
	if (index < 0 || self->size - 1 < index)                                  \
		rt_error("InstrQueue_atIndex out of index");                          \
    self->actual = index;                                                     \
	return (&(self->QueueArr[self->actual]));                                 \
}

#endif
