#include "interpret.h"

void Interpret__init__(Interpret * self, Stack instructions) {
	self->instructions = instructions;
	Stack__init__(&(self->stack));
}

void Interpret_run(Interpret * self) {
	Instruction i = Stack_pop(&(self->instructions));
	switch (i.code) {
	case i_write:
		write(i.type, Stack_pop(&(self->stack)));
		break;
	case i_push:
		if (i.type == iStackRef) {
			Stack_push(&(self->stack),
					*Stack_getAt(&self->stack, i.a1->stackAddr));
		} else {
			Stack_push(&(self->stack), i.a1);
		}
		break;
	default:
		unimplementedError("Unimplemented instruction for the interpret\n");
	}

}
