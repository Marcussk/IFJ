#include "semantic_analyzer.h"

void SemAnalyzer_checktypes(tIFJ param1, tIFJ param2) {
	if (param1 == param2) {	
		return;
	}
	else
	{
		sem_TypeError(iVar_type2str(param2));
	}
}

void SemAnalyzer_checkcond(tIFJ param) {
	if(param != 16 ) {
		sem_CondError(iVar_type2str(param));
	}
	//printf("Condition: %s\n",iVar_type2str(param) );
}