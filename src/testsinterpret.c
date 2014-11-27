#include "testsinterpret.h"


void Interpret_test1() {

	InstrParam a, b;
	a.iString = "haba";
	b.iString = "baba";
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_less, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });

		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_nequal, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });

		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_more, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });

		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_equal, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });



		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_loreq, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });

		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &b, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_moreq, iString,  &a,  &b, NULL });
		InstrQueue_insert(&instr, (Instruction ) { i_write, iInt,  NULL,  NULL, NULL });




	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	//printf("\n %d  \n", (iStack_pop(&(intr.stack))).iInt);

}
void Interpret_test2() {

	InstrParam a, b, c;
	a.iString = "haba";
	b.iInt = 1;
	c.iInt = 3;
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &c, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iString, &a, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_copy, iString, &a, &b, NULL });

	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("%s interpret test end \n", (iStack_pop(&(intr.stack))).iString);

}

void Interpret_test3() {

	InstrParam a, b, c, d1, d2, f;
	a.iInt = 20;
	b.iInt = 15;
	c.iInt = 30;
	d1.iInt = 10;
	d2.iInt = 14;
	f.iInt = 10;
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

	//InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &c, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_less, iInt, &a, &b, NULL });

	InstrQueue_insert(&instr, (Instruction ) { i_jmpz, iInt, NULL, NULL, &d1  });

	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &c, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_add, iInt, &a, &b, &c });

	InstrQueue_insert(&instr, (Instruction ) { i_jmp, iInt, NULL, NULL, &d2  });

	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &c, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });//10
	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_sub, iInt, &a, &b, &c });

	InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &f, NULL, NULL });
	InstrQueue_insert(&instr, (Instruction ) { i_write, iInt, NULL, NULL, NULL }); //14



	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("%d interpret test end \n", iStack_pop(&(intr.stack)).iInt);

}
void Interpret_test4() {

	InstrParam  a,b;
	a.iInt = 20;
	b.iInt = 30;
	//b.iReal = 0;
	InstrQueue instr;
	Interpret intr;
	InstrQueue__init__(&instr);

			InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &a, NULL, NULL });
			InstrQueue_insert(&instr, (Instruction ) { i_push, iInt, &b, NULL, NULL });
			InstrQueue_insert(&instr, (Instruction ) { i_less, iInt,  NULL,  NULL, NULL });
			printf("\n %d  \n", (iStack_pop(&(intr.stack))).iInt);

	Interpret__init__(&intr, instr);
	Interpret_run(&intr);

}
