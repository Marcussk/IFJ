#include "builtins.h"

// to make this use: gcc func.c errorHandler.c ifjTypes.h ifjTypes.c

int func_len(char *str) {

	return strlen(str);
}

void write(tIFJ type, iVal a1) {
	switch (type) {
	case iInt:
		printf("%d", a1.iInt);
		break;
	case iString:
		printf("%s", a1.iString);
		break;
	case iReal:
		printf("%f", a1.iReal);
		break;
	default:
		printf("Function write has performed error");
	}
}

/*
 * params: i- start of substring +1
 * 		   n - len of substring
 */
char * func_copy(char *f_str, int i, int n) {
	int strIndex;
	int offset = i - 1;
	if (i < 1)
		rt_error("Builtin function copy does not support i < 1");
	if (strlen(f_str) < (offset + n)) {
		rt_error(
				"Builtin function copy can't be performed because original string is too short\n");
	}
	char * newStr = malloc(i + 1 * sizeof(char));
	if (!newStr) {
		memoryError("func_copy can't alloc memory for newStr\n");
	}
	for (strIndex = 0; strIndex < n; strIndex++) {
		newStr[strIndex] = f_str[strIndex + offset];
	}
	newStr[strIndex] = '\0';
	return newStr;
}

int readLn(iVal *a1, tIFJ type) {
	char * str;
	switch (type) {
	case iInt:
		return scanf("%d", &(a1->iInt));
	case iReal:
		return scanf("%f", &(a1->iReal));
	case iString:
		str = calloc(sizeof(char), 256);
		a1->iString = str;
		// [TODO] 1. new string, 2. while ch != '\n'; String_append(ch); ch = getChar() // because strlen is unknown so array have to be dynamic
		return scanf("%s", a1->iString);
	case iChar:
		return scanf("%d", &(a1->iInt));
	case iBool:
		unimplementedError("readLn not implemented for bool");
		return 0;
	default:
		rt_readlnNumError();
		return 0;
	}
}

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

/////*** Neviem ako to funguje a co sa tam deje zvacsa to nefunguje a uz nem8m nervy to debugovat dneska **////////
/***pointa algoritmu je vytvorenie tabuliek posunov, jedna tabulka je bad_characters, druha good_suffixes,
 suffixes je len pomocna funkcia na vytvorenie good suffixes tiez sa tam vytvara nejaka tabulka. ***////
/*** Napisala som to podla niekolko algoritmou co som nala neviem mozno som sa pomylila mozno to nefunguje liky su zde:
 ******************************************************************
 http://www-igm.univ-mlv.fr/~lecroq/string/node14.html
 http://www.stoimen.com/blog/2012/04/17/computer-algorithms-boyer-moore-string-search-and-matching/
 http://www2.fiit.stuba.sk/~pospichal/soltis/algo_i513.htm
 https://www.youtube.com/watch?v=xYBM0_dChRE
 ******************************************************************
 Ak tomu niekto kodu niekto nechapete tak ani ja nie...
 Michal nehrot to s tymto toto sa da dorobit aj po pokusnom odovzdani.*/

/* tato funkcia vytvara prvy tabulku mala by nastavit posuny iba pre znaku ktore sa nachadzaju
 v zdrojovom retazci, ale prave to tvori nejake nezmysli domievam sa ze chyba moze byt prave tu
 ale akosi nechapem preco*/

// ked som pozerala ako to je napisane na nete autor tam caroval s f neviem co to tam je .... //
void suffixes(char* pattern, int suffix[], int max_pattern_len) {

	suffix[max_pattern_len - 1] = max_pattern_len;

	int pattern_len = max_pattern_len - 1;
	int i, f;

	for (i = max_pattern_len - 2; i >= 0; --i) {
		if (i > pattern_len
				&& suffix[i + max_pattern_len - 1 - f] < i - pattern_len)
			suffix[i] = suffix[i + max_pattern_len - 1 - f];
		else {
			if (i < pattern_len)
				pattern_len = i;

			f = i;

			while (pattern_len >= 0
					&& pattern[pattern_len]
							== pattern[pattern_len + max_pattern_len - 1 - f])
				pattern_len--;
			suffix[i] = f - pattern_len;
		}
	}

	return;
}

void good_suffixes(char *pattern, int good_suffix[]) {

	int pattern_len = strlen(pattern);
	int suffix[pattern_len]; //dalsie pole??
	int i, j;

	suffixes(pattern, suffix, pattern_len);

	for (i = 0; i < pattern_len; i++)
		good_suffix[i] = pattern_len;

	for (i = pattern_len - 1; i >= 0; i--) {
		if (suffix[i] == i + 1) {
			for (j = 0; j <= pattern_len - i - 1; j++) {
				if (good_suffix[j] == pattern_len)
					good_suffix[j] = pattern_len - i - 1;
			}
		}
	}

	for (i = 0; i < pattern_len - 2; i++) {
		good_suffix[pattern_len - 1 - suffix[i]] = pattern_len - i - 1;
	}

	return;
}

