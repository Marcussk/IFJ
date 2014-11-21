#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "ifjTypes.h"

int readLn(iVar * var);
void write(tIFJ type, void* a1);
int func_len(char * str);

char *func_copy(char *f_str, int i, int n);

void Sift(char *str, int LEFT, int RIGHT);
void change(char *str, int RIGHT);
char *func_sort(char *str);

#endif
