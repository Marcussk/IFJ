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
		printf("%g", a1.iReal);
		break;
	case iBool:
		if (a1.iInt == 0) {
			printf("FALSE");
		} else {
			printf("TRUE");
		}
		break;
	default:
		Error_rt("instr write used with incompatible type");
	}
}

/*
 * params: i- start of substring +1
 * 		   n - len of substring
 */
char * func_copy(char *f_str, int i, int n) {
	int strIndex;
	int offset = i - 1;
	if (offset < 0)
		Error_rt("Builtin function copy does not support i < 1");
	if (strlen(f_str) < (offset + n)) {
		Error_rt(
				"Builtin function copy can't be performed because original string is too short\n");
	}

	
	char * newStr = malloc((n + 1) * sizeof(char));

	if (!newStr) {
		Error_memory("func_copy can't alloc memory for newStr\n");
	}
	for (strIndex = 0; strIndex < n; strIndex++) {
		newStr[strIndex] = f_str[strIndex + offset];
	}
	newStr[strIndex] = '\0';
	return newStr;
}

int readLn(iVal *a1, tIFJ type) {
	char ch;
	String str;

	switch (type) {
	case iInt:
		return scanf("%d", &(a1->iInt));
	case iReal:
		return scanf("%f", &(a1->iReal));
	case iString:
		String__init__(&str, 16);
		while ((ch = getchar()) != '\n')
			String_append(&str, ch);

		a1->iString = str.buff;
		return 1;
	default:
		Error_rt_readln();
		return 0;
	}
}

void registrBuiltins(HashTable * ht) {
	//funkcia, navratova hodnota, typ parametru
	regFn(ht, "readln", b_readLn, iVoid, 1, iVoid);
	regFn(ht, "write", b_write, iVoid, 1, iVoid);
	regFn(ht, "length", b_length, iInt, 1, iString);
	regFn(ht, "copy", b_copy, iString, 3, iString, iInt, iInt);
	regFn(ht, "find", b_find, iInt, 2, iString, iString);
	regFn(ht, "sort", b_sort, iString, 1, iString);
}

void regFn(HashTable * ht, char * name, Builtins b, tIFJ retTyp, int paramsCnt,
		...) {
	va_list valist;
	va_start(valist, paramsCnt);
	iVar * param = NULL;
	int i;
	// becouse ht returns place where is variable created creates
	iVar * item = NULL;
	if ((HashTable_insertNew(ht, name, &item) != ht_inserted)) {
		Error_unimplemented("Redefinition of builtin function");
	}
	item->type = iFn;
	item->isGlobal = true;
	item->val.fn = iFunction__init__();
	item->val.fn->retVal.type = retTyp;
	item->val.fn->builtin = b;
	item->val.fn->bodyFound = true;
	item->val.fn->forwardFound = true;

	for (i = -1 * paramsCnt; i < 0; i++) {

		param = malloc(sizeof(iVar));
		if (!param) {
			Error_memory("Can't allocate memory for new parameter\n");
		}
		iVar__init__(param);
		param->type = va_arg(valist, tIFJ);
		param->stackIndex = i;
		iFunction_addParam(item->val.fn, param, "");
	}
	va_end(valist);
}
