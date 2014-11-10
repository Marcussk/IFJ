#include "ifjTypes.h"

iVar * iVar__init__( char * name) {
	iVar * self = malloc(sizeof(iVar));
	self->isInitialied = false;
	self->name = name;
	self->type = iUnknown;
	self->val.iInt = 0;
	return self;
}

void iFn__dell__(iFunction * fn) {

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
