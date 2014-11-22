#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "ifjTypes.h"
#include "hashTable.h"

typedef enum {
	b_readLn, b_write, b_func_len, b_func_copy, b_func_sort
} builtins; 			// dont forget add func_find

void registrBuiltins(HashTable * ht);

int readLn(iVar * var);

void write(tIFJ type, void* a1);
int func_len(char * str);

char *func_copy(char *f_str, int i, int n);

char *func_sort(char *str);

#endif
