CC = gcc
CFLAGS = -Wall -g
#CPPFLAGS = 

LIBRARIES = libprompt
OBJECTS = prompt.o parsing.o

all: $(LIBRARIES).so $(LIBRARIES).a

$(LIBRARIES).so: %.so: $(OBJECTS)
	$(CC) -shared -Wl,-soname,$@ -o ../$@ $^

$(LIBRARIES).a: %.a: $(OBJECTS)
	ar -cr ../$@ $?

$(OBJECTS): %.o: %.c
	$(CC) -c -fpic $(CPPFLAGS) $(CFLAGS) -o $@ $^

.PHONY: tests
tests:
	make -C ./tests -f Makefile clean all

.PHONY: clean
clean:
	rm -f *.i *.s $(OBJECTS)
	rm -f ../$(LIBRARIES).so ../$(LIBRARIES).a