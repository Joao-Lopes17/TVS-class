CC = gcc
CFLAGS = -Wall -g
LDFLAGS = #-lsystemd

EXECUTABLES = tvsctl
OBJECTS = $(EXECUTABLES).o

all: $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)
	mkdir -p ../bin
	$(CC) $(OBJECTS) $(LDFLAGS) -o ../bin/$@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY: clean
clean:
	rm -f $(OBJECTS) *.i *.s $(EXECUTABLES)