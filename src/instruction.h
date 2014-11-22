#ifndef INSTRUCTION_h
#define INSTRUCTION_h

#include "ifjTypes.h"

typedef enum {
	STOP = 0, NOOP,		// does nothing
	JMP,  //skok pri pravdive podmince
	JMPZ, // skok pri nepravdive podmince
	i_push,
	CALL,
	RETURN,
	i_write,
	READ,
	i_assign,
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

typedef union {
	int stackAddr;
	float iReal;
	int iInt;
	char * iString;
} InstrParam;

/*
 * type can be reference, in that case operands are indexes to interpreter stack
 * dest is always reference if dest.stackAddr <0 push result on stack
 *
 * */
typedef struct {
	InstrCode code;      //kod instrukcie z InstrCode
	tIFJ type;           //datovy typ
	InstrParam * a1;     //operand1
	InstrParam * a2;     //operand2
	InstrParam * dest;   //vysledok operace
} Instruction;

typedef struct s_InstrQueueNode {
	Instruction val;
	struct s_InstrQueueNode * next;
} InstrQueueNode;

typedef struct {
	InstrQueueNode * first;
	InstrQueueNode * actual;
} InstrQueue;

void InstrQueue__init__(InstrQueue * self);
Instruction * InstrQueue_next(InstrQueue * self);
void InstrQueue_insert(InstrQueue * self, Instruction i);
Instruction * InstrQueue_atIndex(InstrQueue * self, int index);


#endif
