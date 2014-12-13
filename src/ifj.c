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

#include "stack.h"
#include "defs.h"
#include "interpret.h"
#include "ial.h"
#include "lex_parser.h"
#include "syntax_analyzer.h"
#include "buff_file.h"

int main(int argc, char *argv[]) {
	LexParser lexParser;
	SyntaxAnalyzer synAnalyzer;
	Interpret interpret;
	if (argc != 2) {
		printf("Usage: %s filename\n", argv[0]);
	} else {
		BuffFile input;
		if (!BuffFile__init__(&input, argv[1])) {
			printf("ERROR: Couldn't open file %s ! \n", argv[1]);
		} else {
			LexParser__init__(&lexParser, input);
			SyntaxAnalyzer__init__(&synAnalyzer, &lexParser);
			SyntaxAnalyzer_parse(&synAnalyzer);
			BuffFile__dell__(&input);
			Interpret__init__(&interpret, synAnalyzer.instr);
			Interpret_run(&interpret);
		}
	}

	return EXIT_SUCCESS;
}
