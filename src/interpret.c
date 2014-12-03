#include "interpret.h"

IMPLEMENT_STACK(i, iVal)

void iStack_debug(iStack * s) {
	int i;
	printf("<stack %p, size: %d>\n", (void *) s, s->size);
	for (i = 0; i < s->size; i++) {
		printf("%d: %d\n", i, (*iStack_getAt(s, i)).iInt);
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
	iVal pomA4;
	InstrQueue_debug(&self->instructions);

	int stackOffset = 0;
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
			if (pomA1.iInt == 0) {
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
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) == 0);
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
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) != 0);
				break;
			default:
				unimplementedError(
						"Instr. not equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_more:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA1.iInt > pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal > pomA2.iReal);
				break;
			case iString:
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) > 0);
				break;
			default:
				unimplementedError(
						"Instr. more is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_less:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = ((pomA1.iInt) < (pomA2.iInt));
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal < pomA2.iReal);
				break;
			case iString:
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) < 0);
				break;
			default:
				unimplementedError(
						"Instr. less is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_moreq:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA1.iInt >= pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal >= pomA2.iReal);
				break;
			case iString:
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) >= 0);
				break;
			default:
				unimplementedError(
						"Instr. more or equal is not implemented for this type\n");
				break;
			}
			iStack_push(&(self->stack), pomA3);
			break;
		case i_loreq:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			switch (i.type) {
			case iInt:
				pomA3.iInt = (pomA1.iInt <= pomA2.iInt);
				break;
			case iReal:
				pomA3.iInt = (pomA1.iReal <= pomA2.iReal);
				break;
			case iString:
				pomA3.iInt = (strcmp(pomA1.iString, pomA2.iString) <= 0);
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
			case iString:
				pomA3.iString = malloc(
						(strlen(pomA1.iString) + strlen(pomA1.iString) + 1)
								* sizeof(char));
				strcpy(pomA3.iString, pomA1.iString);
				strcat(pomA3.iString, pomA2.iString);
				break;
			default:
				unimplementedError(
						"Instr. add is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_sub:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
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
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_mul:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
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
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_div:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));

			switch (i.type) {
			case iInt:
				if (pomA2.iInt == 0) {
					rt_zeroDivisionError();
				}
				pomA3.iInt = pomA1.iInt / pomA2.iInt;
				break;
			case iReal:
				if (pomA2.iReal == 0) {
					rt_zeroDivisionError();
				}
				pomA3.iReal = pomA1.iReal / pomA2.iReal;
				break;
			default:
				unimplementedError(
						"Instr. div is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);

			break;
		case i_assign:
			pomA1 = iStack_pop(&(self->stack));
			if (i.type != iString) {
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA1;
			} else {
				(*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset)).iString =
						strdup(pomA1.iString);
			}
			break;
		case i_write:
			write(i.type, iStack_pop(&(self->stack)));
			break;
		case i_readln:
			if (!(readLn(
					iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset),
					i.type)))
				rt_readlnNumError();
			break;
		case i_sort:
			pomA3.iString = func_sort((iStack_pop(&(self->stack)).iString));
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);
			break;
		case i_len:
			pomA3.iInt = func_len((iStack_pop(&(self->stack)).iString));
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				iStack_push(&(self->stack), pomA3);
			break;

		case i_push:
			if (i.type == iStackRef) {
				iStack_push(&(self->stack),
						*iStack_getAt(&self->stack,
								i.a1->stackAddr + stackOffset));
			} else {
				iStack_push(&(self->stack), InstrP2iVal(i.a1, i.type));
			}
			break;

		case i_int2real:
			pomA1 = iStack_pop(&(self->stack));
			pomA2.iReal = pomA1.iInt;
			iStack_push(&(self->stack), pomA2);
			break;

		case i_copy:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			pomA3 = iStack_pop(&(self->stack));
			pomA4.iString = func_copy(pomA1.iString, pomA2.iInt, pomA3.iInt);
			iStack_push(&(self->stack), pomA4);
			break;

		case i_find:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			pomA3.iInt = func_find(pomA1.iString, pomA2.iString);
			iStack_push(&(self->stack), pomA3);
			break;

		case i_call:
			stackOffset = self->stack.actualIndex;
			iStack_push(&self->stack, (iVal) (self->instructions.index + 1));

			break;

		case i_return:

			while (self->stack.actualIndex != stackOffset + 1)
				iStack_pop(&(self->stack));

			InstrQueue_atIndex(&(self->instructions), i.dest->iInt);
			break;

		case i_stop:
			return;

		default:
			unimplementedError("Unimplemented instruction for the interpret\n");
		}
		InstrQueue_next(&(self->instructions));
	}
	if(!self->instructions.actual){rt_error("Program was not properly finished");}
}
void iStack__dell__(iStack * self) {
	int i;
	for (i = 0; i < self->size; i++) {
		iStack_pop(self);
	}
}

void Interpret__dell__(Interpret * self) {
	InstrQueue__dell__(&self->instructions);
	iStack__dell__(&self->stack);
}
