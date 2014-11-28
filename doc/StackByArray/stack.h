#include "stdlib.h"
#include "stdio.h"
#include "errorHanler.h"
#include "instruction.h"
#include "ifjTypes.h"

#define STACK_MAX_SIZE 64

typedef struct {
	iVal *StackArray; //iVal StackArray[prealocated];
	int top;
	int prealocated; 
} Stack;

void Stack__init__( Stack *self );
void Stack__dell__( Stack * self );
void Stack_push( Stack *s, iVal data );
iVal Stack_pop( Stack *s );
iVal * Stack_getAt( Stack *self, int index );

void Stack__init__( Stack *self ){
	// ?? self->StackArray = malloc( STACK_MAX_SIZE * sizeof(struct iVal));
	//if( self->StackArray == NULL )
	//	memoryError("Can't allocate memory for Stack");
	self->top = 0;
	self->prealocated = STACK_MAX_SIZE; 
}

void Stack_push( Stack *s, iVal data ){
	if( s->top >= prealocated ){
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

iVal Stack_pop( Stack *s ){
	if(s->top == 0 ){
		free(s->StackArray);
		rt_error("Stack underflow");
	}
	iVal data = s->StackArray[s->top];
	s->top--;
	// ?? free(s->StackArray[s->top])
	return data;
}

iVal *Stack_getAt(Stack *self, int index){
	if( index <= 0  || index > prealocated ){
		free(s->StackArray);
		rt_error("Stack invalid access")
	}
	return( &(self->StackArray[index]) );
}

void Stack__dell__( Stack * self ){
	free( self->StackArray );
	self->top = 0;
	self->prealocated = STACK_MAX_SIZE; 
}