#include "strRoutines.h"

void String__init__(String * self, int prealocatedLen) {
	self->buff = calloc(prealocatedLen, sizeof(char));
	if (!self->buff)
		memoryError("Can't alloc memory for string");

	self->len = 0;
	self->prealloc = prealocatedLen;
}

void String__dell_(String * self) {
	free(self->buff);
	self->len = 0;
	self->prealloc = 0;
}

void String_append(String * self, char ch) {
	if (self->prealloc - 1 > self->len + 1) {
		self->buff[self->len] = ch;
		self->len += 1;
	} else {
		//realloc
		int i;
		int newPrealloc = self->prealloc * 2;
		self->buff = realloc(self->buff, newPrealloc);
		if (!self->buff)
			memoryError("Can't realloc string buffer while appending char");

		for (i = self->prealloc; i < newPrealloc; i++)
			self->buff[i] = 0;
		self->prealloc = newPrealloc;

		String_append(self, ch);
	}
}

void String_appendStr(String* a, String *b) {
	int i;
	for (i = 0; i < b->len; i++) {
		String_append(a, b->buff[i]);
	}
}

void String_clear(String * self) {
	int i;
	for (i = 0; i < self->len; i++) {
		self->buff[i] = 0;
	}
	self->len = 0;
}
