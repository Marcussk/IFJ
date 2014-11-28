#include "builtins.h"

// to make this use: gcc func.c errorHandler.c ifjTypes.h ifjTypes.c

int func_len(char *str) {
	return strlen(str);
}

void write(tIFJ type, iVal a1) {
	switch (type) {
	case iInt:
		printf("%d", a1.iInt);
		break;
	case iString:
		printf("%s", a1.iString);
		break;
	case iReal:
		printf("%f", a1.iReal);
		break;
	default:
		printf("Function write has performed error");
	}
}

/*
 * params: i- start of substring +1
 * 		   n - len of substring
 */
char * func_copy(char *f_str, int i, int n) {
	int strIndex;
	int offset = i - 1;
	if (i < 1)
		rt_error("Builtin function copy does not support i < 1");
	if (strlen(f_str) < (offset + n)) {
		rt_error(
				"Builtin function copy can't be performed because original string is too short\n");
	}
	char * newStr = malloc(i + 1 * sizeof(char));
	if (!newStr) {
		memoryError("func_copy can't alloc memory for newStr\n");
	}
	for (strIndex = 0; strIndex < n; strIndex++) {
		newStr[strIndex] = f_str[strIndex + offset];
	}
	newStr[strIndex] = '\0';
	return newStr;
}

int readLn(iVal *a1, tIFJ type) {
	char * str;
	switch (type) {
	case iInt:
		return scanf("%d", &(a1->iInt));
	case iReal:
		return scanf("%f", &(a1->iReal));
	case iString:
		str = calloc(sizeof(char), 256);
		a1->iString = str;
		// [TODO] 1. new string, 2. while ch != '\n'; String_append(ch); ch = getChar() // because strlen is unknown so array have to be dynamic
		return scanf("%s", a1->iString);
	case iChar:
		return scanf("%d", &(a1->iInt));
	case iBool:
		unimplementedError("readLn not implemented for bool");
		return 0;
	default:
		rt_readlnNumError();
		return 0;
	}
}

void registrBuiltins(HashTable * ht) {
	//funkcia, navratova hodnota, typ parametru
	regFn(ht, "readln", b_readLn, iVoid, iVoid);
	regFn(ht, "write", b_write, iVoid, iVoid);
	regFn(ht, "length", b_length, iInt, iString);
	regFn(ht, "copy", b_copy, iString, iString, iInt, iInt);
	regFn(ht, "find", b_find, iInt, iString, iString);
	regFn(ht, "sort", b_sort, iString, iString);
}

void regFn(HashTable * ht, char * name, Builtins b, tIFJ retTyp, int paramsCnt,
		...) {
	va_list valist;
	iVar * param = NULL;
	va_start(valist, paramsCnt);
	int i;
	// becouse ht returns place where it creates
	iVar * item = NULL;
	if((HashTable_insert(ht, name, &item) != ht_inserted)){
		unimplementedError("Redefinition of builtin function");
	}
	item->type = iFn;
	item->isInitialied = true;
	item->val.fn = iFunction__init__();
	item->val.fn->retType = retTyp;
	item->val.fn->builtin = b;

	for (i = 0; i < paramsCnt; i++) {
		param = malloc(sizeof(iVar));
		if (!param) {
			memoryError("Can't allocate memory for new parameter\n");
		}
		param->type = va_arg(valist, tIFJ);
		param->isInitialied = true;
		param->stackIndex = i;
		iFunction_addParam(item->val.fn, param);
	}
	va_end(valist);
}
