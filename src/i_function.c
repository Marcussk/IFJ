#include "i_function.h"


iFunction * iFunction__init__() {
	iFunction * self = malloc(sizeof(iFunction));
	if (!self) {
		memoryError("Can't allocate memory for iFunction");
	}
	self->bodyInstrIndex = -1;
	self->params.First = NULL;
	self->params.Last = NULL;
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
	self->data = NULL;
	self->next = NULL;
	self->prev = NULL;
	return self;
}

void iFunction_listInit(ParamsList *List) {
	List->First = NULL;
	List->Last = NULL;
}

void iFunction_addParam(iFunction * self, iVar * var) {
	ParamsListItem *newItem = NULL;

	newItem = malloc(sizeof(ParamsListItem));
	if (newItem == NULL){
		memoryError("Could not allocate memory for parameter");
		return;
	}

	newItem->data = var;
	newItem->next = NULL;

	if (self->params.First == NULL){
		self->params.First = newItem;
		newItem->prev = NULL;
	}
	else{
		self->params.Last->next = newItem;
		newItem->prev = self->params.Last;
	}

	self->params.Last = newItem;
}
/*
void iFunction_addParam(iFunction * self, iVar * var) {
	ParamsListItem * lastItem = self->params;
	if (!lastItem) {
		self->params = ParamsListItem__init__();
		self->params->data = var;
	} else {
		while (lastItem->next){
			lastItem = lastItem->next;
		}
		lastItem->next = ParamsListItem__init__();
		lastItem->next->data = var;
	}
}
*/

void iFunction_buildParamIndexes(iFunction * self) {
	int i = -1;
	ParamsListItem * lastItem = self->params.Last;
	while(lastItem){
		lastItem->data->stackIndex = i;
		i--;
		lastItem = lastItem->prev;
	}
}

void ParamsListItem__dell__(ParamsListItem * self) {
	if (self->next)
		ParamsListItem__dell__(self->next);
	free(self);
}

void iFunction__dell__(iFunction * self) {
	if (self->params.First)
		ParamsListItem__dell__(self->params.First);
}

