#include "instruction.h"

//vytvari novou instrukci
Instruction Inst_Create(Instructions code, int type,  void *a1,  void *a2,  void *a3)
{
	Instruction I;
	I->code = code;
	I->type = type;
	I->a1 = a1;
	I->a2 = a2;
	I->a3 = a3;
	return I;
}
