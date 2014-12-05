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
			printf("false");
		} else {
			printf("true");
		}
		break;
	default:
		rt_error("instr write used with incompatible type");
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
		a1->iInt = readint();
		return (a1->iInt);
	case iReal:
		return scanf("%f", &(a1->iReal));
	case iString:
		str = calloc(sizeof(char), 256);
		a1->iString = str;
		// [TODO] 1. new string, 2. while ch != '\n'; String_append(ch); ch = getChar() // because strlen is unknown so array have to be dynamic
		return scanf("%s", a1->iString);
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
		unimplementedError("Redefinition of builtin function");
	}
	item->type = iFn;
	item->isInitialized = true;
	item->val.fn = iFunction__init__();
	item->val.fn->retVal.type = retTyp;
	item->val.fn->builtin = b;

	for (i = -1 * paramsCnt; i < 0; i++) {
		param = malloc(sizeof(iVar));
		if (!param) {
			memoryError("Can't allocate memory for new parameter\n");
		}
		param->type = va_arg(valist, tIFJ);
		param->isInitialized = true;
		param->stackIndex = i;
		iFunction_addParam(item->val.fn, param, "");
	}
	va_end(valist);
}

int readint()
{
		int tmp = 0;
		int c;
		while ((c = getchar()) != '\n')
		{
			if (!isdigit(c - '0'))
			{
				rt_readlnNumError();
			}
			tmp *= 10;
			tmp += (c - '0');
		}
		printf("return %d \n", tmp);
		return tmp;
}
