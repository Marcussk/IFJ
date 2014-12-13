#ifndef BUFF_FILE_h
#define BUFF_FILE_h
/*
 * file with pushBah back
 */
#include <stdio.h>
#include <stdbool.h>
#include "error_handler.h"


typedef struct {
	char * filename;
	FILE * input;
	int line;
	int column;
	char backup;
}BuffFile;


int BuffFile__init__(BuffFile * self, char * filename);
char BuffFile_get(BuffFile * self);
void BuffFile_pushBack(BuffFile * self,char ch);
void BuffFile__dell__(BuffFile * self);

#endif
