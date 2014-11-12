#ifndef SYNTAX_ANALYZER_h
#define SYNTAX_ANALYZER_h

#include "lexParser.h"
#include "hashTable.h"
#include "ifjTypes.h"
#include "stack.h"

typedef struct {
	LexParser * lp;
} SyntaxAnalyzer;

extern HashTable * symbolTable;

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp);
void SyntaxAnalyzer_parse(SyntaxAnalyzer * self);
void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self);

#endif
