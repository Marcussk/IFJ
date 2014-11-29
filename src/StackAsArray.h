#include "stdlib.h"
#include "stdio.h"
#include "errorHandler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define RANDOM_NUM 15
#define REAL_NUM 17
#define STACK_MAX_SIZE 8
#define IND 6

typedef struct {
	iVal *StackArray;
	signed int top;
	int prealocated; 
} Stack;

void Stack__init__( Stack *self );
void Stack__dell__( Stack *self );
void Stack_push( Stack *self, iVal data );
iVal Stack_pop( Stack *self );
iVal * Stack_getAt( Stack *self, int index );

void int_Stack_debug( Stack *self, int index );