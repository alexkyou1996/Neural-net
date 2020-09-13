CC=gcc
CFLAGS=-O0 -g -Wall -Wextra -Wconversion
INCLUDES=
LIBS=
OBJS=

%.o: %.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS)

matrix_test: matrix_test.c matrix.o logging.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o matrix_test
