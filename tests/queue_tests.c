#include "queue_tests.h"
/*tests for queueArr implementation */

void test_queue1(void) {
	InstrQueueArr q;
	InstrQueueArr__init__(&q);
	Instruction ins = { .code = i_push, .type = iInt, .a1 = NULL, .a2 = NULL,
			.dest = NULL };
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));

	InstrQueueArr_insert(&q, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_debud(&q,q.actual);
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_debud(&q, q.actual);
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_debud(&q, q.actual);
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_debud(&q, q.actual);

	InstrQueueArr__dell__(&q);
}
/*test for realloc array size*/
void test_queue2(void) {
	InstrQueueArr q;
	InstrQueueArr__init__(&q);
	Instruction ins = { .code = i_push, .type = iInt, .a1 = NULL, .a2 = NULL,
			.dest = NULL };
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));

	InstrQueueArr_insert(&q, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrend });

	InstrQueueArr_insert(&q, ins);

	InstrQueueArr_insert(&q, ins);

	InstrQueueArr_insert(&q, (Instruction ) { i_div, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });

	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_debud(&q, q.actual);
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_debud(&q, q.actual);

	InstrQueueArr__dell__(&q);

}
/*Test next*/
void test_queue3(void) {
	InstrQueueArr q;
	InstrQueueArr__init__(&q);
	Instruction ins = { .code = i_push, .type = iInt, .a1 = NULL, .a2 = NULL,
			.dest = NULL };
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));

	InstrQueueArr_insert(&q, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_insert(&q, (Instruction ) { i_div, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_debud(&q, q.actual);

	q.actual = 5;
	Instruction * i;
	i = InstrQueueArr_next(&q);

	InstrQueueArr_debud(&q, q.actual);
	InstrQueueArr__dell__(&q);
}
/*test on index*/
void test_queue4(void) {
	InstrQueueArr q;
	InstrQueueArr__init__(&q);
	Instruction ins = { .code = i_push, .type = iInt, .a1 = NULL, .a2 = NULL,
			.dest = NULL };
	InstrParam * StackAddrend = malloc(sizeof(InstrParam));

	InstrQueueArr_insert(&q, (Instruction ) { i_jmp, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_insert(&q, ins);
	InstrQueueArr_insert(&q, (Instruction ) { i_div, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_insert(&q, (Instruction ) { i_mul, iVoid, NULL, NULL,
					StackAddrend });
	InstrQueueArr_debud(&q, 5);

	InstrQueueArr__dell__(&q);
}
