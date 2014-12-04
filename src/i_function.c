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

void iFunction_addParam(ParamsList *List, iVar * var) {
	ParamsListItem *newItem = NULL;

	newItem = malloc(sizeof(ParamsListItem));
	if (newItem == NULL){
		memoryError("Could not allocate memory for parameter");
		return;
	}

	newItem->data = var;
	newItem->next = NULL;

	if (List->First == NULL){
		List->First = newItem;
		newItem->prev = NULL;
	}
	else{
		List->Last->next = newItem;
		newItem->prev = List->Last;
	}

	List->Last = newItem;
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
	int paramsCnt = 0;
	int i;
	ParamsListItem * lastItem = self->params;
	while (lastItem) {
		lastItem = lastItem->next;
		paramsCnt++;
	}
	lastItem = self->params;
	for (i = -1 * paramsCnt; i < 0; i++) {
		lastItem->param->stackIndex = i;
		lastItem = lastItem->next;
	}
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

