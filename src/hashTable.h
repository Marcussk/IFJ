#ifndef HASH_TABLE_h
#define HASH_TABLE_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ifjTypes.h"
#include "ial.h"

/*
 * very similar with http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml
 * but there are not better implementations, maybe we should consider changing has function
 * but this seems to me good.
 */

void HashTable_debug(HashTable *hashtable);
HashTableItem * HashTable_lookupEverywhere(HashTable * self, char* str);

#endif
