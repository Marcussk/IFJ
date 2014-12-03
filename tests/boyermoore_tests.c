#include "boyermoore_tests.h"

void test(int n, char *txt, char *pat, int expect)
{
    int result;
    printf("------------------------------------------------------------------------\n");
    printf("test%d\n",n);
    printf("%s\n", txt);
    printf("%s\n", pat);
    result = func_find(txt, pat);
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

void BM_runTests()
{
    test(1,"ABAAABCD","ABC",5);
    test(2,"BCDEFGABCDEF","ABCD",7);
    test(3,"ABDXXXXXABCD","CD",11);
    test(4,"ABDXXXXXABCD","DE",0);
    test(5,"ABAAABCD","ABCD",5);
    test(6,"ADFSFBCD","",1);
    test(7,"","ABCD",0);
    test(8,"ABCD","ABCDE",0);

}
