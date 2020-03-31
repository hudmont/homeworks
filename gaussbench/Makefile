CC=gcc
DEFINES=-DVECTORIZED -DVLEN=4 -DTHREADED

CFLAGS=-march=znver1 -O3 -std=gnu11 -mprefer-vector-width=256 $(DEFINES) -flto -fdevirtualize-at-ltrans -fno-plt -fno-common -fipa-pta -fno-semantic-interposition -fgraphite-identity -floop-nest-optimize
# -g
LDFLAGS=-fopenmp -lm $(CFLAGS) -flto=6
SRCS=$(wildcard *.c)
HDRS=$(wildcard *.h)
OBJS = $(SRCS:.c=.o)

bench: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) bench

.PHONY: lint
lint:
	clang-tidy $(SRCS) $(HDRS) -- $(CFLAGS)
