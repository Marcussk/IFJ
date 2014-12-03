#include "buff_file.h"

void BuffFile__init__(BuffFile * self, FILE * f) {
	self->backup = EOF;
	self->input = f;
}

char BuffFile_get(BuffFile * self) {
	char ret;
	if (self->backup != EOF) {
		ret = self->backup;
		self->backup = EOF;
		return ret;
	} else {
		return fgetc(self->input);
	}
}

void BuffFile_pushBack(BuffFile * self, char ch) {
	if (self->backup != EOF)
		memoryError("Canot push second char back to file\n");
	self->backup = ch;
}