void bad_char(char *pattern, int bad_character[], int max_src_len) {
	int i;
	int pattern_len = strlen(pattern);

	for (i = 0; i < max_src_len; i++) {
		bad_character[i] = pattern_len;
	}
	for (i = 0; i < pattern_len - 1; i++) {
		bad_character[(int) pattern[i]] = pattern_len - i - 1; // plnenie pola indexami, ale deje sa tu nieco zle :D 
	}
	return;
}

// funkcia na zistenie velkosti abecedy vstupneho stringu 
int alfabeth_len(int src_len, char *src) {
	int i;
	int j;
	bool new_char = true; // ak true prvok sa pocita, inac uz bol zapocitany
	int alfa_len = 0; // pocet roznych prvkov

	for (i = 0; i <= src_len - 1; i++) {
		for (j = i; j >= 1; j--) {
			if (src[i] == src[j - 1])
				new_char = false;
		}
		if (new_char) {
			//printf("%c\n",src[i] ); vypis prvkov ako pomocka
			alfa_len++;
		}
		new_char = true;
	}

	return alfa_len;
}

int func_find(char *src, char *search) {

	int i = 0, j = 0, count = 0;
	int src_len = strlen(src);
	int search_len = strlen(search);
	int alfa_len = alfabeth_len(src_len, src);

	//arrays for GS and BC tables;
	// neviem ci robim vobec spravne velke polia
	int good_suffix[src_len]; // pole velkosti dlzky zdrojoveho retazca, neviem ci to nema byt dlzka abecedy zdrojoveho ratazca
	int bad_character[alfa_len]; // pole velkosti abecedy zdrojoveho retazca 

	good_suffixes(search, good_suffix);
	bad_char(search, bad_character, count);

	//tu sa zacina carovat//
	while (j < src_len - search_len) {
		for (i = search_len - 1; i >= 0 && search[i] == src[i + j]; i--)
			;
		if (i < 0) {
			return j;
			//j += good_suffix[0];
		} else if (good_suffix[i]
				>= bad_character[(int) src[i + j]] - search_len + 1 + i)
			j += good_suffix[i];
		else
			j += bad_character[(int) src[i + j]] - search_len + 1 + i;
	}
	return -1;
}


///////// KONIEC TOHOTO VELKEHO NEZMYSLU//////////////////////

void registrBuiltins(HashTable * ht) {
/*	iVar * item = NULL;
	iVar * param = malloc(sizeof(iVar));
	if (!param) {
		memoryError("registrBuiltins can't allocate memory for new param\n");
	}
	//pro vsechny jmena HashTable_insert(ht, jmeno, &item);
	HashTable_insert(ht, "readln", &item);
	item->type = iFn;
	item->isInitialied = true;
	item->val.fn = iFunction__init__();
	item->val.fn->retType = iVoid;
	param->type = iVoid;
	iFunction_addParam(item->val.fn, param);
*/
	//funkcia, navratova hodnota, typ parametru
	regFn(ht,"readln", iVoid, iVoid);  
	regFn(ht,"write", iVoid, iVoid);
	regFn(ht,"length", iInt, iVoid);
	regFn(ht,"copy", iString, iInt);
	regFn(ht,"find", iInt, iString);
	regFn(ht,"sort", iString, iVoid);
	

/*	HashTable_insert(ht, "write", &item);
	HashTable_insert(ht, "length", &item);
	HashTable_insert(ht, "copy", &item);
	HashTable_insert(ht, "sort", &item);
	HashTable_insert(ht, "find", &item);
*/
}

void regFn(HashTable * ht, char * name, tIFJ retTyp,int paramsCnt,...) {
	va_list valist;
	va_start(valist, paramsCnt);
	int i;
	iVar * item = NULL;
	HashTable_insert(ht, name, &item);
	item->type = iFn;
	item->isInitialied = true;
	item->val.fn = iFunction__init__();
	item->val.fn->retType = retTyp;
		
	for (i = 0; i < paramsCnt; i++) {
		iVar * param =  malloc(sizeof(iVar));
		if (!param) {
			memoryError("Can't allocate memory for new parameter\n");
		}
		param->type = va_arg(valist, tIFJ);
		iFunction_addParam(item->val.fn, param);
	}
	va_end(valist);
}
