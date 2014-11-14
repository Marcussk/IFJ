#include "func.h"

int func_len (char *str){
		
		return strlen(str);
}

char *func_copy (char *f_str, int  i, int n){

		
		/*
		char inner_str[n];
		
		strcpy( inner_str , str );
		printf("vrateny ratazec je : %s %s \n", str , inner_str);	
	
		char *copy_str = &inner_str[i]; 
		while(n--)
			*copy_str++ ;

		*copy_str = '\0';	
			printf("vrateny ratazec je : %s \n", inner_str);

		return &inner_str[i]; */

		int j;
		char inner_str[n];
		char *str = &f_str[i-1];

        for(j = 0; (str[j] != '\0') && (j <  n-1) ; ++j){
                inner_str[j] = str[j];
        }       
       		inner_str[j] = str[j];

        return inner_str;

}

 

int main (){

		char retazec[] = "'x'#10'z'";
		printf("retazec je: %s \n", retazec );
		printf("Dlzka retazca je: %d \n", func_len(retazec) );

		char* copy = func_copy(retazec, 10, 5);
		printf("kopirovany ratazec je : %s \n", copy );
		printf("povodny retazec je: %s \n", retazec );

		return 0;

}
