
#ifndef STACK_AS_ARRAY_h
#define STACK_AS_ARRAY_h


#include "stdlib.h"
#include "stdio.h"

#include "error_handler.h"
#include "ifj_types.h"
#include "instruction.h"


#define STACK_MAX_SIZE 32

#define DECLARE_STACK(namePrefix, stackElementT)

typedef struct {
	stackElementT *StackArray;
	int top;
	int prealocated; 
} namePrefix##StackArr;

void namePrefix##StackArr__init__( namePrefix##StackArr *self );
void namePrefix##StackArr__dell__( namePrefix##StackArr *self );
void namePrefix##StackArr_push( namePrefix##StackArr *self, stackElementT data );
stackElementT namePrefix##StackArr_pop( namePrefix##StackArr *self );
stackElementT namePrefix##StackArr_getAt( namePrefix##StackArr *self, int index );
void int_namePrefix##StackArr_debug( namePrefix##StackArr *self);

#endif
