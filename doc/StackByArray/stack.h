#include "stdlib.h"
#include "stdio.h"
#include "errorHanler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define STACK_START_SIZE 64

typedef struct {
	iVal *StackArray; //iVal StackArray[prealocated];
	int top;
	int prealocated; 
} StackArr;

void StackArr__init__( StackArr *self );
void StackArr__dell__( StackArr * self );
void StackArr_push( StackArr *s, iVal data );
iVal StackArr_pop( StackArr *s );
iVal * StackArr_getAt( StackArr *self, int index );

void StackArr__init__( StackArr *self ){
	// ?? self->StackArray = malloc( STACK_MAX_SIZE * sizeof(struct iVal));
	//if( self->StackArray == NULL )
	//	memoryError("Can't allocate memory for Stack");
	self->top = 0;
	self->prealocated = STACK_START_SIZE; 
}

void StackArr_push( StackArr *s, iVal data ){
	if( s->top >= s->prealocated ){
		int newCapacity = s->prealocated *2;
		iVal *newArray = realloc(s->StackArray, newCapacity * sizeof(struct iVal));
		if( newArray == NULL )
			memoryError("Cano't reallocate memory for stack ");
		
		s->StackArray = newArray;
		s->prealocated = newCapacity;	 	
	} else{
		s->StackArray[s->top++] = data;
	}
}

iVal StackArr_pop( StackArr *s ){
	if(s->top == 0 ){
		free(s->StackArray);
		rt_error("Stack underflow");
	}
	iVal data = s->StackArray[s->top];
	s->top--;
	// ?? free(s->StackArray[s->top])
	return data;
}

iVal *StackArr_getAt(StackArr *self, int index){
	if( index <= 0  || index > self->prealocated ){
		free(self->StackArray);
		rt_error("Stack invalid access")
	}
	return( &(self->StackArray[index]) );
}

void StackArr__dell__( StackArr * self ){
	free( self->StackArray );
	self->top = 0;
	self->prealocated = STACK_START_SIZE; 
}
