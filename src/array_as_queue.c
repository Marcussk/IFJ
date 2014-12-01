
#include "array_as_queue.h"

void InstrQueueArr__init__(InstrQueueArr *self) {
	self->QueueArr = malloc( QUEUE_START_SIZE * sizeof(Instruction));
	if( self->QueueArr == NULL )
		memoryError("Can't allocate memory for QueueArr");
	self->first = 0;
	self->last = 0;
	self->actual = 0;
	self->prealocated = QUEUE_START_SIZE;
}

Instruction *InstrQueueArr_next(InstrQueueArr *self) {
	if (self->actual > self->prealocated) {
		free(self->QueueArr);
		rt_error("Can't return next instruction , end of queue.");
	} else {
		self->actual++;
		return &(self->QueueArr[self->actual]);
	}
}

void InstrQueueArr_insert(InstrQueueArr *self, Instruction i) {
	if (self->last > self->prealocated) {
		int newCapacity = self->prealocated * 2;
		Instruction *newArray = realloc(self->QueueArr,	newCapacity * sizeof(Instruction));
		if (newArray == NULL) {
			free(self->QueueArr);
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

void InstrQueue_debud(InstrQueueArr *self){
	Instruction instr;
	int i;

	for(i = self->first ; i=self->last; i++){
		printf("Actual: %d  \n", self->actual);
		printf("index %d. code %s, type %d, a1 %p, a2 %p, dest %p \n", i, instr2Str(instr.code),
				instr.type, instr.a1, instr.a2, instr.dest );
	}

}

void test_queue1(void){
	Instruction ins = {.code=i_push};

}


/*

void test_stack2(void){
	printf("Test2 stared ..................\n");
	Stack s; 
	iVal new_iVal, next_iVal, iVal_1,  ret_iVal;
	new_iVal.iInt = RANDOM_NUM;
	next_iVal.iInt = REAL_NUM;
	iVal_1.iInt = 42; 

	Stack__init__(&s);

	Stack_push(&s, next_iVal );
	Stack_push(&s, new_iVal );
	Stack_push(&s, next_iVal );
	Stack_push(&s, new_iVal );
	Stack_push(&s, next_iVal );
	Stack_push(&s, next_iVal );

	int_Stack_debug(&s, IND);

	ret_iVal = Stack_pop(&s);
	ret_iVal = Stack_pop(&s);
	ret_iVal = Stack_pop(&s);
	ret_iVal = Stack_pop(&s);
	int_Stack_debug(&s, IND);

	Stack_push(&s, next_iVal );
	Stack_push(&s, next_iVal );

	int_Stack_debug(&s, IND);

	Stack__dell__(&s);
}
*/
/*Test of functionality for array stack*/
