#include "error_handler.h"

void Error_lex(Err_lex err) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Lexical) Line %d: last read = \"%s\" %s\n", err.line,
			err.lexBuff, err.msg);
	exit(1);
}

void Error_syntax(Err_syntax err) {
	fflush(stdout);
	if (err.actualTokenName)
		fprintf(stderr, "ERROR(Syntax) Line %d: %s, got \"%s\"\n", err.line,
				err.msg, err.actualTokenName);
	else
		fprintf(stderr, "ERROR(Syntax) Line %d: %s\n", err.line, err.msg);
	exit(2);
}

void Error_sem_definition(Err_def err) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Definition) Line %d: variable \"%s\" or function is undefined or you are atempting to redefine it.\n",
			err.line, err.symbolName);
	exit(3);
}
/*semanticka chyba typove kompatibility v aritmetickych, retezcových a relacnich
 vyrazech, prip. spatny pocet ci typ parametruu u volani funkce.
 dont forget about char * iVar_type2str(type)
 */
void Error_sem_Type(Err_type err) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type) Line %d: Variable or function of type %s does not fit to expression or function has bad parameters.\n",
			err.line, err.msg);
	exit(4);
}

void Error_sem_Cond(Err_cond err) {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Semantic/Type): Variable of type %s does not fit to condition.\n",
			err.cndtTypeStr);
	exit(4);
}

void Error_sem(Err_sem err) {
	fflush(stdout);
	fprintf(stderr, "ERROR(Semantic) Line: %d: %s .\n", err.line, err.msg);
	exit(5);
}

void Error_rt_readln() {
	fflush(stdout);
	fprintf(stderr,
			"ERROR(Runtime/readln): Error while reading value from user input.\n");
	exit(6);
}

void Error_rt_notInit(int stackAddr) {
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

