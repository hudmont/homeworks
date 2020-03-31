CC     = gcc
#OPTFLAGS = -march=amdfam10 -mpopcnt -mcx16 -O3 -pipe
DBGFLAGS = -Wall -Werror -Wextra
CFLAGS = --std=gnu11 $(DBGFLAGS)
#$(OPTFLAGS)
LDFLAGS = -lm
THFLAGS = -fopenmp
# -lblas -llapack

rk4:
	$(CC) rk4.c -lm -o rk4

illeszt: elim.o
	$(CC) -o illeszt illeszt.c elim.o matrix.o array.o -lm

default: matrix.o
	$(CC) -o matmul main.c matrix.o array.o

gauss: elim.o
	$(CC) -o gauss invert.c elim.o matrix.o array.o

lapakk: elim.o

	$(CC) $(THFLAGS) -o lapakk lapakk.c elim.o matrix.o array.o -lm

elim.o: matrix.o elim.h
	$(CC) -c elim.c

matrix.o: matrix.c matrix.h array.o
	$(CC) -c matrix.c

array.o: array.c array.h
	$(CC) -c array.c


.PHONY: clean
clean:
	-rm edit $(objects)
