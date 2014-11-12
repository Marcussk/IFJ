#include "hashTable.h"

HashTable *HashTable__init__(int size) {
	HashTable *new_table;
	int i;

	if (size < 1)
		return NULL;

	if ((new_table = malloc(sizeof(HashTable))) == NULL)
		memoryError("Cannot allocate hash table\n");

	if ((new_table->table = malloc(sizeof(hashTableItem *) * size)) == NULL)
		memoryError("Cannot allocate content of hash table\n");

	for (i = 0; i < size; i++)
		new_table->table[i] = NULL;

	new_table->size = size;

	return new_table;
}

//returns hash key
unsigned int HashTable_hash(HashTable *hashtable, char *str) {
	unsigned int hashval = 0;

	for (; *str != '\0'; str++)
		hashval = *str + (hashval << 5) - hashval;

	// fit into the necessary range
	return hashval % hashtable->size;
}

// = search in table
hashTableItem *HashTable_lookup(HashTable *hashtable, char *str) {
	hashTableItem *list;
	unsigned int hashval = HashTable_hash(hashtable, str);

	for (list = hashtable->table[hashval]; list != NULL; list = list->next)
		if (strcmp(str, list->var->name) == 0)
			return list;

	return NULL;
}

//insert string as iVar reference is gained thru newItem
int HashTable_insert(HashTable *hashtable, char *str, iVar ** newItem) {
	hashTableItem *new_list;
	hashTableItem *current_list;
	unsigned int hashval = HashTable_hash(hashtable, str);

	current_list = HashTable_lookup(hashtable, str);

	if (current_list){
		// already exists
		*newItem = current_list->var;
		return 2;
	}
	if((new_list = malloc(sizeof(hashTableItem))) == NULL)
		memoryError("Cannot allocate new item to hash table\n");
	new_list->var = iVar__init__(strdup(str));
	*newItem = new_list->var;

	new_list->next = hashtable->table[hashval];
	hashtable->table[hashval] = new_list;

	return 0;
}

void HashTable__dell__(HashTable *hashtable) {
	int i;
	hashTableItem *list, *temp;

	for (i = 0; i < hashtable->size; i++) {
		list = hashtable->table[i];
		while (list != NULL) {
			temp = list;
			list = list->next;
			iVar__dell__(temp->var);
			free(temp);
		}
	}

	free(hashtable->table);
	free(hashtable);
}

void HashTable_print(HashTable *self) {
	int hash;
	printf("<hashTable:%p>\n", (void *) self);
	hashTableItem *list, *temp;
	for (hash = 0; hash < self->size; hash++) {
		list = self->table[hash];
		while (list != NULL) {
			temp = list;
			list = list->next;
			printf("<item hash: %d, pointer: %p val: %s>\n", hash, temp,
					temp->var->name);
		}
	}
}
