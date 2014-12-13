#include "buff_file.h"

int BuffFile__init__(BuffFile * self, char * filename) {
	self->filename = filename;
	self->input = fopen(filename, "r");
	if(!self->input )
		return false;

	self->backup = EOF;
	self->line = 0;
	self->column = 0;
	return true;
}
char BuffFile_get(BuffFile * self) {
	char ret;
	if (self->backup != EOF) {
		ret = self->backup;
		self->backup = EOF;
	} else {
		ret = fgetc(self->input);
	}
	if (ret == '\n'){
		self->line++;
		self->column = 0;
	}else
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

void BuffFile__dell__(BuffFile * self) {
	fclose(self->input);
}
