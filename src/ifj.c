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
	/*int a = 8;
	 char * str = " from interpret\n";
	 Stack * s = Stack__init__();
	 Stack_push(s, (Instruction ) { WRITE, iInt, &a, NULL, NULL});
	 Stack_push(s, (Instruction ) { WRITE, iString, &str, NULL, NULL});
	 interpretRun(s);
	 printf("interpret end \n");*/

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

	return EXIT_SUCCESS;
}
