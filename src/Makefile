
#compiler
CC=gcc

#compiler options
OPTS=-c -Wall -Wextra -std=c99

#source files
SOURCES=$(wildcard *.c )

#object files
OBJECTS= $(SOURCES:.c=.o)

EXECUTABLE=ifj.exe
EXECUTABLE_WIN=ifj.run

all: $(EXECUTABLE) $(EXECUTABLE_WIN)
	mv -t bin *.o
	mv -t bin $(EXECUTABLE)
	mv -t bin $(EXECUTABLE_WIN)

$(EXECUTABLE): $(OBJECTS)
	$(LINK.o) $^ -o $@ $(LIBS)


$(EXECUTABLE_WIN): $(OBJECTS)
	$(LINK.o) $^ -o $@ $(LIBS)

clean:
	rm $(EXECUTABLE) $(OBJECTS) $(EXECUTABLE_WIN)

