#include "ial.h"

/* changing first and last elemnts*/
void change(char *str, int right) {
	char h = str[0];
	str[0] = str[right];
	str[right] = h;
	return;
}

/* *str - zoradovany retazec
 * treseme stromem tak, aby se maximum dostalo az do korenoveho uzlu stromu
 *  */
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

	for (i = left; i >= 0; i--) {		/*ustaveni hromady*/
		Sift(str, i, right);
	}
	for (right = str_len - 1; right >= 1; right--) {
		change(str, right); /* vymena prvkov */
		Sift(str, 0, right - 1); /* opakovane zostavenie stromu z prvkov ktore este niesu zoradene */
	}

	return str;
}

/* 	
Bad-character heurestic 	
Initialize array so we can skip to the next occurence 	
or skip the length of pattern 	
Array is filled with distance of character from the end of pat 	
*/
void badcharfill(char *str, int length, int badchar[MAX_SIZE]) {
	int i;
	for (i = 0; i < MAX_SIZE; i++) {
		badchar[i] = length;  
	}
	for (i = 0; i < length; i++) {
		badchar[(int) str[i]] = length -i -1;
	}
}

/* 	
Secondary function to Good-suffix heurestics
Case when mismatch occurs 	
*/
void suffixes(char *str, int length, int *suff) {
	int f, g, i;
	suff[length -1] = length;
	g = length - 1;
	for (i = length - 2;i >= 0;--i) {
		if (i > g && suff[i + length - 1 - f] < i - g)
			suff[i] = suff[i + length - 1 -f];
		else {
			if (i < g)
				g = i;
			f = i;
			while (g >= 0 && str[g] == str[g + length - 1 - f])
				--g;
			suff[i] = f - g;
		}
	}
	return;
}

/* 	
Good-suffix function 	
Fill two arrays with borders of substring and with all matching substrings 	
Use that to skip to the next occurence of substring in text
Case when char belongs to pat 	
*/
void processsuff(char *str, int length, int goodsuff[]) {
	int i, j, suff[SUFFSIZE];
	suffixes(str, length, suff);
	for (i = 0; i<length; ++i)
		goodsuff[i]=length;
	j = 0;
	for (i = length -1;i >= 0;--i)
		if (suff[i] == i +1)
			for (; j < length -1 -i;++j)
				if (goodsuff[j] == length)
					goodsuff[j] = length -1 - i;
	for (i = 0; i < length - 2; ++i)
	{
		goodsuff[length -1 - suff[i]] = length -1 -i;
	}
	return;
}

int func_find(char *T, char *P) {
	if (!strlen(P))
		return 1;
	ComputeJumps(P);
	ComputeMatch(P);

	int i = BMA(P, T) + 1;
	return i;
}

void ComputeJumps(char * P)
{
    int ch;
    int k;

    for(ch = 0; ch < MAX_LENGTH; ch++)
    {
        CharJump[ch] = strlen(P);
    }
    for(k=0; k < strlen(P)-1; k++)
    {
        CharJump[P[k]] = strlen(P) -1 - k;
    }
}
void ComputeMatch(char *P)
{
    int k, q, qq;
    int Backup[MAX_LENGTH];
    int m = (strlen(P))-1;
    for(k = 0; k <= m; k++)
    {
        MatchJump[k] = 2*m - k;
    }
    k = m;
    q = m +1;

    while(k>=0)
    {
        Backup[k] = q;
        while((q<=m) && (P[k] != P[q]))
        {
            MatchJump[q] = min(MatchJump[q],(m-k));
            q = Backup[q];
        }
        k--;
        q--;
    }

    for(k = 0; k <= q; k++)
    {
        MatchJump[k] =  min(MatchJump[k],(m +q-k+1));
    }

    qq = Backup[q];
    while(q<m)
    {
        while(q<=qq)
        {
            MatchJump[q] = min(MatchJump[q],(qq-q+m+1));
            q++;
        }
        qq =Backup[qq];
    }
}

int BMA(char * P, char * T)
{

    int k = strlen(P)-1;
    int j = strlen(P)-1;

    while((j <= strlen(T)) && (k>=0))
    {
        if(T[j] == P[k])
        {
            j--;
            k--;
        }
        else
        {
           j = j + max(MatchJump[k], CharJump[T[j]]);
           k = strlen(P)-1;
        }
    }

    if(k < 0)
    {
        return (j+1);
    }
    else
        return -1;
}

HashTable * HashTable__init__(int size) {
	HashTable * new_table;
	int i;

	if (size < 1)
		return NULL;

	if ((new_table = malloc(sizeof(HashTable))) == NULL)
		Error_memory("Can't allocate hash table\n");

	if ((new_table->table = malloc(sizeof(HashTableItem *) * size)) == NULL)
		Error_memory("Can't allocate content of hash table\n");

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

// = search in table
HashTableItem *HashTable_lookup(HashTable *self, char *str) {
	HashTableItem *list;
	unsigned int hashval = HashTable_hash(self, str);

	for (list = self->table[hashval]; list != NULL; list = list->next)
		if (strcmp(str, list->str) == 0)
			return list;

	return NULL;
}

int HashTable_insert(HashTable *self, char *str, iVar * item2insert){
	HashTableItem *new_list;
	HashTableItem *current_list;
	unsigned int hashval = HashTable_hash(self, str);

	current_list = HashTable_lookup(self, str);

	if (current_list) {
		return ht_found;
	}
	if ((new_list = malloc(sizeof(HashTableItem))) == NULL)
		Error_memory("Can't allocate new item in hash table\n");
	new_list->var =item2insert;
	new_list->str = strdup(str);

	new_list->next = self->table[hashval];
	self->table[hashval] = new_list;

	return ht_inserted;

}

//insert string as iVar reference is gained thru newItem
int HashTable_insertNew(HashTable *self, char *str, iVar ** newItem) {
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
		Error_memory("Can't allocate new item in hash table\n");
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
