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
	//char *str;
	iVar * var;
	struct hashTableItem_ *next;
} hashTableItem;

typedef struct {
	int size;
	hashTableItem **table;
} HashTable;

HashTable * HashTable__init__(int size);
hashTableItem * HashTable_lookup(HashTable *hashtable, char *str);
int HashTable_insert(HashTable *hashtable, char *str, iVar ** newItem);
void HashTable__dell__(HashTable *hashtable);
void HashTable_print(HashTable *hashtable);

#endif
