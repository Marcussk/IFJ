#include "ifj_types.h"

iVar * iVar__init__() {
	iVar * self = malloc(sizeof(iVar));
	if (!self) {
		Error_memory("Can't allocate memory for iVar");
	}
	self->isGlobal = false;
	self->type = iUnknown;
	self->val.iInt = 0;
	self->stackIndex = -1;
	return self;
}

tIFJ Token_getType(Token t) {
	switch (t) {
	case t_num_int:
		return iInt;
		break;
	case t_num_real:
		return iReal;
		break;
	case t_str_val:
		return iString;
		break;
	case t_true:
	case t_false:
		return iBool;
	default:
		Error_unimplemented("Can't convert token to type");
	}
	return iUnknown;
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
	case iStackRef:
		return "iStackRef";
	default:
		return "unknown";
	}
}

void iVar_debug(iVar * v) {
	if (!v)
		printf("Null variable\n");
	else {
		printf("<iVar p:%p, type:%s, global:%d>\n", (void *) v,
				iVar_type2str(v->type), v->isGlobal);
	}
}

void iVar__dell__(iVar * self) {
	if (self->type == iString)
		free(self->val.iString);
	free(self);
}
