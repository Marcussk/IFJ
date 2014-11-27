#include "hashTable.h"

void HashTable_print(HashTable *self) {
	int hash;
	printf("<hashTable:%p>\n", (void *) self);
	HashTableItem *list, *temp;
	for (hash = 0; hash < self->size; hash++) {
		list = self->table[hash];
		while (list != NULL) {
			temp = list;
			list = list->next;
			printf("<item hash: %d, pointer: %p val: %s>\n", hash, temp,
					temp->str);
		}
	}
}
