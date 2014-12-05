#include "interpret.h"

IMPLEMENT_STACK(i, iVal);

void iStack_debug(iStack *self, int stackOffset, char * msg) {
	int i;
	if (msg)
		printf("%s: ", msg);
	printf("<stack %p, size: %d, offset: %d>\n", (void *) self, self->top, stackOffset);
	printf("___________________________________________________________\n");
	printf("Velkost stacku: %d \n ", self->prealocated);
	printf("Vypis poloziek staku:\n ");
	for (i = 0; i <= self->top; i++) {
		if (i == self->top) {
			printf("top->");
		}
		printf("%d.pos -> %d \n ", i, (*iStack_getAt(self, i)).iInt);
	}
}	

void Interpret__init__(Interpret * self, InstrQueue instructions) {
	self->instructions = instructions;
	iStack__init__(&(self->stack));
}

void Interpret_run(Interpret * self) {
	Instruction i;
	int index;
	iVal pomA1;
	iVal pomA2;
	iVal pomA3;
	iVal pomA4;
	InstrQueue_debug(&self->instructions);

	int stackOffset = 0;
	self->instructions.actual = -1;
	i = *InstrQueue_next(&self->instructions);
	while (self->instructions.top > self->instructions.actual) {
		i = self->instructions.QueueArr[self->instructions.actual];
		switch (i.code) {
		case i_noop:
			break;
		case i_jmp:
			self->instructions.actual = i.dest->iInt;
			continue;
		case i_jmpz:
			pomA1 = iStack_pop(&(self->stack));
			if (pomA1.iInt == 0) {
				self->instructions.actual = i.dest->iInt;
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
			pomA3.iString = strdup(func_sort((iStack_pop(&(self->stack)).iString)));
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
			pomA3 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			pomA4.iString = func_copy(pomA1.iString, pomA2.iInt, pomA3.iInt);
			iStack_push(&(self->stack), pomA4);
			break;

		case i_find:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			pomA3.iInt = func_find(pomA1.iString, pomA2.iString);
			iStack_push(&(self->stack), pomA3);
			break;

		case i_call:
			iStack_push(&self->stack, (iVal) 0);            // return value on 0
			pomA4.iInt = self->stack.top;
			iStack_push(&self->stack, (iVal) i.a1->iInt);      // paramsCnt at 1
			iStack_push(&self->stack, (iVal) (self->instructions.actual + 1)); // next instr after return  at 2
			iStack_push(&self->stack, (iVal) (stackOffset)); // push old stack offset  at 3
			InstrQueue_atIndex(&(self->instructions), i.dest->iInt); // jmp to instr body
			stackOffset = pomA4.iInt;
			continue;

		case i_return:

			while (self->stack.top > stackOffset + 4) { //clear all fn mess
				iStack_pop(&(self->stack));
			}
			stackOffset = iStack_pop(&self->stack).iInt;
			pomA4 = iStack_pop(&self->stack);                      // next instr
			InstrQueue_atIndex(&(self->instructions), pomA4.iInt); // jmp back to caller
			pomA1 = iStack_pop(&self->stack);                      // paramsCnt
			pomA2 = iStack_pop(&self->stack);                      //  retVal
			for (index = 0; index < pomA1.iInt; index++) { // pop params
				iStack_pop(&self->stack);
			}
			iStack_push(&self->stack, pomA2);
			continue;

		case i_stop:
			return;

		default:
			rt_error("Instruction with unknown type");
		}
		i = *InstrQueue_next(&self->instructions);
		if (!self->instructions.actual == self->instructions.top - 1) {
			rt_error("Program was not properly finished");
		}

	}

}

void Interpret__dell__(Interpret * self) {
	InstrQueue__dell__(&self->instructions);
	iStack__dell__(&self->stack);
	fflush(stdout);
}
