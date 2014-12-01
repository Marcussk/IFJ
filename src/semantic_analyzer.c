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