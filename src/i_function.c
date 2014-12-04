#include "i_function.h"


iFunction * iFunction__init__() {
	iFunction * self = malloc(sizeof(iFunction));
	if (!self) {
		memoryError("Can't allocate memory for iFunction");
	}
	self->bodyInstrIndex = -1;
	self->params = NULL;
	self->retVal.isInitialized = false;
	self->retVal.stackIndex = 0;
	self->retVal.type = iUnknown;
	self->builtin = b_none;
	return self;
}

ParamsListItem * ParamsListItem__init__() {
	ParamsListItem * self = malloc(sizeof(ParamsListItem));
	if (!self) {
		memoryError("Can't allocate memory for ParamsListItem");
	}
	self->param = NULL;
	self->next = NULL;
	return self;
}

void iFunction_addParam(iFunction * self, iVar * var) {
	ParamsListItem * lastItem = self->params;
	int stackAddr = -1;
	if (!lastItem) {
		self->params = ParamsListItem__init__();
		self->params->param = var;
	} else {
		while (lastItem->next){
			lastItem = lastItem->next;
			var->stackIndex --;
		}
		lastItem->next = ParamsListItem__init__();
		lastItem->next->param = var;
	}
	var->stackIndex = stackAddr;
}

void ParamsListItem__dell__(ParamsListItem * self) {
	if (self->next)
		ParamsListItem__dell__(self->next);
	free(self);
}

void iFunction__dell__(iFunction * self) {
	if (self->params)
		ParamsListItem__dell__(self->params);
}

