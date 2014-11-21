#ifndef INSTRUCTION_h
#define INSTRUCTION_h

#include "ifjTypes.h"

typedef enum {
	STOP = 0, NOOP,		// does nothing
	JMP,  //skok pri pravdive podmince
	JMPZ, // skok pri nepravdive podmince
	CALL,
	RETURN,
	WRITE,
	READ,
	ASSIGN,
	ADD,
	SUB,
	MUL,
	DIV,
	CONCAT, //only string!!
	EQUAL,
	NOT_EQUAL,
	LESS,
	MORE,
	LESS_or_EQUAL,
	MORE_or_EQUAL,

	//funkce ze zadani
	FIND,
	COPY,
	LENGTH,
	SORT

} InstrCode;

typedef struct {
	InstrCode code;  //klucove slovo instrukcie
	tIFJ type;           //datovy typ
	void *a1;           //operand1
	void *a2;           //operand2
	void *dest;           //medzivysledok
} Instruction;

#endif
