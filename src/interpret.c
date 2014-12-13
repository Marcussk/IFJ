#include "interpret.h"

IMPLEMENT_STACK(i, sVal)

#define POP(pom)                     \
pom = iStack_pop(&(self->stack));        \
if (!(pom.isInitialized))                \
	Error_rt_notInit(self->stack.top + 1); \


void iStack_debug(iStack *self, int stackOffset, char * msg) {
	int i;
	if (msg)
		printf("%s: ", msg);
	printf("<stack %p, size: %d, offset: %d>\n", (void *) self, self->top,
			stackOffset);
	printf("___________________________________________________________\n");
	printf("Velkost stacku: %d \n ", self->prealocated);
	printf("Vypis poloziek staku:\n ");
	for (i = 0; i <= self->top; i++) {
		if (i == self->top) {
			printf("top->");
		}
		printf("%d.pos -> %d \n ", i, (*iStack_getAt(self, i)).val.iInt);
	}
}

void Interpret__init__(Interpret * self, InstrQueue instructions) {
	self->instructions = instructions;
	iStack__init__(&(self->stack));
}

void Interpret_run(Interpret * self) {
	Instruction i;
	int index;
	sVal pomA1;
	sVal pomA2;
	sVal pomA3;
	sVal pomA4;
	//InstrQueue_debug(&self->instructions);

	int stackOffset = 0;
	self->instructions.actual = -1;
	i = *InstrQueue_next(&self->instructions);
	while (self->instructions.size > self->instructions.actual) {
		i = self->instructions.QueueArr[self->instructions.actual];
		switch (i.code) {
		case i_push:
			if (i.type == iStackRef) {
				iStack_push(&(self->stack),
						*iStack_getAt(&self->stack,
								i.a1->stackAddr + stackOffset));
			} else if (i.type == iStackGRef) {
				iStack_push(&(self->stack),
						*iStack_getAt(&self->stack, i.a1->stackAddr));
			} else {
				pomA1.val = InstrP2iVal(i.a1, i.type);
				pomA1.isInitialized = i.a1 != NULL;
				iStack_push(&(self->stack), pomA1);
			}
			break;
		case i_assign:
			POP(pomA1)
			if (i.type == iStackRef) {
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA1;
			} else if (i.type == iStackGRef) {
				*iStack_getAt(&self->stack, i.dest->stackAddr) = pomA1;

			}

			break;
		case i_jmp:
			self->instructions.actual = i.dest->iInt;
			continue;
		case i_jmpz:
			POP(pomA1)
			if (pomA1.val.iInt == 0) {
				self->instructions.actual = i.dest->iInt;
				continue;
			}

			break;
		case i_equal:
			POP(pomA1)
			POP(pomA2)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA2.val.iInt == pomA1.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA2.val.iReal == pomA1.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						== 0);
				break;
			default:
				Error_unimplemented(
						"Instr. equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_nequal:
			POP(pomA1)
			POP(pomA2)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA2.val.iInt != pomA1.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA2.val.iReal != pomA1.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						!= 0);
				break;
			default:
				Error_unimplemented(
						"Instr. not equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_more:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt > pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal > pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						> 0);
				break;
			default:
				Error_unimplemented(
						"Instr. more is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_less:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = ((pomA1.val.iInt) < (pomA2.val.iInt));
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal < pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						< 0);
				break;
			default:
				Error_unimplemented(
						"Instr. less is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_moreq:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt >= pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal >= pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						>= 0);
				break;
			default:
				Error_unimplemented(
						"Instr. more or equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_loreq:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt <= pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal <= pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString)
						<= 0);
				break;
			default:
				Error_unimplemented(
						"Instr. less or equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_add:
			POP(pomA2)
			POP(pomA1)

			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA2.val.iInt + pomA1.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA2.val.iReal + pomA1.val.iReal;
				break;
			case iString:
				pomA3.val.iString = malloc(
						strlen(pomA1.val.iString) + strlen(pomA2.val.iString)
								+ 1);
				sprintf(pomA3.val.iString, "%s%s", pomA1.val.iString,
						pomA2.val.iString);
				break;
			default:
				Error_unimplemented(
						"Instr. add is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);

			break;
		case i_sub:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA1.val.iInt - pomA2.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA1.val.iReal - pomA2.val.iReal;
				break;
			default:
				Error_unimplemented(
						"Instr. sub is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);

			break;
		case i_mul:
			POP(pomA2)
			POP(pomA1)
			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA1.val.iInt * pomA2.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA1.val.iReal * pomA2.val.iReal;
				break;
			default:
				Error_unimplemented(
						"Instr. mul is not implemented for this type\n");
				break;
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);

			break;
		case i_div:
			POP(pomA2)
			POP(pomA1)
			if (i.type == iReal) {
				if (pomA2.val.iReal == 0) {
					Error_rt_zeroDivision();
				}
				pomA3.val.iReal = pomA1.val.iReal / pomA2.val.iReal;
			} else {
				Error_unimplemented(
						"Instr. div is not implemented for this type\n");
			}
			if (i.dest != NULL)
				*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
						pomA3;
			else
				pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);

			break;

		case i_write:
			POP(pomA1)
			write(i.type, pomA1.val);
			break;
		case i_readln:
			if (!(readLn(
					&((iStack_getAt(&self->stack,
							i.dest->stackAddr + stackOffset))->val), i.type)))
				Error_rt_readln();
			iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset)->isInitialized =
					true;
			break;
		case i_sort:
			POP(pomA1)
			pomA3.val.iString = func_sort(strdup(pomA1.val.iString));
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
			break;
		case i_len:
			POP(pomA1)
			pomA3.val.iInt = func_len(pomA1.val.iString);
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;

		case i_int2real:
			(*iStack_getAt(&self->stack, self->stack.top + i.dest->stackAddr)).val.iReal =
					(*iStack_getAt(&self->stack,
							self->stack.top + i.a1->stackAddr)).val.iInt;
			break;

		case i_copy:
			POP(pomA3)
			POP(pomA2)
			POP(pomA1)

			pomA4.val.iString = func_copy(pomA1.val.iString, pomA2.val.iInt,
					pomA3.val.iInt);
			pomA4.isInitialized = true;
			iStack_push(&(self->stack), pomA4);
			break;

		case i_find:
			POP(pomA2)
			POP(pomA1)
			pomA3.val.iInt = func_find(pomA1.val.iString, pomA2.val.iString);
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;

		case i_call:
			pomA1.val = (iVal) 0;
			iStack_push(&self->stack, pomA1);            // return value on 0
			pomA4.val.iInt = self->stack.top;
			pomA1.val = (iVal) i.a1->iInt;
			iStack_push(&self->stack, (sVal) pomA1); // paramsCnt at 1
			pomA1.val = (iVal) (self->instructions.actual + 1);
			iStack_push(&self->stack, pomA1); // next instr after return  at 2
			pomA1.val = (iVal) (stackOffset);
			iStack_push(&self->stack, pomA1); // push old stack offset  at 3
			InstrQueue_atIndex(&(self->instructions), i.dest->iInt); // jmp to instr body
			stackOffset = pomA4.val.iInt;
			continue;

		case i_return:
			while (self->stack.top > stackOffset + 3) { //clear all fn mess
				iStack_pop(&(self->stack));
			}
			stackOffset = iStack_pop(&self->stack).val.iInt;
			pomA4 = iStack_pop(&self->stack);                      // next instr
			InstrQueue_atIndex(&(self->instructions), pomA4.val.iInt); // jmp back to caller
			pomA1 = iStack_pop(&self->stack);                      // paramsCnt
			POP(pomA2)
			for (index = 0; index < pomA1.val.iInt; index++) { // pop params
				iStack_pop(&self->stack);
			}
			iStack_push(&self->stack, pomA2);
			continue;
		case i_noop:
			break;

		case i_stop:
			return;

		default:
			Error_rt("Instruction with unknown type");
		}
		i = *InstrQueue_next(&self->instructions);
		if (!self->instructions.actual == self->instructions.size - 1) {
			Error_rt("Program was not properly finished");
		}

	}

}

void Interpret__dell__(Interpret * self) {
	InstrQueue__dell__(&self->instructions);
	iStack__dell__(&self->stack);
	fflush(stdout);
}
