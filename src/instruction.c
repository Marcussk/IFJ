#include "instruction.h"

//vytvari novou instrukci
Instruction Instr_Create(InstrCode code, tIFJ typ, InstrParam * a1,
		InstrParam * a2, InstrParam *a3) {
	Instruction I;
	I.code = code;
	I.type = typ;
	I.a1 = a1;
	I.a2 = a2;
	I.dest = a3;
	return I;
}

void InstrQueue__init__(InstrQueue * self) {
	self->actual = NULL;
	self->first = NULL;
}
Instruction * InstrQueue_next(InstrQueue * self) {
	if (self->actual)
		self->actual = self->actual->next;
	if (self->actual)
		return &(self->actual->val);

	return NULL; // [TODO] we probably wont there an error
}

void InstrQueue_debug(InstrQueue * self) {
	InstrQueueNode * item = self->actual;
	Instruction instr;
	int i = 0;
	self->actual = self->first;

	while (self->actual) {
		instr = self->actual->val;
		if (self->actual == item)
			printf("->");
		else
			printf("  ");
		printf("%d code %d type %d, a1 %p, a2 %p, dest %p\n", i, instr.code,
				instr.type, instr.a1, instr.a2, instr.dest);
		i++;
		self->actual = self->actual->next;
	}
	self->actual = item;

}

void InstrQueue_insert(InstrQueue * self, Instruction i) {
	InstrQueueNode * newItem = malloc(sizeof(InstrQueueNode));
	if (!newItem) {
		memoryError("InstrQueue_insert can't allocate memory for newItem");
	}
	newItem->val = i;
	if (self->actual) {
		newItem->next = self->actual->next;
		self->actual->next = newItem;
	}
	if (!self->first) {
		self->first = newItem;
		newItem->next = NULL;
	}

	self->actual = newItem;
}

Instruction * InstrQueue_atIndex(InstrQueue * self, int index) {
	int i;
	self->actual = self->first;
	for (i = 0; i < index; i++) {
		if (!self->actual) {
			rt_error("InstrQueue_atIndex out of index");
		}
		self->actual = self->actual->next;
	}
	return &(self->actual)->val;
}

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
