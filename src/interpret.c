#include "interpret.h"

IMPLEMENT_STACK(i, iVal)

void iStack_debug(iStack * s){
	int i;
	printf("<stack %p, size: %d>\n", (void *) s, s->size);
	for(i=0 ; i<s->size; i++){
		printf("%d: %d\n", i , (*iStack_getAt(s, i)).iInt);
	}
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
			InstrQueue_atIndex(&(self->instructions), i.dest->iInt);
			continue;
			break;
		case i_jmpz:
			pomA1 = iStack_pop(&(self->stack));
			if(pomA1.iInt == 0){
				InstrQueue_atIndex(&(self->instructions), i.dest->iInt);
				continue;
			}
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
				pomA3.iInt = ((pomA1.iInt) < (pomA2.iInt));
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal < pomA2.iReal);
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
					"Instr. div is not implemented for this type\n");
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
		case i_read:
			    readLn(iStack_getAt(&self->stack, i.dest->stackAddr), i.type);

				break;
		case i_push:
			if (i.type == iStackRef) {
				iStack_push(&(self->stack),
						*iStack_getAt(&self->stack, i.a1->stackAddr));
			} else {
				iStack_push(&(self->stack), InstrP2iVal(i.a1, i.type));
			}
			break;
		case i_stop:
			return;
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
