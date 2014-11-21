#include "hashTable.h"

HashTable * HashTable__init__(int size) {
	HashTable * new_table;
	int i;

	if (size < 1)
		return NULL;

	if ((new_table = malloc(sizeof(HashTable))) == NULL)
		memoryError("Cannot allocate hash table\n");

	if ((new_table->table = malloc(sizeof(HashTableItem *) * size)) == NULL)
		memoryError("Cannot allocate content of hash table\n");

	for (i = 0; i < size; i++)
		new_table->table[i] = NULL;

	new_table->size = size;

	return new_table;
}

//returns hash key
unsigned int HashTable_hash(HashTable *self, char *str) {
	unsigned int hashval = 0;

	for (; *str != '\0'; str++)
		hashval = *str + (hashval << 5) - hashval;

	// fit into the necessary range
	return hashval % self->size;
}

HashTableItem * HashTable_lookupEverywhere(HashTable * self, char* str){
	HashTableItem * found = HashTable_lookup(self, str);
	if(!found && self->masterTable)
		return HashTable_lookup(self->masterTable, str);
	return found;
}

// = search in table
HashTableItem *HashTable_lookup(HashTable *self, char *str) {
	HashTableItem *list;
	unsigned int hashval = HashTable_hash(self, str);

	for (list = self->table[hashval]; list != NULL; list = list->next)
		if (strcmp(str, list->str) == 0)
			return list;

	return NULL;
}

//insert string as iVar reference is gained thru newItem
int HashTable_insert(HashTable *self, char *str, iVar ** newItem) {
	HashTableItem *new_list;
	HashTableItem *current_list;
	unsigned int hashval = HashTable_hash(self, str);

	current_list = HashTable_lookup(self, str);

	if (current_list) {
		// already exists
		if(newItem)
		*newItem = current_list->var;
		return 2;
	}
	if ((new_list = malloc(sizeof(HashTableItem))) == NULL)
		memoryError("Cannot allocate new item to hash table\n");
	new_list->var = iVar__init__();
	new_list->str = strdup(str);
	if(newItem)
	*newItem = new_list->var;

	new_list->next = self->table[hashval];
	self->table[hashval] = new_list;

	return 0;
}

void HashTable__dell__(HashTable *self) {
	int i;
	HashTableItem *list, *temp;

	for (i = 0; i < self->size; i++) {
		list = self->table[i];
		while (list != NULL) {
			temp = list;
			list = list->next;
			iVar__dell__(temp->var);
			free(temp->str);
			free(temp);
		}
	}

	free(self->table);
	free(self);

}

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
