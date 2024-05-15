CC=gcc
CFLAGS=-g3 -O0

SRCDIR=./


SRC=$(wildcard $(SRCDIR)*.c)


OBJ=$(SRC:.c=.o)

EXECUTABLE=Program.exe

.PHONY: all clean

all: $(EXECUTABLE)

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)