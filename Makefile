CC     = gcc
# csakis gcc a beágyazott függvények miatt
OPTFLAGS = -march=native -O3 -pipe -march=znver1 -mprefer-vector-width=256  -flto
# -fdevirtualize-at-ltrans -fno-plt -fno-common -fipa-pta -fno-semantic-interposition -fgraphite-identity -floop-nest-optimize
DBGFLAGS = -Wall -Wextra

DEFINES=-DPRECISE -DVLEN=4 -DTHREADED
#-DNOCOPY
CFLAGS = --std=gnu17 $(DBGFLAGS) $(OPTFLAGS) $(DEFINES) -lm -fopenmp -flto=6
LDFLAGS = $(CFLAGS)
#-lreflapacke -lreflapack -lrefblas 
# -lblas -llapack

src = elim.c matrix.c array.c
obj = $(src:.c=.o)
hdr = $(src:.c=.h)
targets = rk4 intel illeszt matmul lapakk

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(targets): $(obj)
	$(CC) $(CFLAGS) -o $@ $(obj) $@.c $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(targets)

.PHONY: lint
lint:
	clang-tidy $(src) $(hdr) -- $(CFLAGS)
