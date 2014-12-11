#include "error_handler.h"

void lexError(char * str, char * lexBuff, int lineNum) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Lexical) Line %d: last read = \"%s\" %s\n", lineNum,
			lexBuff, str);
	exit(1);
}

void syntaxError(char * str, int lineNum, char * actualTokenName) {
	fflush(stdout);
	if (actualTokenName)
		fprintf(stderr, "ERROR(Syntax) Line %d: %s, got %s\n", lineNum, str,
				actualTokenName);
	else
		fprintf(stderr, "ERROR(Syntax) Line %d: %s\n", lineNum, str);
	exit(2);
}

void sem_definitionError(int lineNum, char * varName) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Definition) Line %d: variable \"%s\" or function is undefined or you are atempting to redefine it.\n",
			lineNum, varName);
	exit(3);
}
/*semanticka chyba typove kompatibility v aritmetickych, retezcových a relacnich
 vyrazech, prip. spatny pocet ci typ parametruu u volani funkce.
 dont forget about char * iVar_type2str(type)
 */
void sem_TypeError(char * nameOfType, int lineNum) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type) Line %d: Variable or function of type %s does not fit to expression or function has bad parameters.\n",
			lineNum, nameOfType);
	exit(4);
}

void sem_CondError(char * nameOfType) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type): Variable type %s does not fit to condition.\n",
			nameOfType);
	exit(4);
}

void sem_Error(char * str, int lineNum) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Semantic) Line: %d: %s .\n", lineNum, str);
	exit(5);
}

void rt_readlnNumError() {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Runtime/readln): Error while reading value from user input.\n");
	exit(6);
}

void rt_notInitError() {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Runtime/notInit): Attempt to use unitialized variable.\n");
	exit(7);
}

void rt_zeroDivisionError() {
	fflush(stdout);
	fprintf(stderr, "ERROR(Runtime/zeroDivision):.\n");
	exit(8);
}
void rt_error(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Runtime): %s\n", str);
	exit(9);
}

void unimplementedError(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Not implemented): %s .\n", str);
	exit(99);
}

void memoryError(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(MemoryAllocation): %s .\n", str);
	exit(99);
}

