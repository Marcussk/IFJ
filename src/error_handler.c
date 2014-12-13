#include "error_handler.h"

void Error_lex(char * str, char * lexBuff, int lineNum) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Lexical) Line %d: last read = \"%s\" %s\n", lineNum,
			lexBuff, str);
	exit(1);
}

void Error_syntax(char * str, int lineNum, char * actualTokenName) {
	fflush(stdout);
	if (actualTokenName)
		fprintf(stderr, "ERROR(Syntax) Line %d: %s, got \"%s\"\n", lineNum, str,
				actualTokenName);
	else
		fprintf(stderr, "ERROR(Syntax) Line %d: %s\n", lineNum, str);
	exit(2);
}

void Error_sem_definition(int lineNum, char * varName) {
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
void Error_sem_Type(char * nameOfType, int lineNum) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type) Line %d: Variable or function of type %s does not fit to expression or function has bad parameters.\n",
			lineNum, nameOfType);
	exit(4);
}

void Error_sem_Cond(char * nameOfType) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type): Variable of type %s does not fit to condition.\n",
			nameOfType);
	exit(4);
}

void Error_sem(char * str, int lineNum) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Semantic) Line: %d: %s .\n", lineNum, str);
	exit(5);
}

void Error_rt_readlnNum() {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Runtime/readln): Error while reading value from user input.\n");
	exit(6);
}

void Error_rt_notInit() {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Runtime/notInit): Attempt to use unitialized variable.\n");
	exit(7);
}

void Error_rt_zeroDivision() {
	fflush(stdout);
	fprintf(stderr, "ERROR(Runtime/zeroDivision):.\n");
	exit(8);
}
void Error_rt(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Runtime): %s\n", str);
	exit(9);
}

void Error_unimplemented(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Unimplemented): %s .\n", str);
	exit(99);
}

void Error_memory(char * str) {
	fflush(stdout);
	fprintf(stderr, "ERROR(MemoryAllocation): %s .\n", str);
	exit(99);
}

