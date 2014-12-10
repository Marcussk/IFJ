#ifndef IAL_h
#define IAL_h

#include "stdbool.h"
#include <stdlib.h>
#include <string.h>

#include "i_function.h"
#include "ifj_types.h"

//cardinality of alphabet - number of all poss. chars
#define MAX_SIZE 255
#define SUFFSIZE 255

int func_find(char *txt, char *pat);
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
int HashTable_insert(HashTable *self, char *str, iVar * item2insert);
int HashTable_insertNew(HashTable *hashtable, char *str, iVar ** newItem);
void HashTable__dell__(HashTable *hashtable);

#endif
