#include "interpret.h"

IMPLEMENT_STACK(i, iVal)

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
	default:
		unimplementedError(
				"Unimplemented cast in InstrP2iVal for the interpret\n");
	}

	return v;
}

void Interpret__init__(Interpret * self, InstrQueue instructions) {
	self->instructions = instructions;
	iStack__init__(&(self->stack));
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
				iStack_push(&(self->stack),
						*iStack_getAt(&self->stack, i.a1->stackAddr));
			} else {
				iStack_push(&(self->stack), InstrP2iVal(i.a1, i.type));
			}
			break;
		default:
			unimplementedError("Unimplemented instruction for the interpret\n");
		}
		InstrQueue_next(&(self->instructions));
	}
}

void Interpret_test1() {
	InstrParam a, b;
	a.iInt = 8;
	b.iString = " from interpret\n";
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
	InstrQueue_insert(&instr,
			(Instruction ) { i_write, iInt, NULL, NULL, NULL });

	InstrQueue_insert(&instr,
			(Instruction ) { i_push, iString, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_write, iString, NULL, NULL,
			NULL });
	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("interpret test end \n");
}
