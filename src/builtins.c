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
		printf("error");
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
		rt_error("Builtin function copy does not support i <1");
	if (strlen(f_str) < (offset + n)) {
		rt_error(
				"Builtin function copy cano't be performed because original string is too short\n");
	}
	char * newStr = malloc(i + 1 * sizeof(char));
	for (strIndex = 0; strIndex < n; strIndex++) {
		newStr[strIndex] = f_str[strIndex + offset];
	}
	newStr[strIndex] = '\0';
	return newStr;
}

int readLn(iVar *var) {
	switch (var->type) {
	case iInt:
		return scanf("%d", &var->val.iInt);
	case iReal:
		return scanf("%f", &var->val.iReal);
	case iString:
		return scanf("%s", var->val.iString);
	case iChar:
		return scanf("%d", &var->val.iInt);
	case iBool:
		sem_TypeError(iVar_type2str(var->type));
		return 0;
	default:
		rt_readlnNumError();
		return 0;
	}
}

/* changing first and last elemnts*/
void change(char *str, int right) {
	char h = str[0];
	str[0] = str[right];
	str[right] = h;
	return;
}

/* *str - zoradovany retazec */
void Sift(char *str, int left, int right) {
	int i = left;
	int j = (2 * i) + 1; /* lavy syn */
	char tmp = str[i];	
	bool cond = j <= right; /* podmienka vymeny prvkov */

	while (cond) {
		if (j < right) /* strom ma oboch synov */
			if (str[j] < str[j + 1])
				j++;
		if (tmp >= str[j])  
			cond = false;
		else {
			str[i] = str[j]; /* prepisanie prvku na zlom mieste */
			i = j;			/* pokracovanie na nizsi uzol stromu */
			j = 2 * i + 1; /* ustanovenie praveho syna */
			cond = j <= right; /* ak j > najpravejsi prvok right, ziadny dalsi syn neexistuje => koneic pola */
		}
	}
	str[i] = tmp;
	return;
}

/* strom implementovany polom kde lavy syn je na pozici 2i+1, a pravy syn na pozicii 2i+2 */
char * func_sort(char *str) {
	int str_len = strlen(str);

	int i;
	int left = str_len / 2 - 1; /* index najpravejsieho uzlu na najnizsej urovni */
	int right = str_len - 1;  /* posleny prvok pola */

	for (i = left; i >= 0; i--) {
		Sift(str, i, right);	
	}
	for (right = str_len - 1; right >= 1; right--) {
		change(str, right); /* vymena prvkov */		 		
		Sift(str, 0, right - 1); /* opakovane zostavenie stromu z prvkov ktore este niesu zoradene */
	}

	return str;
}

void registrBuiltins(HashTable * ht) {
	iVar * item = NULL;
	//pro všechny jmena HashTable_insert(ht, jmeno, &item);
	HashTable_insert(ht, "readln", &item);
	HashTable_insert(ht, "write", &item);
	HashTable_insert(ht, "length", &item);
	HashTable_insert(ht, "copy", &item);
	HashTable_insert(ht, "sort", &item);
	HashTable_insert(ht, "find", &item);
}
