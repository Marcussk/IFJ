#include "errorHandler.h"

void lexError(char * str, char * lexBuff, int lineNum) {
	fprintf(stderr, "ERROR(Lexical) Line %d: last readed = \"%s\" %s", lineNum,
			lexBuff, str);
	exit(1);
}

void syntaxError(char * str, int lineNum) {
	fprintf(stderr, "ERROR(Syntax) Line %d: %s", lineNum, str);
	exit(2);
}

void sem_definitionError(char * varName, int lineNum) {
	fprintf(stderr,
			"ERROR(Semantic/Definition) Line %d: variable or function \"%s\" is not defined or you are atempting to redefine it.\n",
			lineNum, varName);
	exit(3);
}
/*semanticka chyba typove kompatibility v aritmetickych, retezcových a relacnich
 vyrazech, prip. spatny pocet ci typ parametruu u volani funkce.*/
void sem_TypeError(iVar * var) {
	fprintf(stderr,
			"ERROR(Semantic/Type): Variable or function \"%s\" of type %s does not fit to expression or function have bad parameters.\n",
			var->name, iVar_type2str(var->type));
	exit(4);
}

void sem_Error(char * str) {
	fprintf(stderr, "ERROR(Semantic): %s .\n", str);
	exit(5);
}

void rt_readlnNumError(iVar * var) {
	if (var->name) {
		fprintf(stderr,
				"ERROR(Runtime/readln): Error while reading value for variable %s from user input.\n",
				var->name);
	} else {
		fprintf(stderr,
				"ERROR(Runtime/readln): Error while reading value for unnamed variable from user input.\n");
	}
	exit(6);
}

void rt_notInitError(iVar * var) {
	if (var->name) {
		fprintf(stderr,
				"ERROR(Runtime/notInit): Variable %s have to be initialized before is used.\n",
				var->name);
	} else {
		fprintf(stderr,
				"ERROR(Runtime/notInit): Attempt to use not itialized unnamed variable.\n");
	}
	exit(7);
}

void rt_zeroDivisionError() {
	fprintf(stderr, "ERROR(Runtime/zeroDivision):.");
	exit(8);
}
void rt_error(char * str) {
	fprintf(stderr, "ERROR(Runtime): Undescribed runtime error.");
	exit(9);
}

void unimplementedError(char * str){
	fprintf(stderr, "ERROR(Not implemented): %s .\n", str);
	exit(99);
}

void memoryError(char * str) {
	fprintf(stderr, "ERROR(MemoryAllocation): %s .\n", str);
	exit(99);
}

