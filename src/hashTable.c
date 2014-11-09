#include "hashTable.h"

hashTable *HashTable__init__(int size) {
	hashTable *new_table;
	int i;

	if (size < 1)
		return NULL;

	if ((new_table = malloc(sizeof(hashTable))) == NULL)
		return NULL;

	if ((new_table->table = malloc(sizeof(hashTableItem *) * size)) == NULL)
		return NULL;

	for (i = 0; i < size; i++)
		new_table->table[i] = NULL;

	new_table->size = size;

	return new_table;
}

unsigned int HashTable_hash(hashTable *hashtable, char *str) {
	unsigned int hashval = 0;

	for (; *str != '\0'; str++)
		hashval = *str + (hashval << 5) - hashval;

	// fit into the necessary range
	return hashval % hashtable->size;
}

hashTableItem *HashTable_lookup(hashTable *hashtable, char *str) {
	hashTableItem *list;
	unsigned int hashval = HashTable_hash(hashtable, str);

	for (list = hashtable->table[hashval]; list != NULL; list = list->next)
		if (strcmp(str, list->str) == 0)
			return list;

	return NULL;
}

int HashTable_insert(hashTable *hashtable, char *str) {
	hashTableItem *new_list;
	hashTableItem *current_list;
	unsigned int hashval = HashTable_hash(hashtable, str);

	if ((new_list = malloc(sizeof(hashTableItem))) == NULL)
		return 1;

	current_list = HashTable_lookup(hashtable, str);
	if (current_list != NULL)
		return 2; // already exists
	new_list->str = strdup(str);
	new_list->next = hashtable->table[hashval];
	hashtable->table[hashval] = new_list;

	return 0;
}

void HashTable__dell__(hashTable *hashtable) {
	int i;
	hashTableItem *list, *temp;

	if (hashtable == NULL)
		return;

	for (i = 0; i < hashtable->size; i++) {
		list = hashtable->table[i];
		while (list != NULL) {
			temp = list;
			list = list->next;
			free(temp->str);
			free(temp);
		}
	}

	free(hashtable->table);
	free(hashtable);
}

void HashTable_print(hashTable *self) {
	int hash;
	printf("<hashTable:%p>\n", (void *) self);
	hashTableItem *list, *temp;
	for (hash = 0; hash < self->size; hash++) {
		list = self->table[hash];
		while (list != NULL) {
			temp = list;
			list = list->next;
			printf("<item hash: %d, pointer: %p val: %s>\n",hash, temp, temp->str);
		}
	}
}
