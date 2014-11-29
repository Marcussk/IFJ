
#include "DebugTestStack.h"


#define RANDOM_NUM 15


iVal *get_new_iVal(){
	iVal new_iVal = malloc(sizeof(iVal));
	new_iVal.iInt = RANDOM_NUM;
	return &(new_iVal);
}

void test_stack1(void){
	Stack *s;
	int i;

	Stack__init__(s);
	printf("Inicializovany Stack: \n" );
	printf("Velkost stacku: %d \n ", s->prealocated );
	printf("Pozicia : %d \n ", s->top );
	for(i = 0 ;  i <= s->prealocated; i++){
		printf("Vypis staku : %d \n ", s->StackArray[i].Iint );
	}
}