#include "ifj_types.h"

iVar * iVar__init__() {
	iVar * self = malloc(sizeof(iVar));
	if (!self) {
		memoryError("Can't allocate memory for iVar");
	}
	self->isInitialized = false;
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
		unimplementedError("cannot convert token to type");
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
				iVar_type2str(v->type), v->isInitialized);
	}
}

void iVar__dell__(iVar * self) {
	if (self->isInitialized) {
		if (self->type == iString)
			free(self->val.iString);
		/*else if (self->type == iFn)
		 iFunction__dell__(self->val.fn);
		 */
	}

	free(self);
}

iVal str2iVal(char * str, Token token, int lineNum) {
	iVal val;
	switch (token) {
	case t_num_int:
		if (!sscanf(str, "%d", &(val.iInt)))
			lexError("Cannot parse int num", str, lineNum);
		break;

	case t_num_real:
		if (!sscanf(str, "%f", &(val.iReal)))
			lexError("Cannot parse real num", str, lineNum);
		break;

	case t_str_val:
		val.iString = strdup(str);
		if (!val.iString)
			lexError("Cannot parse string", str, lineNum);
		break;
	case t_true:
		val.iInt = 1;
		break;
	case t_false:
		val.iInt = 0;
		break;
	default:
		lexError("cannot convert value", str, lineNum);
	}
	return val;
}

