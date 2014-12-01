
#ifndef STACK_AS_ARRAY_h
#define STACK_AS_ARRAY_h


#include "stdlib.h"
#include "stdio.h"

#include "error_handler.h"
#include "ifj_types.h"
#include "instruction.h"


#define RANDOM_NUM 15
#define REAL_NUM 17
#define STACK_MAX_SIZE 8
#define IND 6

typedef struct {
	iVal *StackArray;
	int top;
	int prealocated; 
} StackArr;

void StackArr__init__( StackArr *self );
void StackArr__dell__( StackArr *self );
void StackArr_push( StackArr *self, iVal data );
iVal StackArr_pop( StackArr *self );
iVal * StackArr_getAt( StackArr *self, int index );

void int_StackArr_debug( StackArr *self, int index );
void test_stack1(void);

#endif
