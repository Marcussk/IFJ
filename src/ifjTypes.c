#include "ifjTypes.h"

iVar * iVar__init__(char * name) {
	iVar * self = malloc(sizeof(iVar));
	if (!self) {
		memoryError("Cant alloc memory for iVar");
	}
	self->isInitialied = false;
	self->name = name;
	self->type = iUnknown;
	self->val.iInt = 0;
	return self;
}

void iFn__dell__(iFunction * fn) {

}

char * iVar_type2str(tIFJ t) {
	switch (t) {
	case iVoid:
		return "void";
	case iInt:
		return "int";
	case iString:
		return "string";
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
		if (!v->name)
			printf("<iVar p:%p without name! >\n", (void *) v);
		else
			printf("<iVar p:%p, name:%s, type:%s, initialised:%d>\n", (void *) v,
					v->name, iVar_type2str(v->type), v->isInitialied);
	}
}

void iVar__dell__(iVar * self) {
	if (self->isInitialied) {
		if (self->type == iString)
			free(self->val.iString);
		else if (self->type == iFn)
			iFn__dell__(self->val.fn);
	}

	free(self->name);
	free(self);
}
