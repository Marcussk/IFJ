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

void InstrQueue_insert(InstrQueue * self, Instruction i) {
	InstrQueueNode * newItem = malloc(sizeof(InstrQueueNode)); // [TODO] check allocation
	newItem->val = i;
	if (self->actual) {
		newItem->next = self->actual->next;
		self->actual->next = newItem;
	}
	if (!self->first)
		self->first = newItem;

	self->actual = newItem;
}

Instruction * InstrQueue_atIndex(InstrQueue * self, int index) {
	int i;
	self->actual = self->first;
	for (i = 0; i < index; i++){
		if(!self->actual){
			// [TODO] InstrQueue_atIndex out of size
		}
		self->actual = self->actual->next;
	}
	return &self->actual;
}
