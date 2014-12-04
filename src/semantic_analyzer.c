#include "semantic_analyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2) {
	if (param1 == param2) {
		return;
	} else {
		sem_TypeError(iVar_type2str(param2));
	}
}

void SemAnalyzer_checkcond(tIFJ param) {
	if (param != 16) {
		sem_CondError(iVar_type2str(param));
	}
	//printf("Condition: %s\n",iVar_type2str(param) );
}

void SemAnalyzer_typeconvert(InstrQueue * self, tIFJ param1, tIFJ param2) {
	//int real
	InstrParam * instrparam;
	if ((param1 == 17) && (param2 == 18)) {
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
	// real int
	if ((param1 == 18) && (param2 == 17)) {
		//convert param2
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
