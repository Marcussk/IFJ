#include "buff_file.h"

void BuffFile__init__(BuffFile * self, FILE * f) {
	self->backup = EOF;
	self->input = f;
	self->line = 0;
	self->column = 0;
}
char BuffFile_get(BuffFile * self) {
	char ret;
	if (self->backup != EOF) {
		ret = self->backup;
		self->backup = EOF;
	} else {
		ret = fgetc(self->input);
	}
	if (ret == '\n')
		self->line++;
	else
		self->column++;
	return ret;
}
void BuffFile_pushBack(BuffFile * self, char ch) {
	if (self->backup != EOF)
		Error_memory("Canot push second char back to file\n");
	self->backup = ch;
	if (ch == '\n')
		self->line--;
	else
		self->column--;
}
