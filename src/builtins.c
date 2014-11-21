#include "builtins.h"

// to make this use: gcc func.c errorHandler.c ifjTypes.h ifjTypes.c

int func_len(char *str) {

	return strlen(str);
}

void write(tIFJ type, void* a1) {
	switch (type) {
	case iInt:
		printf("%d", *((int *) a1));
		break;
	case iString:
		printf("%s", *(char **) a1);
		break;
	case iReal:
		printf("%f", *(float *) a1);
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

void change(char *str, int RIGHT) {
	char h = str[0];
	str[0] = str[RIGHT];
	str[RIGHT] = h;
	return;
}

void Sift(char *str, int LEFT, int RIGHT) {
	int i = LEFT;
	int j = (2 * i) + 1;
	char tmp = str[i];
	bool cond = j <= RIGHT;

	while (cond) {
		if (j < RIGHT)
			if (str[j] < str[j + 1])
				j++;
		if (tmp >= str[j])
			cond = false;
		else {
			str[i] = str[j];
			i = j;
			j = 2 * i + 1;
			cond = j <= RIGHT;
		}
	}
	str[i] = tmp;
	return;
}

char * func_sort(char *str) {
	int str_len = strlen(str);

	int i;
	int LEFT = str_len / 2 - 1;
	int RIGHT = str_len - 1;

	for (i = LEFT; i >= 0; i--) {
		Sift(str, i, RIGHT);
	}
	for (RIGHT = str_len - 1; RIGHT >= 1; RIGHT--) {
		change(str, RIGHT);
		Sift(str, 0, RIGHT - 1);
	}

	return str;
}
