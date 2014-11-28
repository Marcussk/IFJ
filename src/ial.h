#ifndef IAL_h
#define IAL_h

#include "stdbool.h"
#include <stdlib.h>
#include <string.h>
#include "ifjTypes.h"
#include "iFunction.h"

int find(char *txt, char *pat);
//int func_find( char *src, char *search );
char * func_sort(char *str);

typedef enum {
	ht_found, ht_inserted
} HTLookupRes;

typedef struct hashTableItem_ {
	char * str;
	iVar * var;
	struct hashTableItem_ * next;
} HashTableItem;

typedef struct sHashTable {
	int size;
	HashTableItem **table;
	struct sHashTable * masterTable;
	iVar * masterItem;
} HashTable;

HashTable * HashTable__init__(int size);
HashTableItem * HashTable_lookup(HashTable *hashtable, char *str);
HashTableItem * HashTable_lookupEverywhere(HashTable * self, char* str);
int HashTable_insert(HashTable *hashtable, char *str, iVar ** newItem);
void HashTable__dell__(HashTable *hashtable);

#endif
