#include "stdlib.h"
#include "stdio.h"
#include "errorHanler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define QUEUE_MAX_SIZE 128

typedef struct {
 	Instruction *QueueArr; //Instruction QueueArr[prealocated];
    int first;
    int last;
	int actual;
    int prealocated; 
} InstrQueue;

void InstrQueue__init__( InstrQueue *self );
Instruction* InstrQueue_next( InstrQueue* self );
void Instruction_insert( InstrQueue *self, Instruction i );
Instruction* InstrQueue_atIndex( InstrQueue* self, int index );
void InstrQueue__dell__( InstrQueue *self );
void InstrQueue__init__( InstrQueue *self ){
	// ?? self->QueueArr = malloc( QUEUE_MAX_SIZE * sizeof(struct Instruction));
	//if( self->QueueArr == NULL )
	//	memoryError("Can't allocate memory for QueueArr");
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_MAX_SIZE;
}

Instruction *InstrQueue_next( InstrQueue *self ){
	if( self->actual > sefl->prealocated ){
		//free(self->QueueArr);
		rt_error("Can't return next instruction , end of queue.");
	}
	else{
		self->actual++;
		return &(self->QueueArr[self->actual]);
	}
} 

void Instruction_insert( InstrQueue *self, Instruction i ){
	if( self->last > self->prealocated ){
		int newCapacity = self->prealocated *2;
		Instruction *newArray = realloc(self->QueueArr, newCapacity * sizeof(struct Instruction));
		if( newArray == NULL ){
			//free(self->QueueArr);
			memoryError("Can't reallocate memory for queue ");
		}
	} else {
		self->last++;
		self->QueueArr[self->last] = i;
		self->actual++;	
	}
}

Instruction* InstrQueue_atIndex( InstrQueue* self, int index ){
	if(index < self->first || index < self->prealocated ){
		free(self->QueueArr);
		rt_error("InstrQueue_atIndex out of index");
	}else 
		return( &(self->QueueArr[index] );
}

void InstrQueue__dell__( InstrQueue *self ){
	free(self->QueueArr);
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_MAX_SIZE;
}


