#ifndef BUILTINS_h
#define BUILTINS_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorHandler.h"
#include "ifjTypes.h"
 
int readLn(iVar * var);

int func_len (char * str);
char *func_copy (char *f_str, int i, int n);

#endif