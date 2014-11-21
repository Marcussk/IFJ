#include "interpret.h"

void interpretRun(Stack * s) {
	Instruction i = Stack_pop(s);
	switch (i.code) {
	case WRITE:
		write(i.type, i.a1);
		break;
	default:
		printf("inter. err.\n");
	}

}
