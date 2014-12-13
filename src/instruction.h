#ifndef INSTRUCTION_h
#define INSTRUCTION_h

#include <string.h>
#include "ifj_types.h"
#include "queue.h"

// synergy with builtins
typedef enum {
	i_stop = 0, i_noop,		// does nothing
	i_jmp,  //skok vzdy
	i_jmpz, //podmineny skok pri nepravdive podmince
	i_pop,
	i_push,
	i_assign, //5
	i_add,
	i_sub,
	i_mul,
	i_div,
	i_int2real, //10
	i_equal,
	i_nequal,
	i_less,
	i_more,
	i_loreq, //15
	i_moreq,
	i_call,
	i_return,
	i_neg,

	//funkce ze zadani
	i_readln = 21,
	i_write,
	i_find,
	i_copy,
	i_len,
	i_sort,

	i_not

} InstrCode;

/*
 * type can be reference, in that case operands are indexes to interpreter stack
 * dest is always reference if dest.stackAddr <0 push result on stack
 *
 * */
typedef struct s_Instruction {
	InstrCode code;      //kod instrukcie z InstrCode
	tIFJ type;           //datovy typ
	union u_InstrParam * a1;     //operand1
	union u_InstrParam * dest;   //vysledok operace
} Instruction;

typedef union u_InstrParam {
	int stackAddr;
	float iReal;
	int iInt;
	char * iString;
} InstrParam;

QUEUE_DECLARE(Instr, Instruction)


void InstrQueue__dell__(InstrQueue * self);

iVal InstrP2iVal(InstrParam * a, tIFJ type);
InstrParam inline iVal2InstrP(iVal v, tIFJ type);
void InstrQueue_debug(InstrQueue * self);
InstrCode Token2Instruction(Token token);
char * instr2Str(InstrCode code);

#endif
