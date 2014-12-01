
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
/*
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
	int * suffix = calloc(sizeof(int), pattern_len); //dalsie pole??
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
	int * good_suffix = calloc(sizeof(int), src_len); // pole velkosti dlzky zdrojoveho retazca, neviem ci to nema byt dlzka abecedy zdrojoveho ratazca
	int * bad_character = calloc(sizeof(int), alfa_len); // pole velkosti abecedy zdrojoveho retazca

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
*/