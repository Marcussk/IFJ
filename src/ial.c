#include "ial.h"

/* changing first and last elemnts*/
void change(char *str, int right) {
	char h = str[0];
	str[0] = str[right];
	str[right] = h;
	return;
}

/* *str - zoradovany retazec */
void Sift(char *str, int left, int right) {
	int i = left;
	int j = (2 * i) + 1; /* lavy syn */
	char tmp = str[i];
	bool cond = j <= right; /* podmienka vymeny prvkov */

	while (cond) {
		if (j < right) /* strom ma oboch synov */
			if (str[j] < str[j + 1])
				j++;
		if (tmp >= str[j])
			cond = false;
		else {
			str[i] = str[j]; /* prepisanie prvku na zlom mieste */
			i = j; /* pokracovanie na nizsi uzol stromu */
			j = 2 * i + 1; /* ustanovenie praveho syna */
			cond = j <= right; /* ak j > najpravejsi prvok right, ziadny dalsi syn neexistuje => koneic pola */
		}
	}
	str[i] = tmp;
	return;
}

/* strom implementovany polom kde lavy syn je na pozici 2i+1, a pravy syn na pozicii 2i+2 */
char * func_sort(char *str) {
	int str_len = strlen(str);

	int i;
	int left = str_len / 2 - 1; /* index najpravejsieho uzlu na najnizsej urovni */
	int right = str_len - 1; /* posleny prvok pola */

	for (i = left; i >= 0; i--) {
		Sift(str, i, right);
	}
	for (right = str_len - 1; right >= 1; right--) {
		change(str, right); /* vymena prvkov */
		Sift(str, 0, right - 1); /* opakovane zostavenie stromu z prvkov ktore este niesu zoradene */
	}

	return str;
}

void badcharfill(char *str, int size, int badchar[MAX_SIZE])
{
	int i;
	//initialize array
	for(i = 0; i < MAX_SIZE; i++)
	{
		badchar[i] = -1;
	}
	//fill array of patlength with last occurence of character
	//array is rewritten during iterations and array[currentchar] = positioninpatter
	for (i = 0; i < size; i++)
	{
		badchar[(int) str[i]] = i;
	}
}

int max(int a, int b)
{
	return (a > b)? a: b;
}

int func_find(char *txt, char *pat)
{
	int patlength = strlen(pat);
	int txtlength = strlen(txt);
	int shift = 0;
	
	int badchar[MAX_SIZE];
	badcharfill(pat,patlength, badchar);

	while(shift <= txtlength - patlength)
	{
		int matchindex = patlength - 1;
		//Check chars from right to left to see if we have pattern match
		while((matchindex >= 0) && (pat[matchindex] == txt[shift + matchindex]))
		{
			matchindex--;
		}
		//Succesfully found match (patlength of chars are matching)
		if(matchindex < 0)
		{
			//printf("Finding: %d \n", shift + 1);
			return shift + 1;
			//if we can move shift so it aligns with text
			if(shift + patlength < txtlength)
			{
				shift += patlength - badchar[txt[shift + patlength]];
			}
			//if not move just by one
			else {
				shift += 1;
			}
		}
		//havent found match in text
		else
		{
			//move shift, if there is bigger skip choose it
			shift += max(1 , matchindex - badchar[txt[shift+matchindex]]);
		}
	}
	return 0;
}

HashTable * HashTable__init__(int size) {
	HashTable * new_table;
	int i;

	if (size < 1)
		return NULL;

	if ((new_table = malloc(sizeof(HashTable))) == NULL)
		memoryError("Can't allocate hash table\n");

	if ((new_table->table = malloc(sizeof(HashTableItem *) * size)) == NULL)
		memoryError("Can't allocate content of hash table\n");

	for (i = 0; i < size; i++)
		new_table->table[i] = NULL;

	new_table->size = size;

	new_table->masterTable = NULL;
	new_table->masterItem = NULL;
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

HashTableItem * HashTable_lookupEverywhere(HashTable * self, char* str) {
	HashTableItem * found = HashTable_lookup(self, str);
	if (!found && self->masterTable)
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
		if (newItem)
			*newItem = current_list->var;
		return ht_found;
	}
	if ((new_list = malloc(sizeof(HashTableItem))) == NULL)
		memoryError("Can't allocate new item in hash table\n");
	new_list->var = iVar__init__();
	new_list->str = strdup(str);
	if (newItem)
		*newItem = new_list->var;

	new_list->next = self->table[hashval];
	self->table[hashval] = new_list;

	return ht_inserted;
}

void HashTableItem__dell__(HashTableItem *self) {
	if (self->var->type == iFn)
		iFunction__dell__(self->var->val.fn);
	iVar__dell__(self->var);
	free(self->str);
}

void HashTable__dell__(HashTable *self) {
	int i;
	HashTableItem *list, *temp;

	for (i = 0; i < self->size; i++) {
		list = self->table[i];
		while (list != NULL) {
			temp = list;
			list = list->next;
			HashTableItem__dell__(temp);
			free(temp);
		}
	}

	free(self->table);
	free(self);
}
