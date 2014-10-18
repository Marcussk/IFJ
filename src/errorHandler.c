#include "errorHandler.h"

void memoryError(char * str) {
	fprintf(stderr, "ERROR(memory):%s\n", str);
	exit(99);
}

void syntaxError(char * str, int lineNum) {
	fprintf(stderr, "ERROR(syntax) Line %d: %s", lineNum, str);
	exit(1);
}
