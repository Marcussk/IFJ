#include "interpret.h"

IMPLEMENT_STACK(i, sVal)

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
			} else {
				pomA1.val = InstrP2iVal(i.a1, i.type);
				 pomA1.isInitialized = i.a1 != NULL;
				iStack_push(&(self->stack), pomA1);
			}
			break;
		case i_assign:
					pomA1 = iStack_pop(&(self->stack));
					pomA1.isInitialized = true;
					if (i.type != iString) {
						*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset) =
								pomA1;
					} else {
						(*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset)).val.iString =
								strdup(pomA1.val.iString);
						(*iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset)).isInitialized =
														true;
					}
					break;
		case i_jmp:
			self->instructions.actual = i.dest->iInt;
			continue;
		case i_jmpz:
			pomA1 = iStack_pop(&(self->stack));
			if (pomA1.val.iInt == 0) {
				self->instructions.actual = i.dest->iInt;
				continue;
			}

			break;
		case i_equal:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA2.val.iInt == pomA1.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA2.val.iReal == pomA1.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) == 0);
				break;
			default:
				unimplementedError(
						"Instr. equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_nequal:
			pomA1 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA2.val.iInt != pomA1.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA2.val.iReal != pomA1.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) != 0);
				break;
			default:
				unimplementedError(
						"Instr. not equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_more:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt > pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal > pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) > 0);
				break;
			default:
				unimplementedError(
						"Instr. more is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_less:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = ((pomA1.val.iInt) < (pomA2.val.iInt));
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal < pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) < 0);
				break;
			default:
				unimplementedError(
						"Instr. less is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_moreq:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt >= pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal >= pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) >= 0);
				break;
			default:
				unimplementedError(
						"Instr. more or equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_loreq:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iBool:
			case iInt:
				pomA3.val.iInt = (pomA1.val.iInt <= pomA2.val.iInt);
				break;
			case iReal:
				pomA3.val.iInt = (pomA1.val.iReal <= pomA2.val.iReal);
				break;
			case iString:
				pomA3.val.iInt = (strcmp(pomA1.val.iString, pomA2.val.iString) <= 0);
				break;
			default:
				unimplementedError(
						"Instr. less or equal is not implemented for this type\n");
				break;
			}
			pomA3.isInitialized = true;
			iStack_push(&(self->stack), pomA3);
			break;
		case i_add:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));

			if(!(pomA1.isInitialized) || !(pomA2.isInitialized)){
				rt_notInitError();
			}


			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA2.val.iInt + pomA1.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA2.val.iReal + pomA1.val.iReal;
				break;
			case iString:
				pomA3.val.iString = malloc(
						(strlen(pomA1.val.iString) + strlen(pomA1.val.iString) + 1)
								* sizeof(char));
				strcpy(pomA3.val.iString, pomA1.val.iString);
				strcat(pomA3.val.iString, pomA2.val.iString);
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
				pomA3.isInitialized = true;
				iStack_push(&(self->stack), pomA3);

			break;
		case i_sub:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));

			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();

			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA1.val.iInt - pomA2.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA1.val.iReal - pomA2.val.iReal;
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
				pomA3.isInitialized = true;
				iStack_push(&(self->stack), pomA3);

			break;
		case i_mul:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			switch (i.type) {
			case iInt:
				pomA3.val.iInt = pomA1.val.iInt * pomA2.val.iInt;
				break;
			case iReal:
				pomA3.val.iReal = pomA1.val.iReal * pomA2.val.iReal;
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
				pomA3.isInitialized = true;
				iStack_push(&(self->stack), pomA3);

			break;
		case i_div:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			if(!(pomA1.isInitialized) || !(pomA2.isInitialized))
							rt_notInitError();
			if (i.type == iReal) {
				if (pomA2.val.iReal == 0) {
					rt_zeroDivisionError();
				}
				pomA3.val.iReal = pomA1.val.iReal / pomA2.val.iReal;
			} else {
				unimplementedError(
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
			pomA1 = iStack_pop(&(self->stack));

			if(!(pomA1.isInitialized))
										rt_notInitError();

			write(i.type, pomA1.val);
			break;
		case i_readln:
			if (!(readLn(
					&((iStack_getAt(&self->stack, i.dest->stackAddr + stackOffset))->val),
					i.type)))
				rt_readlnNumError();
			break;
		case i_sort:
			pomA3.val.iString = strdup(
					func_sort((iStack_pop(&(self->stack)).val.iString)));
			iStack_push(&(self->stack), pomA3);
			break;
		case i_len:
			pomA3.val.iInt = func_len((iStack_pop(&(self->stack)).val.iString));
			iStack_push(&(self->stack), pomA3);
			break;


		case i_int2real:
			(*iStack_getAt(&self->stack, self->stack.top + i.dest->stackAddr)).val.iReal =
					(*iStack_getAt(&self->stack,
							self->stack.top + i.a1->stackAddr)).val.iInt;
			break;

		case i_copy:
			pomA3 = iStack_pop(&(self->stack));
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			pomA4.val.iString = func_copy(pomA1.val.iString, pomA2.val.iInt, pomA3.val.iInt);
			iStack_push(&(self->stack), pomA4);
			break;

		case i_find:
			pomA2 = iStack_pop(&(self->stack));
			pomA1 = iStack_pop(&(self->stack));
			pomA3.val.iInt = func_find(pomA1.val.iString, pomA2.val.iString);
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
			pomA2 = iStack_pop(&self->stack);                      //  retval
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
			rt_error("Instruction with unknown type");
		}
		i = *InstrQueue_next(&self->instructions);
		if (!self->instructions.actual == self->instructions.size - 1) {
			rt_error("Program was not properly finished");
		}

	}

}

void Interpret__dell__(Interpret * self) {
	InstrQueue__dell__(&self->instructions);
	iStack__dell__(&self->stack);
	fflush(stdout);
}
