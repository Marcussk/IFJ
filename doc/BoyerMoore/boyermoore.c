#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 255
#define SUFFSIZE 255

#define DEGUG

#ifdef DEBUG
#define DEBUGING(body) body
#else
#define DEBUGING(body) ;
#endif



void test(int n, char *txt, char *pat, int expect);
void test1();
void test2();
void test3();
void test4();
void test5();
int find(char *txt, char *pat);
int max(int a, int b);
void badcharfill( char *str, int length, int badchar[MAX_SIZE] );

int main() 
{
    test(1,"ABAAABCD","ABC",5);
    test(2,"BCDEFGABCDEF","ABCD",7);
    test(3,"ABDXXXXXABCD","CD",11);
    test(4,"ABDXXXXXABCD","DE",0);
    test(5,"ABAAABCD","ABCD",5);
    test(6,"ADFSFBCD","",1);
    test(7,"","ABCD",0);
    return 0;

}

void badcharfill( char *str, int length, int badchar[] ) {
    int i;
    //initialize array
    for (i = 0; i < MAX_SIZE; i++) {
        badchar[i] = length; 
    }
    //fill array of patlength with last occurence of character
    //array is rewritten during iterations and array[currentchar] = positioninpatter
    for (i = 0; i < length; i++) {
        badchar[(int) str[i]] = length -i -1;
    }
}

void suffixes(char *str, int length, int *suff) {
   int f, g, i;
 
   suff[length - 1] = length;
   g = length - 1;
   for (i = length - 2; i >= 0; --i) {
      	if (i > g && suff[i + length - 1 - f] < i - g)
        	suff[i] = suff[i + length - 1 - f];
      	else {
        	if (i < g)
            	g = i;
         	f = i;
         	while (g >= 0 && str[g] == str[g + length - 1 - f])
            	--g;
         	suff[i] = f - g;
      	}
   	}
}

void processsuff(char *str, int length, int goodsuff[]) {
    int i, j, suff[SUFFSIZE];
   	suffixes(str, length, suff);
    for (i = 0; i < length; ++i)
    	goodsuff[i] = length;
    j = 0;
    for (i = length - 1; i >= 0; --i)
      	if (suff[i] == i + 1)
         	for (; j < length - 1 - i; ++j)
            	if (goodsuff[j] == length)
               		goodsuff[j] = length - 1 - i;
   	for (i = 0; i <= length - 2; ++i)
      	goodsuff[length - 1 - suff[i]] = length - 1 - i;
}

int find(char *txt, char *pat) {
    int patlength = strlen(pat);
    int txtlength = strlen(txt);
    int shift = 0;  
    int matchindex;
    int badchar[MAX_SIZE];
    int goodsuff[SUFFSIZE];

    if (strcmp(pat, "") == 0)
    {
        return 1;
    }

    badcharfill(pat, patlength, badchar);
    processsuff(pat, patlength, goodsuff);

    while (shift <= txtlength - patlength) { 
        matchindex = patlength - 1;
        //Check chars from right to left to see if we have pattern match
        for (matchindex = patlength - 1; matchindex >= 0 && pat[matchindex] == txt[matchindex +shift]; --matchindex);
        DEBUGING(printf("Match index: %d\n",matchindex);)
        //Succesfully found match (patlength of chars are matching)
            if (matchindex < 0) {
                //printf("Finding: %d \n", shift + 1);
                return shift + 1;
                //if we can move shift so it aligns with text
                shift += goodsuff[0];
            }
            else {
                shift += max(goodsuff[matchindex], badchar[(int)txt[matchindex + shift]] - patlength +1 + matchindex);
            }
    }       
    return 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void test(int n, char *txt, char *pat, int expect)
{
    int result;
    printf("------------------------------------------------------------------------\n");
    printf("test%d\n",n);
    printf("%s\n", txt);
    printf("%s\n", pat);
    result = find(txt, pat);
    if(expect == result)
    {
        printf("test%d passed\n",n);
    }
    else
    {
        
        printf("test%d failed Expected: %d Found at: %d \n", n, expect, result);
    }
    printf("------------------------------------------------------------------------\n");
    return;    
}

