#include "instruction.h"

QUEUE_DEFINE(Instr, Instruction)

void InstrParamDebug(InstrParam * p, char * name){
	if(!p){
		printf("%s: %p", name, (void *) p );
	}else{
		printf("%s: %d", name , p->iInt);
	}
}

void InstrQueue_debug(InstrQueue * self) {
	Instruction instr;
	int i;

	for(i =0; i < self->size; i++) {
		instr = *InstrQueue_atIndex(self, i);
		if (self->actual == i)
			printf("->");
		else
			printf("  ");
		printf("%d code %s type %s, ", i, instr2Str(instr.code),
				iVar_type2str(instr.type));
		InstrParamDebug(instr.a1, "a1");
		InstrParamDebug(instr.a2, ", a2");
		InstrParamDebug(instr.dest, ", a2");
		printf("\n");
	}
}

void InstrQueue__dell__(InstrQueue * self) {
	int i;
	for(i =0; i < self->size; i++) {
		// [TODO] clear instr
	}
	free(self->QueueArr);
}



InstrParam inline iVal2InstrP(iVal v, tIFJ type) {
	InstrParam p;
	switch (type) {
	case iBool:
	case iInt:
		p.iInt = v.iInt;
		break;
	case iString:
		p.iString = v.iString;
		break;
	case iReal:
		p.iReal = v.iReal;
		break;
	default:
		unimplementedError(
				"Unimplemented cast in InstrP2iVal for the interpret\n");
	}
	return p;
}

iVal inline InstrP2iVal(InstrParam * a, tIFJ type) {
	iVal v;
	if (!a) {
		v.iInt = 0;
		return v;
	}
	switch (type) {
	case iBool:
	case iInt:
		v.iInt = a->iInt;
		break;
	case iString:
		v.iString = a->iString;
		break;
	case iReal:
		v.iReal = a->iReal;
		break;
	default:
		unimplementedError(
				"Unimplemented cast in InstrP2iVal for the interpret\n");
	}

	return v;
}

char * instr2Str(InstrCode code) {
	switch (code) {
	case i_stop:
		return "stop";
	case i_noop:
		return "noop";
	case i_jmp:
		return "jmp";
	case i_jmpz:
		return "jmpz";
	case i_push:
		return "push";
	case i_pop:
		return "pop";
	case i_assign:
		return "assign";
	case i_add:
		return "add";
	case i_sub:
		return "sub";
	case i_mul:
		return "mul";
	case i_div:
		return "div";
	case i_int2real:
		return "int2real";
	case i_equal:
		return "equal";
	case i_nequal:
		return "nequal";
	case i_less:
		return "less";
	case i_more:
		return "more";
	case i_loreq:
		return "loreq";
	case i_moreq:
		return "moreq";
	case i_call:
		return "call";
	case i_return:
		return "return";
	case i_readln:
		return "readln";
	case i_write:
		return "write";
	case i_find:
		return "find";
	case i_copy:
		return "copy";
	case i_len:
		return "len";
	case i_sort:
		return "sort";
	default:
		return "unknown instr";
	}
}

InstrCode Token2Instruction(Token token) {
	switch (token) {
	case t_plus:
		return i_add;
	case t_minus:
		return i_sub;
	case t_asterisk:
		return i_mul;
	case t_slash:
		return i_div;
	case t_less:
		return i_less;
	case t_greater:
		return i_more;
	case t_lessOrEqv:
		return i_loreq;
	case t_greaterOrEqv:
		return i_moreq;
	case t_eqv:
		return i_equal;
	case t_notEqv:
		return i_nequal;
	default:
		return i_noop;
	}
}
