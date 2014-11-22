#include "stack.h"

void Stack__init__(Stack * self) {
	self->top = NULL;
	self->size = 0;
}

void Stack_push(Stack * self, stackElementT data) {
	stackNodeT * tmp = malloc(sizeof(stackNodeT));
	if (!tmp)
		memoryError("Cano't allocate memory for stack ");
	tmp->next = self->top;
	tmp->data = data;
	self->size ++;
	self->top = tmp;
}

stackElementT Stack_pop(Stack * self) {
	stackElementT data;
	stackNodeT * tmp = self->top;
	data = self->top->data;
	self->top = self->top->next;
	free(tmp);
	return data;
}

stackElementT * Stack_getAt(Stack * self, int index) {
	int i;
	int offset = self->size - index -1;
	stackNodeT * tmp = self->top;

	for(i=0; i<offset; i++)
		tmp= tmp->next;
	return &(tmp->data);
}

/*
 void Stack_debug(Stack * s) {
 stackNodeT *temp = s->top;
 if (temp)
 while (temp != NULL) {
 printf("%p\n", temp->data);
 temp = temp->next;
 }
 else {
 printf("\nThe stack is empty!");
 }
 }
 */
