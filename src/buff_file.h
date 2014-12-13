#ifndef BUFF_FILE_h
#define BUFF_FILE_h
/*
 * file with pushBah back
 */
#include <stdio.h>

#include "error_handler.h"

typedef struct {
	FILE * input;
	int line;
	int column;
	char backup;
}BuffFile;


void BuffFile__init__(BuffFile * self, FILE * f);
char BuffFile_get(BuffFile * self);
void BuffFile_pushBack(BuffFile * self,char ch);

#endif
