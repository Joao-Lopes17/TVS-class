CC = gcc
CFLAGS = -Wall -g
#CPPFLAGS = 

LDFLAGS = -L./libs -Wl,-rpath=./libs
LOADLIBES = -lprompt

EXECUTABLES = bach
OBJECTS = $(EXECUTABLES).o

all: $(EXECUTABLES)

$(EXECUTABLES): %: $(OBJECTS)
	$(CC) $(LDFLAGS) $? $(LOADLIBES) -o $@

$(OBJECTS): %.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(LOADLIBES) $? -o $@ 

.PHONY: libs
libs:
	make -C ./libs/prompt -f Makefile clean all

.PHONY: tests
tests:
	make -C ./libs/prompt -f Makefile clean all tests

.PHONY: clean
clean:
	rm -f $(OBJECTS) *.i *.s $(EXECUTABLES)