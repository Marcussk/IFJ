#include "interpret.h"

iVal inline InstrP2iVal(InstrParam * a, tIFJ type) {
	iVal v;
	switch (type) {
	case iInt:
		v.iInt = a->iInt;
		break;
	case iString:
		v.iString = a->iString;
		break;
	case iReal:
		v.iReal = a->iReal;
		break;
	default :
		unimplementedError("Unimplemented cast in InstrP2iVal for the interpret\n");
	}

	return v;
}

void Interpret__init__(Interpret * self, InstrQueue instructions) {
	self->instructions = instructions;
	Stack__init__(&(self->stack));
}

void Interpret_run(Interpret * self) {
	Instruction i;
	self->instructions.actual = self->instructions.first;
	while (self->instructions.actual) {
		i = self->instructions.actual->val;

		switch (i.code) {
		case i_write:
			write(i.type, Stack_pop(&(self->stack)));
			break;
		case i_push:
			if (i.type == iStackRef) {
				Stack_push(&(self->stack),
						*Stack_getAt(&self->stack, i.a1->stackAddr));
			} else {
				Stack_push(&(self->stack), InstrP2iVal(i.a1, i.type));
			}
			break;
		default:
			unimplementedError("Unimplemented instruction for the interpret\n");
		}
		InstrQueue_next(&(self->instructions));
	}
}
