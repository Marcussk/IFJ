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
#include "hashTable.h"

hashTable * ht;


void printToken(LexParser * p, Token t) {
	char * str;
	switch (t) {
	case t_string:
		printf("Line %d:<str:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_id:
		if( HashTable_insert(ht, strdup(p->str.buff)) ==2){
			printf("Line %d:<id:'%s', already in table >\n", p->lineNum, p->str.buff);
		}
		printf("Line %d:<id:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_num_int:
		printf("Line %d:<int:'%s'>\n", p->lineNum, p->str.buff);
		break;
	case t_num_real:
		printf("Line %d:<real:'%s'>\n", p->lineNum, p->str.buff);
		break;
	default:
		str = getTokenStr(t);
		if (str) {
			printf("Line %d: [%s]\n", p->lineNum, str);
		} else {
			printf("Line %d: <tokenWithoutStr: %d>\n", p->lineNum, t);
		}
	}
}

int main(int argc, char *argv[]) {
	ht = HashTable__init__(128);
	LexParser lp = LexParser__init__(&printToken);
	//TokenMap_vizualize(lp.tParser.map, 0);
	if (argc != 2) {
		printf("usage: %s filename\n", argv[0]);
	} else {
		FILE *file = fopen(argv[1], "r");
		if (file == 0) {
			printf("Could not open file\n");
		} else {
			char ch;
			while ((ch = fgetc(file)) != EOF) {
				LexParser_push(&lp, ch);
			}
			LexParser_flush(&lp); //
			fclose(file);
		}
	}
	HashTable_print(ht);
	return EXIT_SUCCESS;
}
