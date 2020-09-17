CC     = gcc # egyelőre csakis gcc a beágyazott helper függvények miatt
OPTFLAGS =-march=native -O3 -pipe -march=znver1 -mprefer-vector-width=256  -flto
# -fdevirtualize-at-ltrans -fno-plt -fno-common -fipa-pta -fno-semantic-interposition -fgraphite-identity -floop-nest-optimize
DBGFLAGS = -Wall -Wextra
#-DDEBUG -g

DEFINES=-DPRECISE -DVLEN=4 -DTHREADED
CFLAGS = --std=gnu17 $(DBGFLAGS) $(OPTFLAGS) $(DEFINES) -fopenmp
LDFLAGS = -lm -flto=6
#-lreflapacke -lreflapack -lrefblas -lblas -llapack

SRCS = elim.c matrix.c array.c
OBJS = $(SRCS:.c=.o)
HDRS = $(SRCS:.c=.h)
TARGETS = rk4 intel illeszt matmul lapakk

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $@.c $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGETS)

.PHONY: lint
lint:
	clang-tidy $(SRCS) $(HDRS) -- $(CFLAGS)
