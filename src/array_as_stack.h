
#ifndef STACK_AS_ARRAY_h
#define STACK_AS_ARRAY_h


#include "stdlib.h"
#include "stdio.h"

#include "error_handler.h"
#include "ifj_types.h"
#include "instruction.h"


#define STACK_MAX_SIZE 32


typedef struct {
	iVal *StackArray;
	int top;
	int prealocated; 
} StackArr;

void StackArr__init__( StackArr *self );
void StackArr__dell__( StackArr *self );
void StackArr_push( StackArr *self, iVal data );
iVal StackArr_pop( StackArr *self );
iVal StackArr_getAt( StackArr *self, int index );
void int_StackArr_debug( StackArr *self);

#endif
