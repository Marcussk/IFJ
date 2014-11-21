#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "ifjTypes.h"
#include "hashTable.h"

 typedef enum { readLn, write, func_len, func_copy, func_sort } builtins;// dont forget add func_find

 void registrBuiltins(HashTable * ht);

 int readLn(iVar * var);
 void write(tIFJ type, void* a1, void* a2, void*a3);
 int func_len(char * str);

 char *func_copy(char *f_str, int i, int n);

 void Sift(char *str, int LEFT, int RIGHT);
 void change(char *str, int RIGHT);
 char *func_sort(char *str);

#endif
