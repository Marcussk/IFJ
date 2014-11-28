#include "iFunction.h"


iFunction * iFunction__init__() {
	iFunction * self = malloc(sizeof(iFunction));
	if (!self) {
		memoryError("Can't allocate memory for iFunction");
	}
	self->body = NULL;
	self->params = NULL;
	self->retType = iUnknown;
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
	if (!lastItem) {
		self->params = ParamsListItem__init__();
		self->params->param = var;
	} else {
		while (lastItem->next)
			lastItem = lastItem->next;
		lastItem->next = ParamsListItem__init__();
		lastItem->next->param = var;
	}
}

ParamsListItem * iFunction_lastParam(iFunction * self) {
	ParamsListItem * lastItem = self->params;
	if (!lastItem)
		return self->params;

	while (lastItem->next)
		lastItem = lastItem->next;

	return lastItem;
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

