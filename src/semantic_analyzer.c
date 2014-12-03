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

void SemAnalyzer_typeconvert (tIFJ param1, tIFJ param2) {
	//int real
	if (( param1 == 17) && (param2 == 18)) {
		//convert param1
	}
	
	//real int
	if (( param1 == 18) && (param2 == 17)) {
		//convert param2
	}

}