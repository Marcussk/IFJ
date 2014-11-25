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
#include "defs.h"
#include "interpret.h"

int main(int argc, char *argv[]) {
	Interpret_test3();
	/*LexParser lexParser;
	SyntaxAnalyzer synAnalyzer;
	Interpret interpret;
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
			Interpret__init__(&interpret, synAnalyzer.instr);
			Interpret_run(&interpret);
			Interpret__dell__(&interpret);
		}
	}
*/
	return EXIT_SUCCESS;
}
