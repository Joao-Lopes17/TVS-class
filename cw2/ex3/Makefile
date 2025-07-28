CC = gcc
CFLAGS = -Wall -g

EXECUTABLE = prog

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): prog.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) prog.o

prog.o: prog.c
	$(CC) $(CFLAGS) -c prog.c

clean:
	rm -f *.o $(EXECUTABLE)


# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -fPIC -Wall
LDFLAGS = -shared

# Nome dos binários
LIBRARY = library.so
EXECUTABLE = prog

.PHONY: all clean

# Arquivos fonte
LIB_SRC = library.c
CODE_SRC = prog.c

# Objetivos principais
all: $(LIBRARY) $(EXECUTABLE)

# Compilação da biblioteca compartilhada
$(LIBRARY): $(LIB_SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Compilação do programa principal
$(EXECUTABLE): $(CODE_SRC)
	$(CC) $(CODE_SRC) -ldl -o $@

# Limpeza dos arquivos binários
clean:
	rm -f $(LIBRARY) $(EXECUTABLE)
