#include "builtins.h"
#include "ifjTypes.h"
#include "errorHandler.h"

// to make this use: gcc func.c errorHandler.c ifjTypes.h ifjTypes.c

int func_len(char *str) {

	return strlen(str);
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

int readLn(iVar *var){
	switch (var->type) {
	case iInt:
		scanf("%d", &var->val.iInt);
		break;
	case iReal:
		scanf("%f", &var->val.iReal);
		break;
	case iString:
		scanf("%s", var->val.iString);
		break;
	case iChar:
		scanf("%d", &var->val.iInt);
		break;		
	case iBool:
		sem_TypeError(var);
		return 0;						
	default:
		rt_readlnNumError(var);
		return 0;
		}
	return 1;
}
