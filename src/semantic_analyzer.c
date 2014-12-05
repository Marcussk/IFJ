#include "semantic_analyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2, int lineNum) {
	if (param1 == param2) {
		return;
	} else {
		printf("%d %d \n", param1, param2);
		sem_TypeError(iVar_type2str(param2), lineNum);
	}
}

void SemAnalyzer_checkcond(tIFJ param) {
	if(param != iBool ) {
		sem_CondError(iVar_type2str(param));
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
			memoryError(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam1->stackAddr = 0;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal,
						instrparam1, NULL, instrparam1 });

		instrparam2 = malloc(sizeof(iVar));
		if (!instrparam2) {
			memoryError(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam2->stackAddr = -1;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal, instrparam2,
							NULL, instrparam2 });
		return;	

	}
	if (( param1 == iInt) && (param2 == iReal)) {
		//convert param1
		instrparam1 = malloc(sizeof(iVar));
		if (!instrparam1) {
			memoryError(
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
			memoryError(
					"Can't allocate memory for new instruction parameter\n");
		}
		instrparam2->stackAddr = -1;
		InstrQueue_insert(self, (Instruction ) { i_int2real, iReal, instrparam2,
						NULL, instrparam2 });
	}
		return;
}
