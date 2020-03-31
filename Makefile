CC     = gcc
OPTFLAGS = -march=native -O3 -pipe
DBGFLAGS = -Wall -Wextra -Werror
CFLAGS = --std=gnu11 $(DBGFLAGS) $(OPTFLAGS)
#LDFLAGS = -lreflapacke -lreflapack -lrefblas -lm
THFLAGS = -fopenmp
# -lblas -llapack

rk4:
	$(CC) $(CFLAGS) rk4.c -lm -o rk4

illeszt: elim.o
	$(CC) $(CFLAGS) -o illeszt illeszt.c elim.o matrix.o array.o -lm

default: matrix.o
	$(CC) -o matmul main.c matrix.o array.o

gauss: elim.o
	$(CC) $(CFLAGS) -o gauss invert.c elim.o matrix.o array.o

lapakk: elim.o

	$(CC) $(THFLAGS) -o lapakk lapakk.c elim.o matrix.o array.o

elim.o: matrix.o elim.h
	$(CC) $(CFLAGS) -c elim.c

matrix.o: matrix.c matrix.h array.o
	$(CC) $(CFLAGS) -c matrix.c

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c


#clean:
#	rm matrix.o elim.o matmul gauss lapakk illeszt
