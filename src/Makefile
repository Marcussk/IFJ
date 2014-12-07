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

all: $(EXECUTABLE) 
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(LINK.o) $^ -o $@ $(LIBS)

clean:
	-@rm -rf $(EXECUTABLE) $(OBJECTS) 2>/dev/null || true
