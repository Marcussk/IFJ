#ifndef STACK_h
#define STACK_h

#include "stdlib.h"
#include "stdio.h"

#define IMPL_STACK(stackElementT, namePrefix) \
			typedef struct {                  \
				stackElementT *contents;      \
				int top;                      \
				int maxSize;                  \
			} namePrefix##stackT;                         \
											\
void namePrefix##Stack__init__(namePrefix##stackT *stackP, int maxSize) {                 \
	stackElementT *newContents;                                               \
	newContents = (stackElementT *) malloc(sizeof(stackElementT) * maxSize);  \
                                                                              \
	if (newContents == NULL) {                                                \
		fprintf(stderr, "Insufficient memory to initialize stack.\n");        \
		exit(1);                                                              \
	}                                                                         \
                                                                              \
	stackP->contents = newContents;                                           \
	stackP->maxSize = maxSize;                                                \
	stackP->top = -1;                                                         \
}                                                                             \
                                                                              \
void namePrefix##Stack__dell__(namePrefix##stackT *stackP) {                              \
	free(stackP->contents);                                                   \
                                                                              \
	stackP->maxSize = 0;                                                      \
	stackP->contents = NULL;                                                  \
	stackP->top = -1;                                                         \
}                                                                             \
                                                                              \
int namePrefix##Stack_IsEmpty(namePrefix##stackT *stackP) {                               \
	return stackP->top < 0;                                                   \
}                                                                             \
int namePrefix##Stack_IsFull(namePrefix##stackT *stackP) {                                \
	return stackP->top >= stackP->maxSize - 1;                                \
}                                                                             \
                                                                              \
void namePrefix##Stack_Push(namePrefix##stackT *stackP, stackElementT element){           \
	if (namePrefix##Stack_IsFull(stackP)) {                                               \
		fprintf(stderr, "Push error: stack is full.\n");                      \
		exit(1);                                                              \
	}                                                                         \
                                                                              \
	stackP->contents[++stackP->top] = element;                                \
}                                                                             \
                                                                              \
stackElementT namePrefix##Stack_Pop(namePrefix##stackT *stackP) {             \
	if (namePrefix##Stack_IsEmpty(stackP)) {                                  \
		fprintf(stderr, "Pop error: stack is empty.\n");                      \
		exit(1);                                                              \
	}                                                                         \
	return stackP->contents[stackP->top--];                                   \
}                                                                             \
                                                                              \

/*stackElementT * namePrefix##_Top(namePrefix##stackT *stackP){                   \
	if(stackP->top <0){                                                       \
		fprintf(stderr, "Top error: stack is empty.\n");                      \
		exit(1);                                                              \
	}                                                                         \
	return &(stackP->contents[stackP->top]);                                     \
}*/

#endif
