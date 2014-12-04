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

#include "array_as_stack.h"
#include "defs.h"
#include "interpret.h"
#include "ial.h"
#include "lex_parser.h"
#include "syntax_analyzer.h"

int main(int argc, char *argv[]) {

	LexParser lexParser;
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
			//Interpret__dell__(&interpret);
			//InstrQueue__dell__(&(synAnalyzer.instr));
			//SyntaxAnalyzer__dell__(&synAnalyzer);
		}
	}

	return EXIT_SUCCESS;
}
