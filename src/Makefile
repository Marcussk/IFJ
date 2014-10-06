

all: lexParser.o ifj.o tokenMap.o
	gcc -Wall *.o -o ifj.out

ifj.o: ifj.c defs.h 
	gcc -Wall ifj.c defs.h -c

lexParser.o: lexParser.c tokenMap.o strRoutines.o
	gcc -Wall lexParser.c tokenMap.h -c

hashTable.o: hashTable.c
	gcc -Wall hashTable.c -c

errorHandler.o:
	gcc -Wall errorHandler.c -c 

tokenMap.o: tokenMap.c errorHandler.o
	gcc -Wall tokenMap.c -c

strRoutines.o: strRoutines.c
	gcc -Wall strRoutines.c -c

clean:
	rm ifj.out *.o *.gch
