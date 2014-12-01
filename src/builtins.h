#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ial.h"
#include <stdarg.h>
#include "error_handler.h"
#include "hash_table.h"
#include "i_function.h"
#include "ifj_types.h"

void registrBuiltins(HashTable * ht);

int readLn(iVal *a1, tIFJ type);

void write(tIFJ type, iVal a1);
int func_len(char * str);

char *func_copy(char *f_str, int i, int n);

void regFn(HashTable * ht, char * name, Builtins b, tIFJ retTyp, int paramsCnt,
		...);

#endif
