#include "array_as_stack.h"

/*value self->top = -1 is not work be carfule using top value,*/
/* DONT FORGET DECREMENT */
void StackArr__init__(StackArr *self) {
	self->StackArray = malloc( STACK_MAX_SIZE * sizeof(iVal));
	if (self->StackArray == NULL)
		memoryError("Can't allocate memory for Stack");

	self->top = -1;
	self->prealocated = STACK_MAX_SIZE;
}

void StackArr_push(StackArr *self, iVal data) {
	if (self->top >= self->prealocated - 1) {
		int newCapacity = self->prealocated * 2;
		printf("here is problem\n");
		iVal *newArray = realloc(self->StackArray, newCapacity * sizeof(iVal));
		if (newArray == NULL)
			memoryError("Cano't reallocate memory for stack ");
		self->StackArray = newArray;
		self->prealocated = newCapacity;
		self->StackArray[self->top++] = data;
	} else {
		self->StackArray[self->top++] = data;
	}
}

iVal StackArr_pop(StackArr *self) {
	if (self->top <= 0) {
		free(self->StackArray);
		rt_error("Stack underflow");
	}
	iVal data = self->StackArray[self->top];
	self->top--;
	// ?? free(self->StackArray[self->top])
	return data;
}

iVal StackArr_getAt(StackArr *self, int index) {
	if (index > self->top) {
		free(self->StackArray);
		rt_error("Stack invalid access");
	}
	return self->StackArray[index];
}

/*When you dell Stack, and you want to use it again,
 dont forget init it again*/
void StackArr__dell__(StackArr * self) {
	free(self->StackArray);
	self->top = 0;
	self->prealocated = STACK_MAX_SIZE;
}

/*debug only for integers iVal*/
void int_StackArr_debug(StackArr *self) {
	int i;
	printf("Velkost stacku: %d \n ", self->prealocated);
	printf("Vypis poloziek staku:\n ");
	for (i = 0; i < self->top; i++) {
		if (i == self->top) {
			printf("top->");
		}
		printf("%d.pos -> %d \n ", i, (StackArr_getAt(self, i)).iInt);
	}
}
