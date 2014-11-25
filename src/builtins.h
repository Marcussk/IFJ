#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "ifjTypes.h"
#include "hashTable.h"
#include <stdarg.h>

typedef enum {
	b_readLn, b_write, b_func_len, b_func_copy, b_func_sort
} builtins; 			// dont forget add func_find

void registrBuiltins(HashTable * ht);

int readLn(iVal *a1, tIFJ type);

void write(tIFJ type, iVal a1);
int func_len(char * str);

char *func_copy(char *f_str, int i, int n);

char *func_sort(char *str);

int func_find ( char *src, char *search );
int alfabeth_len(int src_len, char *src );
void bad_char(char *pattern, int bad_character[], int max_src_len );
void  good_suffixes( char *pattern, int good_suffix[]);
void suffixes(char* pattern, int suffix[], int max_pattern_len);
void regFn(HashTable * ht, char * name, tIFJ retTyp, int paramsCnt,...);

#endif
