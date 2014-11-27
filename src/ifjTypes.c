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

