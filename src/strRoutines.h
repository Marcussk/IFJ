#ifndef STR_ROUTINES_h
#define STR_ROUTINES_h

#include <ctype.h>
#include <stdlib.h>
#include "errorHandler.h"

typedef struct {
	char * buff;
	int len;
	int prealloc;
} String;

void String__init__(String * self, int prealocatedLen);
void String_append(String * self, char ch);
void String_appendStr(String* a, String *b);
void String_clear(String * self);
void String__dell_(String * self);


#endif
