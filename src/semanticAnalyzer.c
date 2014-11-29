#include "semanticAnalyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2) {
	if (param1 == param2) {
		printf("SemAnalyze OK %d %d \n", param1, param2);	
		return;
	}
	else
	{
		printf("SemAnalyze WRONG %d %d \n", param1, param2);
		sem_TypeError(iVar_type2str(param2));
	}
}