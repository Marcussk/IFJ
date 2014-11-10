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


int main(int argc, char *argv[]) {
    LexParser lexParser;
    SyntaxAnalyzer synAnalyzer;

	if (argc != 2) {
		printf("usage: %s filename\n", argv[0]);
	} else {
		FILE *file = fopen(argv[1], "r");
		if (file == 0) {
			printf("Could not open file\n");
		} else {
			LexParser__init__(&lexParser,file);
			SyntaxAnalyzer__init__(&synAnalyzer, &lexParser);
			SyntaxAnalyzer_parse(&synAnalyzer);
			fclose(file);
		}
	}
	//TokenMap_vizualize(lp.tParser.map, 0);
	//HashTable_print(ht);

	//LexParser__dell__(&lexParser);
	return EXIT_SUCCESS;
}
