#include "stdlib.h"
#include "stdio.h"
#include "errorHanler.h"
#include "instruction.h"
#include "ifjTypes.h"


#define QUEUE_START_SIZE 128

typedef struct {
	Instruction *QueueArr; //Instruction QueueArr[prealocated];
	int first;
	int last;
	int actual;
	int prealocated;
} InstrQueueArr;

void InstrQueueArr__init__(InstrQueueArr *self);
Instruction* InstrQueueArr_next(InstrQueueArr* self);
void InstrQueueArr_insert(InstrQueueArr *self, Instruction i);
Instruction* InstrQueueArr_atIndex(InstrQueueArr* self, int index);
void InstrQueueArr__dell__(InstrQueueArr *self);

void InstrQueueArr__init__(InstrQueueArr *self) {
	// ?? self->QueueArr = malloc( QUEUE_MAX_SIZE * sizeof(struct Instruction));
	//if( self->QueueArr == NULL )
	//	memoryError("Can't allocate memory for QueueArr");
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_START_SIZE;
}

Instruction *InstrQueueArr_next(InstrQueueArr *self) {
	if (self->actual > self->prealocated) {
		//free(self->QueueArr);
		rt_error("Can't return next instruction , end of queue.");
	} else {
		self->actual++;
		return &(self->QueueArr[self->actual]);
	}
}

void InstrQueueArr_insert(InstrQueueArr *self, Instruction i) {
	if (self->last > self->prealocated) {
		int newCapacity = self->prealocated * 2;
		Instruction *newArray = realloc(self->QueueArr,
				newCapacity * sizeof(struct Instruction));
		if (newArray == NULL) {
			//free(self->QueueArr);
			memoryError("Can't reallocate memory for queue ");
		}
	} else {
		self->last++;
		self->QueueArr[self->last] = i;
		self->actual++;
	}
}

Instruction* InstrQueueArr_atIndex(InstrQueueArr* self, int index) {
	if (index < self->first || index < self->prealocated) {
		free(self->QueueArr);
		rt_error("InstrQueue_atIndex out of index");
	} else
		return (&(self->QueueArr[index]));
}

void InstrQueueArr__dell__(InstrQueueArr *self) {
	free(self->QueueArr);
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_START_SIZE;
}
