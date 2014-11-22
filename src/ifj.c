/*
 ============================================================================
 Name        : ifj.c
 Author      : 
 Version     : 0.1
 Copyright   : Your copyright notice
 Description :
 Hint        : refactor-your-code-commrades-or-it-s-the-gulags-for-you
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "lexParser.h"
#include "syntaxAnalyzer.h"
#include "tokenMap.h"
#include "defs.h"
#include "interpret.h"
#include "instruction.h"
#include "stack.h"

int main(int argc, char *argv[]) {
	int a = 8;
	char * str = " from interpret\n";
	Stack instr;
	Interpret intr;
	Stack__init__(&instr);
	Stack_push(&instr, (Instruction ) { i_push, iInt, a, NULL, NULL});
	Stack_push(&instr, (Instruction ) { i_write, iInt, NULL, NULL, NULL});
	Stack_push(&instr, (Instruction ) { i_write, iString, NULL, NULL, NULL});
	Interpret__init__(&intr, instr);
	Interpret_run(&intr);
	printf("interpret end \n");
/*
	LexParser lexParser;
	SyntaxAnalyzer synAnalyzer;
	if (argc != 2) {
		printf("usage: %s filename\n", argv[0]);
	} else {
		FILE *file = fopen(argv[1], "r");
		if (file == 0) {
			printf("ERROR: Could not open file!\n");
		} else {
			LexParser__init__(&lexParser, file);
			SyntaxAnalyzer__init__(&synAnalyzer, &lexParser);
			SyntaxAnalyzer_parse(&synAnalyzer);
			fclose(file);
		}
	}
*/
	return EXIT_SUCCESS;
}
