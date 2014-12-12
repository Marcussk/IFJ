#include "semantic_analyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2, int lineNum) {
	if (param1 != param2) {
		Error_sem_Type(iVar_type2str(param2), lineNum);
	}
}

void SemAnalyzer_checkcond(tIFJ param) {
	if(param != iBool ) {
		Error_sem_Cond(iVar_type2str(param));
	}
}


// [TODO] : Prepisat
void SemAnalyzer_typeconvert(InstrQueue * self, tIFJ param1, tIFJ param2, Token operator) {
	InstrParam * instrparam1;
	InstrParam * instrparam2;
	if (operator == t_slash)
	{
		//convert param1
		instrparam1 = malloc(sizeof(iVar));
		if (!instrparam1) {
			Error_memory(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam1->stackAddr = 0; // Tady nesmi byt 0, ale vrchol zasobniku
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal,
						instrparam1, NULL, instrparam1 });

		instrparam2 = malloc(sizeof(iVar));
		if (!instrparam2) {
			Error_memory(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam2->stackAddr = -1; // Tady zase vrchol zasobniku - 1
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal, instrparam2,
							NULL, instrparam2 });
		return;	

	}
	if (( param1 == iInt) && (param2 == iReal)) {
		//convert param1
		instrparam1 = malloc(sizeof(iVar));
		if (!instrparam1) {
			Error_memory(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam1->stackAddr = 0;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal,
						instrparam1, NULL, instrparam1 });
		return;
	}
	
	if ((param1 == iReal) && (param2 == iInt)) {
		instrparam2 = malloc(sizeof(iVar));
		if (!instrparam2) {
			Error_memory(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam2->stackAddr = -1;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal, instrparam2,
						NULL, instrparam2 });
	}
		return;
}
