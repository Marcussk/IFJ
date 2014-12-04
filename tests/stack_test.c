#include "stack_test.h"

#define RANDOM_NUM 15
#define REAL_NUM 17
#define IND 6

void test_stack1(void) {
	printf("Test1 stared ..................\n");
	StackArr s;
	int i;
	StackArr__init__(&s);
	printf("Inicializovany Stack: \n");
	printf("Velkost stacku: %d \n ", s.prealocated);
	printf("TOP: %d \n ", s.top);
	printf("Vypis poloziek staku:\n ");
	iVal new_iVal, next_iVal, iVal_1;
	new_iVal.iInt = RANDOM_NUM;
	next_iVal.iInt = REAL_NUM;
	iVal_1.iInt = 42;
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, next_iVal);
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, next_iVal);
	StackArr_push(&s, new_iVal);
	StackArr_push(&s, next_iVal);
	StackArr_push(&s, next_iVal);

	printf("Velkost stacku: %d \n ", s.prealocated);
	printf("TOP: %d \n ", s.top);
	printf("Vypis poloziek staku:\n ");
	int_StackArr_debug(&s);
	printf("__________________________________\n ");

	iVal ret_iVal = StackArr_pop(&s);
	printf("pop : %d", ret_iVal.iInt);
	printf("TOP: %d \n ", s.top);
	int_StackArr_debug(&s);

	StackArr_push(&s, new_iVal);
	printf("TOP: %d \n ", s.top);
	printf("Vypis poloziek staku:\n ");
	int_StackArr_debug(&s);

	StackArr__dell__(&s);
	printf("Velkost stacku: %d \n ", s.prealocated);
	printf("TOP: %d \n ", s.top);
	printf("Vypis poloziek staku:\n ");
	int_StackArr_debug(&s);

	StackArr__init__(&s);
	StackArr_push(&s, iVal_1);
	StackArr_push(&s, iVal_1);
	StackArr_push(&s, iVal_1);
	StackArr_push(&s, iVal_1);

	printf("TOP: %d \n ", s.top);
	printf("Vypis poloziek staku:\n ");
	int_StackArr_debug(&s);

	iVal ret_iVal_1;
	ret_iVal_1 = StackArr_getAt(&s, IND);
	printf("Hodnota na indexe %d je %d \n", IND, ret_iVal_1.iInt);
	StackArr__dell__(&s);
}
