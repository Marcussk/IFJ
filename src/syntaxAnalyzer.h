#ifndef SYNTAX_ANALYZER_h
#define SYNTAX_ANALYZER_h

#include "lexParser.h"
#include "hashTable.h"
#include "ifjTypes.h"
#include "stack.h"
#include "errorHandler.h"

typedef struct {
	LexParser * lp;
	Token lastToken;
} SyntaxAnalyzer;

extern HashTable * symbolTable;

void SyntaxAnalyzer__init__(SyntaxAnalyzer * self, LexParser * lp);
void SyntaxAnalyzer_parse(SyntaxAnalyzer * self);
void SyntaxAnalyzer__dell__(SyntaxAnalyzer * self);
void SyntaxAnalyzer_parseCond(SyntaxAnalyzer * se);
void SyntaxAnalyzer_parseExpr(SyntaxAnalyzer * se);

#endif
