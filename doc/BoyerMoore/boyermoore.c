#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
#define MAX_SIZE 256

void test1();
void test2();
void test3();
void test4();
int find(char *txt, char *pat);
int max(int a, int b);
void badcharfill(char *str, int size, int badchar[MAX_SIZE]);

int main() 
{
	test1();
	test2();
	test3();
	test4();
    return 0;

}

int find(char *txt, char *pat)
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

void badcharfill(char *str, int size, int badchar[MAX_SIZE])
{
	//initialize array
	for(int i = 0; i < MAX_SIZE; i++)
	{
		badchar[i] = -1;
	}
	//fill array of patlength with last occurence of character
	//array is rewritten during iterations and array[currentchar] = positioninpatter
	for (int i = 0; i < size; i++)
	{
		badchar[(int) str[i]] = i;
	}
}


int max(int a, int b)
{
	return (a > b)? a: b;
}

void test1()
{	
	char txt[] = "ABAAABCD";
    char pat[] = "ABC";
    int result = find(txt, pat);
    if (result == 5)
    {
    	printf("test1 passed\n");
    }
    else
    {
    	printf("test1 failed Found at: %d \n",result);
    }
    return;
}

void test2()
{	
	char txt[] = "BCDEFGABC";
    char pat[] = "ABC";
     int result = find(txt, pat);
    if (result == 7)
    {
    	printf("test2 passed\n");
    }
    else
    {
    	printf("test2 failed Found at: %d \n",result);
    }
    return;
}

void test3()
{	
	char txt[] = "ABCXXXXXABC";
    char pat[] = "ABC";
     int result = find(txt, pat);
    if (result == 1)
    {
    	printf("test3 passed\n");
    }
    else
    {
    	printf("test3 failed Found at: %d \n",result);
    }
    return;
}

void test4()
{	
	char txt[] = "DEFGHIJKL";
    char pat[] = "ABC";
     int result = find(txt, pat);
    if (result == 0)
    {
    	printf("test4 passed\n");
    }
    else
    {
    	printf("test4 failed Found at: %d \n",result);
    }
    return;
}
*/
