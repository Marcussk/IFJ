#include "stack.h"

Stack * Stack__init__(){
	Stack * s = malloc(sizeof(Stack));
	s->top = NULL;
	return s;
}

void Stack_push(Stack * s, stackElementT data) {
	stackNodeT * tmp = malloc(sizeof(stackNodeT));
	if (!tmp)
		memoryError("Cano't allocate memory for stack ");
	tmp->next = s->top;
	tmp->data = data;
	s->top = tmp;
}

stackElementT Stack_pop(Stack * s) {
	stackElementT data;
	stackNodeT * tmp = s->top;
	data = s->top->data;
	s->top = s->top->next;
	free(tmp);
	return data;
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
