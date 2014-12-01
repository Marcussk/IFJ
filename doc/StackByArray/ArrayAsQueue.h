#include "stdlib.h"
#include "stdio.h"

#include "../../src/ifj_types.h"
#include "errorHanler.h"
#include "instruction.h"


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
