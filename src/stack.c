#include "stack.h"

void Stack__init__(stackT *stackP, int maxSize) {
	stackElementT *newContents;
	newContents = (stackElementT *) malloc(sizeof(stackElementT) * maxSize);

	if (newContents == NULL) {
		fprintf(stderr, "Insufficient memory to initialize stack.\n");
		exit(1);
	}

	stackP->contents = newContents;
	stackP->maxSize = maxSize;
	stackP->top = -1;
}

void Stack__dell__(stackT *stackP) {
	free(stackP->contents);

	stackP->maxSize = 0;
	stackP->contents = NULL;
	stackP->top = -1;
}

int Stack_IsEmpty(stackT *stackP) {
	return stackP->top < 0;
}
int Stack_IsFull(stackT *stackP) {
	return stackP->top >= stackP->maxSize - 1;
}

void Stack_Push(stackT *stackP, stackElementT element) {
	if (Stack_IsFull(stackP)) {
		fprintf(stderr, "Push error: stack is full.\n");
		exit(1);
	}

	stackP->contents[++stackP->top] = element;
}

stackElementT Stack_Pop(stackT *stackP) {
	if (Stack_IsEmpty(stackP)) {
		fprintf(stderr, "Pop error: stack is empty.\n");
		exit(1);
	}
	return stackP->contents[stackP->top--];
}
