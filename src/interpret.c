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
		iVal pomA1;
		iVal pomA2;
		iVal pomA3;
		switch (i.code) {
		case i_add:
			if (i.dest != NULL) {
				switch(i.type){
					case iInt:
						*(int *)(i.dest) =
								(pomA2 = iStack_pop(&(self->stack))).iInt +
								(pomA1 = iStack_pop(&(self->stack))).iInt;
						break;
					case iReal:
						*(float*)(i.dest) =
								(pomA2 = iStack_pop(&(self->stack))).iReal +
								(pomA1 = iStack_pop(&(self->stack))).iReal;
						break;
					default:
					// [TODO] ERROR
						break;
								}
			}
			else{
				pomA2 = iStack_pop(&(self->stack));
				pomA1 = iStack_pop(&(self->stack));
				switch(i.type){
				case iInt:
					pomA3.iInt = (pomA1.iInt + pomA2.iInt);
					iStack_push(&(self->stack), pomA3);
					break;
				case iReal:
					pomA3.iReal = (pomA1.iReal + pomA2.iReal);
					iStack_push(&(self->stack), pomA3);
					break;
				default:
					// [TODO] ERROR
					break;
				}
			}
			break;
		case i_sub:

			break;

		case i_write:
			write(i.type, iStack_pop(&(self->stack)));
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
void Interpret_test2() {

	InstrParam a, b;
		a.iReal = 8;
		b.iReal = 2.5;
		InstrQueue instr;
		Interpret intr;
		InstrQueue__init__(&instr);
		InstrQueue_insert(&instr, (Instruction ) { i_push, iReal, &a, NULL, NULL });
		InstrQueue_insert(&instr,
				(Instruction ) { i_push, iReal, &b, NULL, NULL });

		InstrQueue_insert(&instr,
				(Instruction ) { i_add, iReal, &a, &b, NULL });

		Interpret__init__(&intr, instr);
		Interpret_run(&intr);
		printf("%f interpret test end \n", (iStack_pop(&(intr.stack))).iReal );

}

void Interpret_test3() {

	InstrParam a, b, c;
		a.iReal = 8;
		b.iReal = 2.5;
		c.iReal = 0;
		InstrQueue instr;
		Interpret intr;
		InstrQueue__init__(&instr);
		InstrQueue_insert(&instr, (Instruction ) { i_push, iReal, &a, NULL, NULL });
		InstrQueue_insert(&instr,
				(Instruction ) { i_push, iReal, &b, NULL, NULL });

		InstrQueue_insert(&instr,
				(Instruction ) { i_add, iReal, &a, &b, &c });

		Interpret__init__(&intr, instr);
		Interpret_run(&intr);
		printf("%f interpret test end \n", c );

}
