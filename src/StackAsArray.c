#include "StackAsArray.h"


/*value self->top = -1 is not work be carfule using top value,*/
/* DONT FORGET DECREMENT */
void StackArr__init__( StackArr *self ){
	int i;
	self->StackArray = malloc( STACK_MAX_SIZE *sizeof(iVal));
	if( self->StackArray == NULL )
		memoryError("Can't allocate memory for Stack");

	self->top = 0;
	self->prealocated = STACK_MAX_SIZE; 
	for( i = 0; i < self->prealocated ; i++ ){
		self->StackArray[i].iInt = 0;
	}
}

void StackArr_push( StackArr *self, iVal data ){
	if( self->top >= self->prealocated ){
		int newCapacity = self->prealocated *2;
		iVal *newArray = realloc(self->StackArray, newCapacity * sizeof(iVal));
		if( newArray == NULL )
			memoryError("Cano't reallocate memory for stack ");
		
		self->StackArray = newArray;
		self->prealocated = newCapacity;	
		self->StackArray[self->top++] = data;	
	} else{
		self->StackArray[self->top++] = data;
	}
}

iVal StackArr_pop( StackArr *self ){
	if(self->top <= 0 ){
		free(self->StackArray);
		rt_error("Stack underflow");
	}
	iVal data = self->StackArray[self->top];
	self->top--;
	// ?? free(self->StackArray[self->top])
	return data;
}

iVal *StackArr_getAt(StackArr *self, int index){
	if( index < 0  || index >= self->prealocated ){
		free(self->StackArray);
		rt_error("Stack invalid access");
	}
	return( &(self->StackArray[index]) );
}

/*When you dell Stack, and you want to use it again,
 dont forget init it again*/
void StackArr__dell__( StackArr * self ){
	free( self->StackArray );
	self->top = -1;
	self->prealocated = STACK_MAX_SIZE; 
}

/*debug only for integers iVal*/
void int_StackArr_debug( StackArr *self, int index ){
	int i;
	iVal *ret_iVal;

	printf("__________________________________\n ");
	printf("Velkost stacku: %d \n ", self->prealocated );
	printf("TOP: %d -> %d \n ", self->top-1 , self->StackArray[self->top-1].iInt );
	printf("Vypis poloziek staku:\n ");
	for(i = 0 ;  i < self->prealocated; i++){
		printf("%d.pos -> %d \n ", i,  (self->StackArray[i]).iInt );
	}
	(ret_iVal) = StackArr_getAt( self, index );
	printf("Hodnota na indexe %d je %d \n", index, ret_iVal->iInt );

	printf("__________________________________\n ");

}
