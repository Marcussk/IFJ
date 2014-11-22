#include "ifjTypes.h"

iVar * iVar__init__() {
	iVar * self = malloc(sizeof(iVar));
	if (!self) {
		memoryError("Can't allocate memory for iVar");
	}
	self->isInitialied = false;
	self->type = iUnknown;
	self->val.iInt = 0;
	self->stackIndex = -1;
	return self;
}

char * iVar_type2str(tIFJ t) {
	switch (t) {
	case iVoid:
		return "void";
	case iInt:
		return "int";
	case iString:
		return "string";
	case iChar:
		return "char";
	case iReal:
		return "real";
	case iFn:
		return "fn";
	default:
		return "unknown";
	}
}

void iVar_debug(iVar * v) {
	if (!v)
		printf("Null variable\n");
	else {
		printf("<iVar p:%p, type:%s, initialised:%d>\n", (void *) v,
				iVar_type2str(v->type), v->isInitialied);
	}
}

void iVar__dell__(iVar * self) {
	if (self->isInitialied) {
		if (self->type == iString)
			free(self->val.iString);
		else if (self->type == iFn)
			iFunction__dell__(self->val.fn);
	}

	free(self);
}

iFunction * iFunction__init__() {
	iFunction * self = malloc(sizeof(iFunction));
	self->body = NULL;
	self->params = NULL;
	self->retType = iUnknown;

	return self;
}

ParamsListItem * ParamsListItem__init__() {
	ParamsListItem * self = malloc(sizeof(ParamsListItem));
	self->param = NULL;
	self->next = NULL;
	return self;
}

void iFunction_addParam(iFunction * self, iVar * var) {
	ParamsListItem * lastItem = self->params;
	if (!self->params) {
		self->params = ParamsListItem__init__();
		lastItem = self->params;
	} else {
		while (lastItem->next)
			lastItem = lastItem->next;
		lastItem = ParamsListItem__init__();
	}
	lastItem->param = var;
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

