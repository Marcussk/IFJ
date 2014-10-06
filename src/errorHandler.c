#include "errorHandler.h"

void memoryError(char * str) {
	fprintf(stderr, "ERROR(memory):%s\n", str);
	exit(99);
}
