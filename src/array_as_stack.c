#include "array_as_stack.h"

void StackArr__init__(StackArr *self) {
	self->StackArray = malloc( STACK_MAX_SIZE * sizeof(iVal));
	if (self->StackArray == NULL)
		memoryError("Can't allocate memory for Stack");

	self->top = -1;
	self->prealocated = STACK_MAX_SIZE;
}

void StackArr_push(StackArr *self, iVal data) {
	if (self->top == self->prealocated - 1) {
		int newCapacity = self->prealocated * 2;
		iVal *newArray = realloc(self->StackArray, newCapacity * sizeof(iVal));
		if (newArray == NULL)
			memoryError("Cano't reallocate memory for stack ");
		self->StackArray = newArray;
		self->prealocated = newCapacity;
	}
	self->top++;
	self->StackArray[self->top] = data;
}

iVal StackArr_pop(StackArr *self) {
	iVal data;
	if (self->top < 0)
		rt_error("Stack underflow");

	data = self->StackArray[self->top];
	self->top--;
	return data;
}

iVal StackArr_getAt(StackArr *self, int index) {
	if (index > self->top || index < 0) {
		rt_error("Stack invalid access");
	}
	return self->StackArray[index];
}

void StackArr__dell__(StackArr * self) {
	free(self->StackArray);
	self->top = 0;
	self->prealocated = STACK_MAX_SIZE;
}

void int_StackArr_debug(StackArr *self) {
	int i;
	printf("Velkost stacku: %d \n ", self->prealocated);
	printf("Vypis poloziek staku:\n ");
	for (i = 0; i <= self->top; i++) {
		if (i == self->top) {
			printf("top->");
		}
		printf("%d.pos -> %d \n ", i, (StackArr_getAt(self, i)).iInt);
	}
}
