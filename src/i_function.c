#include "i_function.h"


void ParamsList__init__(ParamsList *List) {
	List->First = NULL;
	List->Last = NULL;
	List->size = 0;
}

iFunction * iFunction__init__() {
	iFunction * self = malloc(sizeof(iFunction));
	if (!self) {
		Error_memory("Can't allocate memory for iFunction");
	}
	self->bodyInstrIndex = malloc(sizeof(InstrParam));
	if (!self->bodyInstrIndex) {
		Error_memory("Can't allocate memory for bodyInstrIndex in iFunction");
	}
	self->bodyInstrIndex->iInt = -1;
	ParamsList__init__(&self->params);
	self->retVal.isGlobal = false;
	self->retVal.stackIndex = 0;
	self->retVal.type = iUnknown;
	self->builtin = b_none;
	self->name = NULL;
	self->bodyFound = false;
	self->forwardFound = false;
	return self;
}

ParamsListItem * ParamsListItem__init__() {
	ParamsListItem * self = malloc(sizeof(ParamsListItem));
	if (!self) {
		Error_memory("Can't allocate memory for ParamsListItem");
	}
	self->name = NULL;
	self->data = NULL;
	self->next = NULL;
	self->prev = NULL;
	return self;
}


void iFunction_addParam(iFunction * self, iVar * var, char * name) {
	ParamsListItem *newItem = NULL;
	newItem = malloc(sizeof(ParamsListItem));
	if (newItem == NULL) {
		Error_memory("Could not allocate memory for parameter");
		return;
	}

	newItem->data = var;
	newItem->next = NULL;
	newItem->name = name;

	if (self->params.First == NULL) {
		self->params.First = newItem;
		newItem->prev = NULL;
	} else {
		self->params.Last->next = newItem;
		newItem->prev = self->params.Last;
	}

	self->params.Last = newItem;
	(self->params.size)++;
}

void iFunction_buildParamIndexes(iFunction * self) {
	int i = -1;
	ParamsListItem * lastItem = self->params.Last;
	while (lastItem) {
		lastItem->data->stackIndex = i;
		i--;
		lastItem = lastItem->prev;
	}
}

void ParamsListItem__dell__(ParamsListItem * self) {
	free(self->name);
	if (self->next)
		ParamsListItem__dell__(self->next);
	free(self);
}

void iFunction__dell__(iFunction * self) {
	if (self->params.First)
		ParamsListItem__dell__(self->params.First);
}

