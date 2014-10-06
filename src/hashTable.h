#ifndef HASH_TABLE_h
#define HASH_TABLE_h

#include <stdlib.h>
#include <string.h>

/*
 * very similar with http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
 * but there are not better implementations, maybe we should consider changing has function
 * but this seems to me good.
 */

typedef struct hashTableItem_ {
	char *str;
	struct hashTableItem_ *next;
} hashTableItem;

typedef struct {
	int size;
	hashTableItem **table;
} hashTable;

hashTable * HashTable__init__(int size);
hashTableItem * HashTable_lookup(hashTable *hashtable, char *str);
int HashTable_insert(hashTable *hashtable, char *str);
void HashTable__dell__(hashTable *hashtable);

#endif
