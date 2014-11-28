#ifndef INSTRUCTION_h
#define INSTRUCTION_h

#include "ifjTypes.h"
#include <string.h>

// synergy with builtins
typedef enum {
	i_stop = 0, i_noop,		// does nothing
	i_jmp,  //skok pri pravdive podmince
	i_jmpz, // skok pri nepravdive podmince
	i_push,
	i_call,
	i_assign,
	i_add,
	i_sub,
	i_mul,
	i_div,
	i_int2real,
	i_equal,
	i_nequal,
	i_less,
	i_more,
	i_loreq,
	i_moreq,

	//funkce ze zadani
	i_readln = 20,
	i_write,
	i_find,
	i_copy,
	i_len,
	i_sort

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
void InstrQueue__dell__(InstrQueue * self);

iVal InstrP2iVal(InstrParam * a, tIFJ type);
InstrParam inline iVal2InstrP(iVal v, tIFJ type);
void InstrQueue_debug(InstrQueue * self);

#endif
