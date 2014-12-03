#include "array_as_queue.h"

void InstrQueueArr__init__(InstrQueueArr *self) {
	self->QueueArr = malloc( QUEUE_START_SIZE * sizeof(Instruction));
	if (self->QueueArr == NULL)
		memoryError("Can't allocate memory for QueueArr");
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_START_SIZE;
}

Instruction *InstrQueueArr_next(InstrQueueArr *self) {
	if (self->actual >= self->last - 1) {
		free(self->QueueArr);
		rt_error("Can't return next instruction , end of queue.");
	}
	self->actual++;
	return &(self->QueueArr[self->actual]);
}

void InstrQueueArr_insert(InstrQueueArr *self, Instruction i) {
	if (self->last >= self->prealocated - 1) {
		int newCapacity = self->prealocated * 2;
		Instruction *newArray = realloc(self->QueueArr,
				newCapacity * sizeof(Instruction));
		if (newArray == NULL) {
			free(self->QueueArr);
			memoryError("Can't reallocate memory for queue ");
		}
		self->QueueArr = newArray;
		self->prealocated = newCapacity;

	}
	self->last++;
	self->QueueArr[self->last] = i;
	self->actual++;

}

Instruction* InstrQueueArr_atIndex(InstrQueueArr* self, int index) {
	if (index >= self->last - 1) {
		free(self->QueueArr);
		rt_error("InstrQueue_atIndex out of index");
	}
	return (&(self->QueueArr[index]));
}

void InstrQueueArr__dell__(InstrQueueArr *self) {
	free(self->QueueArr);
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_START_SIZE;
}

/* Debugging function, use in troubles */
void InstrQueueArr_debug(InstrQueueArr *self, int index) {
	int i;
	Instruction * ins;

	printf("Max queue size: %d: \n", self->prealocated);
	printf("First is : %d, Last is: %d \n", self->first, self->last);
	printf("Actual: %d  \n", self->actual);
	printf("_____________________________________________________________\n");
	for (i = self->first; i <= self->last; i++) {
		printf("index %d. code %s, type %d, a1 %p, a2 %p, dest %p \n", i,
				instr2Str((self->QueueArr[i]).code),
				(int) (self->QueueArr[i]).type, (void *) (self->QueueArr[i]).a1,
				(void *) (self->QueueArr[i]).a2,
				(void *) (self->QueueArr[i]).dest);
	}
	printf("_____________________________________________________________\n");
	printf("Finding instruction at index %d\n", index);
	ins = InstrQueueArr_atIndex(self, index);

	printf("index %d. code %s, type %d, a1 %p, a2 %p, dest %p \n", index,
			instr2Str(ins->code), (int) ins->type, (void *) ins->a1,
			(void *) ins->a2, (void *) ins->dest);
	printf("_____________________________________________________________\n");
}

