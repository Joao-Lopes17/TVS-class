CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lsystemd

EXECUTABLES = tvsctld
OBJECTS = $(EXECUTABLES).o

all: $(EXECUTABLES)

$(EXECUTABLES): %: $(OBJECTS)
	$(CC) $? $(LDFLAGS) -o ../bin/$@

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $? -o $@ 

.PHONY: clean
clean:
	rm -f $(OBJECTS) *.i *.s $(EXECUTABLES)