CC = gcc
CFLAGS = -Wall -g
#CPPFLAGS = 

LDFLAGS = -L../../ -Wl,-rpath=../../
LOADLIBES = -lprompt

EXECUTABLES = tests_parsing
OBJECTS = $(EXECUTABLES).o

all: $(EXECUTABLES) tests

$(EXECUTABLES): %: $(OBJECTS)
	$(CC) $(LDFLAGS) $? $(LOADLIBES) -o $@

$(OBJECTS): %.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(LOADLIBES) $? -o $@ 

.PHONY: tests
tests: %: $(EXECUTABLES)
	./$^

.PHONY: clean
clean:
	rm -f *.i *.s $(OBJECTS) $(EXECUTABLES)