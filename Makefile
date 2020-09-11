CC     = gcc
OPTFLAGS = -march=native -O3 -pipe -march=znver1 -mprefer-vector-width=256  -flto
# -fdevirtualize-at-ltrans -fno-plt -fno-common -fipa-pta -fno-semantic-interposition -fgraphite-identity -floop-nest-optimize
DBGFLAGS = -Wall -Wextra
#  -Werror
# -g

DEFINES=-DVECTORIZED -DVLEN=4 -DTHREADED
CFLAGS = --std=gnu11 $(DBGFLAGS) $(OPTFLAGS) $(DEFINES)
LDFLAGS = -lm -fopenmp $(CFLAGS) -flto=6
#-lreflapacke -lreflapack -lrefblas 
# -lblas -llapack

rk4:
	$(CC) $(CFLAGS) rk4.c -lm -o rk4

intel:
	$(CC) $(CFLAGS) intel.c -lm -o intel

illeszt: elim.o
	$(CC) $(CFLAGS) -o illeszt illeszt.c elim.o matrix.o array.o -lm

default: matrix.o
	$(CC) -o matmul main.c matrix.o array.o

gauss: elim.o
	$(CC) $(CFLAGS) -o gauss invert.c elim.o matrix.o array.o

lapakk: elim.o

	$(CC) $(CFLAGS) -o lapakk lapakk.c elim.o matrix.o array.o

elim.o: matrix.o elim.h
	$(CC) $(CFLAGS) -c elim.c

matrix.o: matrix.c matrix.h array.o
	$(CC) $(CFLAGS) -c matrix.c

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c

.PHONY: clean
clean:
	rm -f illeszt lapakk rk4

.PHONY: lint
lint:
	clang-tidy $(SRCS) $(HDRS) -- $(CFLAGS)
