#include "semantic_analyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2) {
	if (param1 == param2) {
		return;
	} else {
		sem_TypeError(iVar_type2str(param2));
	}
}

void SemAnalyzer_checkcond(tIFJ param) {
	if(param != iBool ) {
		sem_CondError(iVar_type2str(param));
	}
}

void SemAnalyzer_typeconvert(InstrQueue * self, tIFJ param1, tIFJ param2) {

	InstrParam * instrparam;
	if (( param1 == iInt) && (param2 == iReal)) {
		//convert param1
		instrparam = malloc(sizeof(iVar));
		if (!instrparam) {
			memoryError(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam->stackAddr = 0;
		InstrQueue_insert(*(&self), (Instruction ) { i_int2real, iInt,
						instrparam, NULL, instrparam });
	}
	
	if ((param1 == iReal) && (param2 == iInt)) {
		instrparam = malloc(sizeof(iVar));
		if (!instrparam) {
			memoryError(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam->stackAddr = -1;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iInt, instrparam,
						NULL, instrparam });
	}

}
