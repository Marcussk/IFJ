#ifndef ARRAY_AS_QUEUE_h
#define ARRAY_AS_QUEUE_h

#include "stdlib.h"
#include "stdio.h"

#include "ifj_types.h"
#include "error_handler.h"
#include "instruction.h"


#define QUEUE_START_SIZE 32

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
void InstrQueueArr_debud(InstrQueueArr *self, int index); 

#endif