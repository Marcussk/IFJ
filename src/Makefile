export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

#compiler
CC=gcc

#compiler options
OPTS=-c -pipe -Wall -std=c99 -Wextra 

#source files
SOURCES=$(wildcard *.c )

#object files
OBJECTS= $(SOURCES:.c=.o)

EXECUTABLE=ifj.run
#EXECUTABLE_WIN=ifj.exe

all: $(EXECUTABLE) 
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(LINK.o) $^ -o $@ $(LIBS)


#$(EXECUTABLE_WIN): $(OBJECTS)
#	$(LINK.o) $^ -o $@ $(LIBS)

clean:
	rm $(EXECUTABLE) $(OBJECTS) #$(EXECUTABLE_WIN)

