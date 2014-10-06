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
#include "tokenMap.h"
#include "defs.h"

int main(int argc, char *argv[]) {
	LexParser lp = LexParser__init__(NULL);
	//TokenMap_vizualize(lp.tParser.map, 0);
	if (argc != 2) {
		printf("usage: %s filename\n", argv[0]);
	} else {
		FILE *file = fopen(argv[1], "r");
		if (file == 0) {
			printf("Could not open file\n");
		} else {
			char x;
			while ((x = fgetc(file)) != EOF) {
				LexParser_push(&lp, x);
			}
			fclose(file);
		}
	}

	return EXIT_SUCCESS;
}
