#ifndef HASH_TABLE_h
#define HASH_TABLE_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ifjTypes.h"

/*
 * very similar with http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
 * but there are not better implementations, maybe we should consider changing has function
 * but this seems to me good.
 */

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
int HashTable_insert(HashTable *hashtable, char *str, iVar ** newItem);
void HashTable__dell__(HashTable *hashtable);
void HashTable_debug(HashTable *hashtable);

#endif
