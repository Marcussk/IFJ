#include "interpret.h"

void interpretRun(Stack * s) {
	Instruction i = Stack_pop(s);
	switch (i) {
	case WRITE:
		write(i.a1, i.a2, i.a3);
	}

}
