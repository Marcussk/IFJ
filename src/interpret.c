#include "interpret.h"

IMPLEMENT_STACK(i, iVal)

iVal inline InstrP2iVal(InstrParam * a, tIFJ type) {
	iVal v;
	if (!a) {
		v.iInt = 0;
		return v;
	}
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
	iVal pomA1;
	iVal pomA2;
	iVal pomA3;

	self->instructions.actual = self->instructions.first;
	while (self->instructions.actual) {
		i = self->instructions.actual->val;
		if (i.type == iUnknown)
			rt_error("Instruction with unknown type");
		switch (i.code) {
		case i_noop:
			break;
		case i_jmp:
			unimplementedError("JMP is not implemented yet");
			break;
		case i_jmpz:
			unimplementedError("JMPZ is not implemented yet");
			break;
		case i_equal:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA2.iInt == pomA1.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA2.iReal == pomA1.iReal);
				break;
			case iString:
			    //[TODO]
				break;
			default:
				unimplementedError(
						"Instr. equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_nequal:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA2.iInt != pomA1.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA2.iReal != pomA1.iReal);
				break;
			case iString:
				 //[TODO]
				break;
			default:
				unimplementedError(
						"Instr. not equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_more:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
					pomA3.iInt = (pomA1.iInt > pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal > pomA2.iReal);
				break;
			case iString:
			//[TODO]
				break;
			default:
				unimplementedError(
						"Instr. more is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_less:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				if ((pomA1.iInt) < (pomA2.iInt)) {
					pomA3.iInt = 1;
				} else {
					pomA3.iInt = 0;
				}
				break;
			case iReal:
				if (pomA1.iReal < pomA2.iReal) {
					pomA3.iInt = 1;
				} else {
					pomA3.iInt = 0;
				}
				break;
			case iString:
				//[TODO]
				break;
			default:
				unimplementedError(
						"Instr. less is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_moreq:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA1.iInt >= pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal >= pomA2.iReal);
				break;
			case iString:
				//[TODO]
				break;
			default:
				unimplementedError(
						"Instr. more or equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_loreq:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA1.iInt <= pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal <= pomA2.iReal);
				break;
			case iString:
				//[TODO]
				break;
			default:
				unimplementedError(
						"Instr. less or equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_stop:
			return;
		case i_add:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = pomA2.iInt + pomA1.iInt;
				break;
			case iReal:
				pomA3.iReal = pomA2.iReal + pomA1.iReal;
				break;
			default:
				unimplementedError(
						"Instr. add is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_sub:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
				case iInt:
					pomA3.iInt = pomA1.iInt - pomA2.iInt;
					break;
				case iReal:
					pomA3.iReal = pomA1.iReal - pomA2.iReal;
					break;
				default:
					unimplementedError(
					"Instr. sub is not implemented for this type\n");
					break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_mul:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
				case iInt:
					pomA3.iInt = pomA1.iInt * pomA2.iInt;
					break;
				case iReal:
					pomA3.iReal = pomA1.iReal * pomA2.iReal;
					break;
				default:
					unimplementedError(
					"Instr. mul is not implemented for this type\n");
					break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_div:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			switch (i.type) {
				case iInt:
					pomA3.iInt = pomA1.iInt / pomA2.iInt;
					break;
				case iReal:
					pomA3.iReal = pomA1.iReal / pomA2.iReal;
					break;
				default:
					unimplementedError(
					"Instr. sub is not implemented for this type\n");
					break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_assign:
			if (i.type != iString) {
				pomA1 = iStack_pop(&(self->stack));
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA1;
				break;
			} else {
				//[TODO]
				break;
			}

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

void Interpret__dell__(Interpret * self) {
	/*
	 InstrQueue__dell__(self->instructions);
	 iStack__dell__(&self->stack);
	 */
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
	InstrQueue_insert(&instr, (Instruction ) { i_push, iReal, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_add, iReal, &a, &b, NULL });

	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("%f interpret test end \n", (iStack_pop(&(intr.stack))).iReal);

}

void Interpret_test3() {

	InstrParam a, b, c;
	a.iString = "kanonk";
	b.iString = "kanon";
	c.iInt = 20;
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

	//InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &c, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
	InstrQueue_insert(&instr,
			(Instruction ) { i_moreq, iString, &a, &b, NULL });

	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("%d interpret test end \n", (iStack_pop(&(intr.stack))).iInt);

}
void Interpret_test4() {

	InstrParam a, b, c;
	a.iString = "fsdfsd";
	b.iInt = 20;

	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

	InstrQueue_insert(&instr,
			(Instruction ) { i_push, iString, &c, NULL, NULL });
	InstrQueue_insert(&instr,
			(Instruction ) { i_push, iString, &a, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_assign, iString, &a, &b, &c });

	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("%s  \n", (iStack_pop(&(intr.stack))).iString);

}
